#!/usr/bin/env python3
#
# Copyright (c) 2026 TOKITA Hiroshi
#
# SPDX-License-Identifier: Apache-2.0

from __future__ import annotations

from pathlib import Path
from typing import Iterator, Tuple

from jinja2 import Environment, FileSystemLoader

from .service_model import ServiceModel

__all__ = [
    "ServiceRenderer",
]

_TEMPLATES_DIR = Path(__file__).resolve().parent / "templates"
_ENV = Environment(
    loader=FileSystemLoader(str(_TEMPLATES_DIR)),
    autoescape=False,
    trim_blocks=True,
    lstrip_blocks=True,
    keep_trailing_newline=True,
)


class ServiceRenderer:
    """Renders a ServiceModel into generated files."""

    def __init__(self, model: ServiceModel) -> None:
        self._model = model

    def __iter__(self) -> Iterator[Tuple[str, str]]:
        yield self._model.interface_header, self._render_header()

    @staticmethod
    def _render_template(template_name: str, **context) -> str:
        rendered = _ENV.get_template(template_name).render(**context)
        return rendered if rendered.endswith("\n") else f"{rendered}\n"

    def _render_header(self) -> str:
        return ServiceRenderer._render_template(
            "ifc_header.hpp.j2",
            includes=self._model.include_list,
            namespace_name=self._model.namespace_name,
            methods=self._model.methods,
            proto_enums=self._model.proto_enums,
            interface_name=self._model.interface_name,
            base_interface_class_names=self._model.base_interface_class_names,
        )
