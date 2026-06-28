## Rodata Linker Script Generator

This tool analyzes ELF files and generates linker script fragments for Zephyr
two-pass linking, separating read-only data sections based on relocations.

The tool examines `.rodata` sections in a temporary ELF file and generates a linker
script that places sections with relocations into `.rodata` (copied to RAM by LLEXT)
and sections without relocations into `.rodata.noreloc` (kept in flash). This
optimization significantly reduces RAM usage for LLEXT applications.

### Getting the tool

If you have installed the Arduino IDE and the Arduino core for Zephyr, you can
find the pre-built files in the `.arduino15/packages/arduino/tools/` folder in
your home directory. You can directly use the tool from there.

### Building manually

To build the tool, you need to have the Go programming language installed; make
sure you have the `go` command available in your PATH. Then, use the `go build`
command to build the tool for your platform.

To build the full set of binaries for all platforms, run the `package_tool.sh`
script in the parent directory with `../package_tool.sh`, or provide the path
to this directory as an argument.
