# Adding custom boards/ variants

- Boards already supported by Zephyr can be added to the variants folder as outlined in this documentation.
- Custom boards can first by added by following the [official Zephyr porting guide](https://docs.zephyrproject.org/latest/hardware/porting/board_porting.html).
Once completed, continue here by adding a variant for your custom board.

## Suppored Boards/variants

- [X] Arduino Nano ble sense 33
- [X] Arduino Nano ble 33
- [X] Arduino Nano 33 iot

## Planned Support: (TODO)
- [ ] Particle Xenon
- [ ] Arduino mkrzero
- [ ] TI-CC3220SF LaunchXL
- [ ] nrf52840dk_nrf52840

## How to add board variants

This module uses the board name (supplied at build time by the `-b
arduino_nano_33_ble` flag) to correctly map Arduino pin names/numbers to the
target board. To add board support:

1. This project is structured in a way so as to isolate the variants from the core API. Thus, whenever a new board
needs to be added it needs to be done in the `variants/` folder.
Add a folder inside of the variants folder that matches the name of your board.
2. Add an overlay file and a pinmap header file that match the name of the board.
3. Add your new headerfile to an `#ifdef` statement in the variants.h file.

An example of this structure is shown below.

```tree
variants/
├── arduino_nano_33_ble
│   ├── arduino_nano_33_ble.overlay
│   └── arduino_nano_33_ble_pinmap.h
├── CMakeLists.txt
└── variants.h

```

- The top level consists of `CMakeLists.txt`, `variants.h` and the `<BOARD_NAME>` folder. Each of these files have a specific role to play.
		- The `Cmakelists` help the compiler locate the proper directory to help find the proper header files that are board specific. You need to add the name using `zephyr_include_directories(BOARD_NAME)` to this file. Do note that this `BOARD_NAME` is the same as the name of your board's directory.
		- `variants.h` contains the necessary `#includes` inorder to tell the source code about your board's pinmap.
- The `<BOARD_NAME>` folder is where the overlay and pinmap file resides. Inorder to understand how to write DT overlays, lookup `Documentation/overlays.md`. To understand the `<boardname_pinmap.h>` file, go through the existing `variants/ARDUINO_NANO33BLE/arduino_nano_ble_sense_pinmap.h` which shows how to use the overlay nodes inside our C programs using zephyr macros like `GPIO_DT_SPEC_GET`. The zephyr-project documentation on this is pretty extensive as well and worth reading.

## Guide to Writing Overlays

### DeviceTree Overlay files for Arduino boards

This module requires that your Arduino header pins be mapped in the DeviceTree
to 'digital-pin-gpios' array that child of a `zephyr,user` node. Follow the
examples in the variants directory to create an overlay file and a header file
for your board.

### Overlays using previously-defined Arduino headers

When an Arduino header exists in a board's in-tree DTS file it can easily be
used to create the necessary overlay file. Assign the relevant mapping using the
Arduino header label (usually either `&arduino_header` or `&arduino_nano_header`
and the `gpio_map` number. The second number is used to add GPIO flags and may
safely be left as zero.

For example, creating an overlay file for the Nordic nRF52840 Development Kit
uses [the Arduino header definitions](https://github.com/zephyrproject-rtos/zephyr/blob/6f8ee2cdf7dd4d746de58909204ea0ce156d5bb4/boards/arm/nrf52840dk_nrf52840/nrf52840dk_nrf52840.dts#L74-L101), beginning with the first digital pin:

```
/ {
	zephyr,user {
		digital-pin-gpios = <&arduino_header 6 0>,	/* Digital */
				    <&arduino_header 7 0>;
				    ...
				    <&arduino_header 19 0>;
				    <&arduino_header 0 0>;	/* Analog */
				    <&arduino_header 1 0>;
				    ...
				    <&arduino_header 5 0>;
				    <&arduino_header 20 0>;	/* SDA */
				    <&arduino_header 21 0>;	/* SCL */
				    <&gpio0 13 GPIO_ACTIVE_LOW>;	/* LED0 */
		};
	};
};
```

### Configure Serial devices

The `serials` node defines the Serial devices to use.
It instantiate the `Serial` with the UART device that contained in the node.
Also instantiate as `Serial1`, `Serial2`, .. `SerialN` with the devices that is
after the second in the case of the array contains plural devices.

If the `serials` node is not defined, Use the node labeled `arduino-serial`.
Boards with Arduino-shield style connectors usually label `arduino-serial` for
UART port exposed in header or frequently used UART port.

If even 'arduino_serial' does not define, it uses the stub implementation
that redirects to printk().

The following example instantiates `Serial` and `Serial1` with each `uart0` and `uart1`.

```
/ {
       zephyr,user {
               serials = <&uart0, &uart1>;
       };
};
```

### Configure I2C devices

The `i2cs` node defines the I2C devices to use.
It instantiate the `Wire` with the i2c device that contained in the node.
Also instantiate as `Wire1`, `Wire2`, .. `WireN` with the devices
that is after the second in the case of the array contains plural devices.

If the `i2cs` node is not defined, Use the node labeled `arduino-i2c`.
Boards with Arduino-shield style connectors usually label `arduino-i2c`
to i2c exposed in the connector.

The following example instantiates `Wire` and `Wire2` with each `i2c0` and `i2c1`.

```
/ {
       zephyr,user {
               i2cs = <&i2c0, &i2c1>;
       };
};
```

### Overlays from scratch

You can see in the example above that there is no mapping for `LED0` in the
board's Arduino header definition so it has been added using the Zephyr `gpios`
syntax (port, pin, flags). When creating an overlay file that doesn't have an
Arduino header defined, you should follow this syntax for adding all pins

### You control the pin mapping

Zephyr [chooses to map Arduino headers beginning with the Analog
pins](https://docs.zephyrproject.org/latest/build/dts/api/bindings/gpio/arduino-header-r3.html),
but the overlay file example above begins with the digital pins. This is to
match user
expectation that issuing `pinMode(0,OUTPUT);` should control digital pin 0 (and
not pin 6). In the same way, the Analog 0 pin was mapped to D14 as this is
likely what a shield made for the Arduino Uno R3 header would expect.

Ultimately the mapping is completely up to you and should match the needs of the
sketch you will be compiling.

## Creating the pinmap header file

It is recommended that you copy an existing pinmap file from one of the board
folders inside of the variants folder. For the most part, this header file will
not change from board to board.

One example of a change that you may find useful is mapping additional pins. For
example, the LEDs on the nRF52840 are not connected to any of the Arduino header
pins. To define a built-in LED for this board, a 22nd pin definition was added.

Your pinmap header file must be added to the variants.h file by adding three
lines using this format:

```c
#ifdef CONFIG_BOARD_NRF52840DK_NRF52840
#include "nrf52840dk_nrf52840.h"
#endif // CONFIG_BOARD_NRF52840DK_NRF52840
```
