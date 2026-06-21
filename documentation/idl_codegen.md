# Arduino IDL Codegen

This document describes the current `protoc-gen-arduinoif` flow used by
`alt_core_api`.

The `.proto` files are treated as the first-class Arduino API IDL.  The
current generator target is deliberately small: it emits C++ interface headers
from protobuf service definitions.  The same IDL should remain useful later as
input for other renderers, such as RPC adapters or stubs.

## Goals

- Define Arduino-compatible API surfaces as protobuf services.
- Generate pure virtual C++ interface headers from those services.
- Keep Arduino/C++ compatibility details in explicit protobuf options.
- Preserve enough semantic information for future non-header targets.

The generator does not currently emit API wrappers, service contracts,
service implementations, transports, dispatch tables, or runtime RPC code.

## Files

- Generator entrypoint: `alt_core_api/tools/protoc-gen-arduinoif`
- Python package: `alt_core_api/tools/protoc_gen_arduinoif/`
- Options: `alt_core_api/idl/proto/arduino_opts.proto`
- IDL sources: `alt_core_api/idl/proto/*.proto`
- CMake integration: `alt_core_api/idl/CMakeLists.txt`

Generated headers are written under the build tree:

```text
build/modules/arduinocore-zephyr/alt_core_api/idl/generated/arduinoif/
```

## Prerequisites

The plugin is a Python program executed by `protoc` during the build.  The
Python environment used by the build must provide:

- `google.protobuf`
- `jinja2`

In the normal Zephyr workflow these modules should be installed in the same
Python environment used by `west build` (typically the project virtual
environment).  When invoking `protoc` manually, run it with that environment on
`PATH` so the plugin's `#!/usr/bin/env python3` resolves to the expected Python
interpreter.

## Build Integration

`alt_core_api/idl/CMakeLists.txt` drives generation.

The build first generates `arduino_opts_pb2.py` from `arduino_opts.proto`, then
passes that file to the plugin with `PROTOC_GEN_ARDUINOIF_PB2`.  The plugin
requires this environment variable so extension descriptors are registered
before parsing the `CodeGeneratorRequest`.

Class-style API proto files currently generate `*_interface.hpp` headers.
Shared enum-only proto files generate `*_types.h` headers.

## Processing Model

The generator follows a small descriptor-to-template pipeline:

1. `core.py` receives the protoc `CodeGeneratorRequest` and selects
   `file_to_generate`.
2. `RequestContext` indexes messages and services from the request.
3. `ServiceModelBuilder` converts each protobuf service into a template-ready
   `ServiceModel`.
4. `ServiceRenderer` renders the service model with `ifc_header.hpp.j2`.
5. `EnumRenderer` renders top-level enum-only proto files as C-compatible type
   headers.

Only the request context is shared across services.  Service models are built
and rendered one at a time.

## IDL Mapping

Use this mapping when defining Arduino APIs:

- A protobuf `service` represents an Arduino interface/class surface.
- An `rpc` represents one callable API operation.
- The input message fields represent C++ method parameters, in descriptor field
  order.
- The output message represents the C++ return value.
- `google.protobuf.Empty` maps to `void`.
- A single-field output message maps to that field's C++ type.
- Multi-field output messages are currently rejected.
- Separate proto method names should be used for C++ overloads.
- `cpp_name` maps those proto method names back to the Arduino-compatible C++
  name.

For example, `Print.WriteByte` and `Print.WriteBuffer` are distinct protobuf
methods, but both map to the C++ overload set named `write`.

## Field Options

Defined as `google.protobuf.FieldOptions` extensions:

- `cpp_type`
  Overrides the C++ type used for a field.
- `field_cpp_name`
  Overrides the generated C++ parameter name.

If `cpp_type` is omitted, the generator uses a small built-in scalar mapping
such as `uint32 -> uint32_t`, `bool -> bool`, and `bytes -> const uint8_t *`.

## Method Options

Defined as `google.protobuf.MethodOptions` extensions:

- `cpp_name`
  C++ method name.  If omitted, the protobuf RPC name is used.
- `cpp_return`
  C++ return type override.  If omitted, the output message is inferred.
- `method_visibility`
  Access specifier for the generated pure virtual method.  Supported values are
  `public`, `protected`, and `private`.  The default is `public`.

The generator rejects duplicate C++ declarations within the same service.  This
catches cases where two protobuf RPCs accidentally map to the same C++ method
signature.

## Service Options

Defined as `google.protobuf.ServiceOptions` extensions:

- `interface_class_name`
  Overrides the generated interface class name.
- `interface_header_name`
  Overrides the generated header name.
- `base_services`
  Names parent services that the generated interface should inherit from.
- `extra_includes`
  Adds headers to the generated interface header.

`base_services` may use a same-package service name, a package-qualified name,
or a leading-dot fully-qualified service name.  Generated C++ inheritance uses
`virtual public` for base interfaces so diamonds such as
`Print -> Stream -> HardwareSerial` remain safe.

## Example

```proto
syntax = "proto3";

package arduino;

import "arduino_opts.proto";
import "google/protobuf/empty.proto";
import "google/protobuf/wrappers.proto";

message PrintWriteByteRequest {
  uint32 value = 1 [
    (arduino.cpp_type) = "uint8_t",
    (arduino.field_cpp_name) = "arg0"
  ];
}

service Print {
  rpc WriteByte(PrintWriteByteRequest) returns (google.protobuf.UInt32Value) {
    option (arduino.cpp_name) = "write";
    option (arduino.cpp_return) = "size_t";
  }

  rpc Flush(google.protobuf.Empty) returns (google.protobuf.Empty) {
    option (arduino.cpp_name) = "flush";
  }
}
```

This produces a pure virtual interface similar to:

```cpp
class PrintInterface {
public:
  virtual ~PrintInterface() = default;

public:
  virtual size_t write(uint8_t arg0) = 0;
public:
  virtual void flush() = 0;
};
```

## Command Line Example

```sh
protoc \
  --python_out=/tmp/arduinoif-pb2 \
  --proto_path=alt_core_api/idl/proto \
  alt_core_api/idl/proto/arduino_opts.proto

PROTOC_GEN_ARDUINOIF_PB2=/tmp/arduinoif-pb2/arduino_opts_pb2.py \
protoc \
  --plugin=protoc-gen-arduinoif=alt_core_api/tools/protoc-gen-arduinoif \
  --arduinoif_out=/tmp/arduinoif-gen \
  --proto_path=alt_core_api/idl/proto \
  --proto_path=/path/to/nanopb/generator/proto \
  alt_core_api/idl/proto/print.proto
```

In normal Zephyr builds this is handled by CMake.

## Validation Rules

Generation fails fast when the IDL cannot be mapped cleanly:

- Unknown output message type.
- Output message with more than one field.
- Repeated fields without an explicit `cpp_type`.
- Unsupported protobuf field types without an explicit `cpp_type`.
- Unsupported `method_visibility` value.
- Cyclic `base_services` references.
- Unknown `base_services` reference.
- Duplicate generated C++ declarations in one service.

## Notes

- The current generator emits headers only.
- Protobuf is used as an IDL/DSL, not as a commitment to a protobuf runtime
  transport.
- Future RPC adapter generation should be added as another renderer over the
  same semantic model.
- Keep compatibility exceptions in `.proto` options so generated output remains
  deterministic and reviewable.
