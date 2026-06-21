#!/usr/bin/env python3
#
# Copyright (c) 2026 TOKITA Hiroshi
#
# SPDX-License-Identifier: Apache-2.0

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Tuple

from google.protobuf.compiler import plugin_pb2
from google.protobuf.descriptor_pb2 import DescriptorProto, ServiceDescriptorProto

from .common import full_service_name

__all__ = [
    "RequestContext",
]


ServiceDescriptor = Tuple[ServiceDescriptorProto, str]


@dataclass
class RequestContext:
    """Descriptor indexes shared while generating one CodeGeneratorRequest."""

    message_map: Dict[str, DescriptorProto]
    service_index: Dict[str, ServiceDescriptor]
    lineage_cache: Dict[str, List[str]]

    @classmethod
    def build(cls, request: plugin_pb2.CodeGeneratorRequest) -> "RequestContext":
        message_map: Dict[str, DescriptorProto] = {}
        service_index: Dict[str, ServiceDescriptor] = {}

        for proto_file in request.proto_file:
            prefix = f".{proto_file.package}" if proto_file.package else ""
            for message in proto_file.message_type:
                cls._add_message(message_map, prefix, message)
            for service in proto_file.service:
                full_name = full_service_name(proto_file.package, service.name)
                service_index[full_name] = (service, proto_file.package)

        return cls(
            message_map=message_map,
            service_index=service_index,
            lineage_cache={},
        )

    @staticmethod
    def _add_message(
        message_map: Dict[str, DescriptorProto],
        scope: str,
        message: DescriptorProto,
    ) -> None:
        full_name = f"{scope}.{message.name}"
        message_map[full_name] = message

        for nested in message.nested_type:
            RequestContext._add_message(message_map, full_name, nested)
