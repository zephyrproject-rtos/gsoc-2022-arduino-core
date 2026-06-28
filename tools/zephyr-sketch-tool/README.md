## Zephyr Sketch Tool

This tool converts various binary files into a format that can be used
by the Zephyr loader.

The loader expects to find a specific header in a fixed location in the binary
file, with information about the sketch and the build options chosen by the
user. The location of the header was selected so that it affects unused bytes
in the ELF file format; when dealing with binary files, 16 bytes are added at
the beginning to reserve space for the header.

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
