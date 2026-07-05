## Zephyr-specific Portenta C33 bootloader

The `c33_bl.bin` file is the Zephyr-specific Portenta C33 bootloader. It is a
binary file that can be flashed to the Portenta C33 to enable it to boot Zephyr
applications.

Compared to the default C33 bootloader, the Zephyr-specific bootloader supports
an additional `alt` section for the sketch to be loaded.

### Sources and compilation instructions

The source code for the Zephyr-specific C33 bootloader can be found in the
[`arduino-renesas-bootloader` repository](https://github.com/arduino/arduino-renesas-bootloader/tree/zephyr_alt/)
on GitHub. The Zephyr-specific bootloader uses the `zephyr_alt` branch.

This binary can be regenerated with the following commands:

```bash
git clone https://github.com/arduino/arduino-renesas-bootloader
git clone https://github.com/hathach/tinyusb
cd tinyusb
patch -p1 < ../arduino-renesas-bootloader/0001-fix-arduino-bootloaders.patch
python tools/get_deps.py ra
cd ..
cd arduino-renesas-bootloader
git checkout zephyr_alt
TINYUSB_ROOT=$PWD/../tinyusb make -f Makefile.c33
```

### License

The Zephyr-specific Portenta C33 bootloader is licensed under the MIT license.
