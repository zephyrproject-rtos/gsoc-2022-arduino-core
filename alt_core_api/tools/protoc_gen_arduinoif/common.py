#!/usr/bin/env python3
#
# Copyright (c) 2026 TOKITA Hiroshi
#
# SPDX-License-Identifier: Apache-2.0

from __future__ import annotations

import importlib.util
import os
import re
import sys
import types
from functools import lru_cache
from pathlib import Path
from typing import List

__all__ = [
    "full_service_name",
    "snake_case",
    "uniq",
    "get_arduino_opts_pb2",
]


def full_service_name(pkgname: str, servicename: str) -> str:
    return f".{pkgname}.{servicename}" if pkgname else f".{servicename}"


def snake_case(name: str) -> str:
    word_edges = re.sub(r"(.)([A-Z][a-z]+)", r"\1_\2", name)
    return re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", word_edges).lower()


def uniq(values: List[str]) -> List[str]:
    return list(dict.fromkeys(values))


@lru_cache(maxsize=1)
def get_arduino_opts_pb2() -> types.ModuleType:
    module_name = "_protoc_gen_arduinoif_arduino_opts_pb2"
    pb2_path = os.environ.get("PROTOC_GEN_ARDUINOIF_PB2")
    if not pb2_path:
        raise RuntimeError(
            "PROTOC_GEN_ARDUINOIF_PB2 is not set; it must point to the "
            "generated arduino_opts_pb2.py file before running "
            "protoc-gen-arduinoif"
        )

    source_path = Path(pb2_path)
    if not source_path.exists():
        raise RuntimeError(
            f"PROTOC_GEN_ARDUINOIF_PB2 points to a missing file: '{source_path}'"
        )

    spec = importlib.util.spec_from_file_location(module_name, source_path)
    if spec is None or spec.loader is None:
        raise RuntimeError(
            "failed to load arduino_opts_pb2 from "
            f"PROTOC_GEN_ARDUINOIF_PB2='{source_path}'"
        )

    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    spec.loader.exec_module(module)
    return module
