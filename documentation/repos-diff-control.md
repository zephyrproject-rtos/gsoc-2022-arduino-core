# Repository diff control

This file tracks how it handled the differences that arise when merging with
https://github.com/arduino/ArduinoCore-zephyr.

## Excluded because of license incompatibility.

* `libraries/Arduino_LED_Matrix/`
* `libraries/Camera/`
* `libraries/Ethernet/`
* `libraries/RTC/`
* `libraries/SocketWrapper/`
* `libraries/Storage/`
* `libraries/WiFi/`
* `libraries/Zephyr_SDRAM/`
* `libraries/ea_malloc/`
* `loader/blobs/`
* `tools/sync-zephyr-artifacts/`
* `boards.txt`
* `platform.txt`
* `post_install.sh`
* `programmers.txt`

## Excluded due to repository configuration differences

* `.github/ISSUE_TEMPLATE/compilation_bug_report.md`
* `.github/ISSUE_TEMPLATE/miscellaneous_bug_report.md`
* `.github/ISSUE_TEMPLATE/runtime_bug_report.md`
* `.github/workflows/commit_check.yml`
* `.github/workflows/leave_pr_comment.yml`
* `.github/workflows/package_core.yml`
* `.github/workflows/package_tool.yml`
* `.github/workflows/report_size_deltas.yml`
* `.github/workflows/scancode.yml`
* `.github/workflows/upload_json.yml`
* `.gitmodules`
* `cores/arduino/api`

## Keep the original for now because integration is not yet complete

- [ ] `.github/`
- [ ] `README.md`
- [ ] `Kconfig`
- [ ] `CMakeLists.txt`
- [ ] `cores/CMakeLists.txt`
- [ ] `cores/arduino/CMakeLists.txt`
- [ ] `cores/arduino/Arduino.h`
- [ ] `cores/arduino/main.cpp`
- [ ] `cores/arduino/zephyrPrint.cpp`
- [ ] `cores/arduino/zephyrPrint.h`
- [ ] `cores/arduino/zephyrSerial.cpp`
- [ ] `cores/arduino/zephyrSerial.h`
- [ ] `documentation/variants.md`
- [ ] `libraries/SPI`
- [ ] `libraries/Wire`
- [ ] `samples`
- [ ] `variants/default/variant.h`
- [ ] `west.yml`


### Bump to 0.55.0

- Add SPDX-Identifier
  - [x] `variants/arduino_nano_33_ble/arduino_nano_33_ble.overlay`
  - [x] `variants/arduino_nano_33_iot/arduino_nano_33_iot.overlay`
  - [x] `variants/nrf52840dk_nrf52840/nrf52840dk_nrf52840.overlay`
  - [x] `variants/nrf9160dk_nrf9160/nrf9160dk_nrf9160.overlay`

- Add CAN library
  - [x] `libraries/CMakeLists.txt`

