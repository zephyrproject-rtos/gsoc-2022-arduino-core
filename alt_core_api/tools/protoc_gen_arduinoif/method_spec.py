#!/usr/bin/env python3
#
# Copyright (c) 2026 TOKITA Hiroshi
#
# SPDX-License-Identifier: Apache-2.0

from __future__ import annotations

from typing import Dict, List, NamedTuple, Tuple

from google.protobuf.descriptor_pb2 import DescriptorProto, FieldDescriptorProto

__all__ = [
    "MethodSpec",
]

_DEFAULT_FIELD_CPP_TYPES: Dict[int, str] = {
    FieldDescriptorProto.TYPE_BOOL: "bool",
    FieldDescriptorProto.TYPE_INT32: "int32_t",
    FieldDescriptorProto.TYPE_INT64: "int64_t",
    FieldDescriptorProto.TYPE_UINT32: "uint32_t",
    FieldDescriptorProto.TYPE_UINT64: "uint64_t",
    FieldDescriptorProto.TYPE_SINT32: "int32_t",
    FieldDescriptorProto.TYPE_SINT64: "int64_t",
    FieldDescriptorProto.TYPE_FIXED32: "uint32_t",
    FieldDescriptorProto.TYPE_FIXED64: "uint64_t",
    FieldDescriptorProto.TYPE_SFIXED32: "int32_t",
    FieldDescriptorProto.TYPE_SFIXED64: "int64_t",
    FieldDescriptorProto.TYPE_FLOAT: "float",
    FieldDescriptorProto.TYPE_DOUBLE: "double",
    FieldDescriptorProto.TYPE_STRING: "const char *",
    FieldDescriptorProto.TYPE_BYTES: "const uint8_t *",
    FieldDescriptorProto.TYPE_ENUM: "int32_t",
}


def _opt_str(options, extension) -> str:
    try:
        return str(options.Extensions[extension]).strip()
    except (AttributeError, KeyError):
        return ""


class MethodSpec(NamedTuple):
    """C++ method signature derived from one protobuf RPC method."""

    decl: str
    overload_key: Tuple[str, Tuple[str, ...]]
    call_name: str
    arg_names: List[str]
    returns_void: bool
    visibility: str

    @classmethod
    def build(
        cls,
        method,
        opts_pb2,
        message_map: Dict[str, DescriptorProto],
    ) -> "MethodSpec":
        def _resolve_field(field: FieldDescriptorProto) -> Tuple[str, str]:
            field_opts = field.options
            explicit_cpp_type = _opt_str(field_opts, opts_pb2.cpp_type)
            if explicit_cpp_type:
                field_type = explicit_cpp_type
            else:
                if field.label == FieldDescriptorProto.LABEL_REPEATED:
                    raise ValueError(
                        f"{method.name}: field '{field.name}' is repeated; "
                        "C++ header generation requires an explicit "
                        "(arduino.cpp_type) override"
                    )
                if field.type not in _DEFAULT_FIELD_CPP_TYPES:
                    raise ValueError(
                        f"{method.name}: field '{field.name}' has unsupported "
                        f"protobuf type '{field.type}'; C++ header generation "
                        "requires an explicit (arduino.cpp_type) override"
                    )
                field_type = _DEFAULT_FIELD_CPP_TYPES[field.type]
            field_name = _opt_str(field_opts, opts_pb2.field_cpp_name) or field.name
            return field_type, field_name

        input_msg = message_map.get(method.input_type)
        output_msg = message_map.get(method.output_type)

        if input_msg is None:
            raise ValueError(
                f"{method.name}: unknown input type '{method.input_type}'; "
                "use google.protobuf.Empty for methods without parameters"
            )
        if output_msg is None:
            raise ValueError(
                f"{method.name}: unknown output type '{method.output_type}'; "
                "use google.protobuf.Empty for void methods"
            )
        if len(output_msg.field) > 1:
            raise ValueError(
                f"{method.name}: output type '{method.output_type}' has "
                f"{len(output_msg.field)} fields; C++ header generation supports "
                "void outputs or a single return-value field"
            )

        visibility = _opt_str(method.options, opts_pb2.method_visibility).lower()
        if visibility not in {"public", "protected", "private", ""}:
            raise ValueError(
                f"{method.name}: unsupported method_visibility '{visibility}' "
                "(expected: public, protected, private)"
            )
        if not visibility:
            visibility = "public"

        method_name = _opt_str(method.options, opts_pb2.cpp_name) or method.name
        return_type = _opt_str(method.options, opts_pb2.cpp_return)
        if not return_type:
            return_type = (
                "void"
                if len(output_msg.field) == 0
                else _resolve_field(output_msg.field[0])[0]
            )

        params = [_resolve_field(f) for f in input_msg.field]
        arg_types = tuple(ftype for ftype, _ in params)
        arg_names = [name for _, name in params]
        params_blob = ", ".join(f"{ftype} {fname}" for ftype, fname in params)

        decl = (
            f"{method_name}({params_blob})"
            if method_name.startswith("operator ")
            else f"{return_type} {method_name}({params_blob})"
        )

        return cls(
            decl=decl,
            overload_key=(method_name, arg_types),
            call_name=method_name,
            arg_names=arg_names,
            returns_void=(return_type == "void"),
            visibility=visibility,
        )
