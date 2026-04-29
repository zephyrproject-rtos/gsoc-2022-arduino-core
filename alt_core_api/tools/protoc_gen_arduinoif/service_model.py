#!/usr/bin/env python3
#
# Copyright (c) 2026 TOKITA Hiroshi
#
# SPDX-License-Identifier: Apache-2.0

from __future__ import annotations

from typing import List, NamedTuple, Optional

from google.protobuf.descriptor_pb2 import EnumDescriptorProto, ServiceDescriptorProto

from .common import full_service_name, get_arduino_opts_pb2, snake_case, uniq
from .request_context import RequestContext
from .method_spec import MethodSpec

__all__ = [
    "ServiceModel",
    "ServiceModelBuilder",
]


class _OptionsView:
    """Small wrapper for optional protobuf extension fields."""

    def __init__(self, options) -> None:
        self._options = options

    def has(self, extension) -> bool:
        try:
            return self._options.HasExtension(extension)
        except (AttributeError, KeyError):
            return False

    def string(self, extension) -> str:
        if not self.has(extension):
            return ""
        return str(self._options.Extensions[extension])

    def string_list(self, extension) -> List[str]:
        try:
            values = self._options.Extensions[extension]
        except (AttributeError, KeyError):
            return []
        return [text for text in (str(value).strip() for value in values) if text]


class ServiceModel(NamedTuple):
    """Template-ready description of a generated interface header."""

    include_list: List[str]
    namespace_name: str
    proto_enums: List[EnumDescriptorProto]
    interface_name: str
    base_interface_class_names: List[str]
    interface_header: str
    methods: List[MethodSpec]


class ServiceModelBuilder:
    """Builds a ServiceModel from one protobuf service descriptor."""

    def __init__(
        self,
        service,
        package_name: str,
        proto_enums: List[EnumDescriptorProto],
        context: RequestContext,
    ) -> None:
        self._service = service
        self._package_name = package_name
        self._proto_enums = proto_enums
        self._context = context
        self._opts_pb2 = get_arduino_opts_pb2()
        self._service_full_name = full_service_name(package_name, service.name)

    @classmethod
    def build(
        cls,
        service,
        package_name: str,
        proto_enums: List[EnumDescriptorProto],
        context: RequestContext,
    ) -> ServiceModel:
        return cls(service, package_name, proto_enums, context)._build()

    def _build(self) -> ServiceModel:
        lineage = self._collect_service_lineage(self._service_full_name)
        methods = self._collect_own_methods()
        base_services = self._direct_base_services()
        base_if_headers = uniq([self._interface_header(bs) for bs in base_services])
        base_if_names = uniq([self._interface_name(bs) for bs in base_services])
        inc_list = uniq([*base_if_headers, *self._collect_lineage_includes(lineage)])

        namespace_name = "::".join(p for p in self._package_name.split(".") if p)

        return ServiceModel(
            include_list=inc_list,
            namespace_name=namespace_name,
            proto_enums=self._proto_enums,
            interface_name=self._interface_name(self._service),
            base_interface_class_names=base_if_names,
            interface_header=self._interface_header(self._service),
            methods=methods,
        )

    def _service_strings(self, svc, opt):
        return _OptionsView(svc.options).string_list(opt)

    def _collect_own_methods(self) -> List[MethodSpec]:
        methods: List[MethodSpec] = []
        overloads = set()
        for method in self._service.method:
            spec = MethodSpec.build(method, self._opts_pb2, self._context.message_map)
            if spec.overload_key in overloads:
                raise ValueError(
                    f"{self._service.name}.{method.name}: duplicated '{spec.decl}'"
                )

            overloads.add(spec.overload_key)
            methods.append(spec)

        return methods

    def _interface_name(self, service) -> str:
        service_opts = _OptionsView(service.options)
        return (
            service_opts.string(self._opts_pb2.interface_class_name).strip()
            or f"{service.name}Interface"
        )

    def _interface_header(self, service) -> str:
        service_opts = _OptionsView(service.options)
        return (
            service_opts.string(self._opts_pb2.interface_header_name).strip()
            or f"{snake_case(service.name)}_interface.hpp"
        )

    def _direct_base_services(self) -> List[ServiceDescriptorProto]:
        return [
            self._context.service_index[base_full_name][0]
            for base_full_name in self._direct_base_names()
        ]

    def _direct_base_names(self) -> List[str]:
        return self._base_service_names(self._service, self._package_name)

    def _base_service_names(self, service, package_name: str) -> List[str]:
        return [
            self._resolve_service_reference(base_ref, package_name)
            for base_ref in self._service_strings(service, self._opts_pb2.base_services)
        ]

    def _collect_service_lineage(
        self,
        service_full_name: str,
        visiting: Optional[List[str]] = None,
    ) -> List[str]:
        if visiting is None:
            visiting = []

        if service_full_name in self._context.lineage_cache:
            return self._context.lineage_cache[service_full_name]
        if service_full_name in visiting:
            cycle = " -> ".join([*visiting, service_full_name])
            raise ValueError(f"cyclic service inheritance detected: {cycle}")
        entry = self._context.service_index.get(service_full_name)
        if entry is None:
            raise ValueError(f"service '{service_full_name}' not found")

        service, package_name = entry

        inherited: List[str] = []
        for base_ref in self._service_strings(service, self._opts_pb2.base_services):
            ancestors = self._collect_service_lineage(
                self._resolve_service_reference(base_ref, package_name),
                [*visiting, service_full_name],
            )
            inherited.extend(ancestors)

        lineage = uniq([*inherited, service_full_name])
        self._context.lineage_cache[service_full_name] = lineage

        return lineage

    def _resolve_service_reference(self, ref: str, current_pkg: str) -> str:
        if ref.startswith("."):
            candidate = ref
        elif "." not in ref and current_pkg:
            candidate = f".{current_pkg}.{ref}"
        else:
            candidate = f".{ref}"

        if candidate not in self._context.service_index:
            raise ValueError(f"service '{candidate}' not found in '{ref}'")

        return candidate

    def _collect_lineage_includes(self, lineage: List[str]) -> List[str]:
        includes: List[str] = []
        for service_full_name in lineage:
            svc, _ = self._context.service_index[service_full_name]
            includes.extend(self._service_strings(svc, self._opts_pb2.extra_includes))

        return includes
