#!/usr/bin/env python3
#
# Copyright (c) 2026 TOKITA Hiroshi
#
# SPDX-License-Identifier: Apache-2.0

from __future__ import annotations

import sys
from typing import Iterator, Tuple

from google.protobuf.compiler import plugin_pb2 as pb2

from .common import get_arduino_opts_pb2
from .enum_renderer import EnumRenderer
from .service_renderer import ServiceRenderer
from .service_model import ServiceModelBuilder
from .request_context import RequestContext

__all__ = [
    "main",
]


def _iter_generated_files(
    request: pb2.CodeGeneratorRequest,
) -> Iterator[Tuple[str, str]]:
    context = RequestContext.build(request)
    proto_names = {proto_file.name: proto_file for proto_file in request.proto_file}
    generated_files = [proto_names[name] for name in request.file_to_generate]

    for proto_file in generated_files:
        if proto_file.enum_type and not proto_file.service:
            yield from EnumRenderer(proto_file.name, list(proto_file.enum_type))

        for service in proto_file.service:
            service_model = ServiceModelBuilder.build(
                service,
                proto_file.package,
                list(proto_file.enum_type),
                context,
            )
            yield from ServiceRenderer(service_model)


def main() -> int:
    # Extensions must be registered before parsing request payload.
    get_arduino_opts_pb2()
    req = pb2.CodeGeneratorRequest()
    req.ParseFromString(sys.stdin.buffer.read())
    resp = pb2.CodeGeneratorResponse()

    try:
        for name, content in _iter_generated_files(req):
            output = resp.file.add()
            output.name = name
            output.content = content
    except Exception as error:
        resp.error = str(error)

    if hasattr(pb2.CodeGeneratorResponse, "FEATURE_PROTO3_OPTIONAL"):
        resp.supported_features = pb2.CodeGeneratorResponse.FEATURE_PROTO3_OPTIONAL

    sys.stdout.buffer.write(resp.SerializeToString())
    return 0
