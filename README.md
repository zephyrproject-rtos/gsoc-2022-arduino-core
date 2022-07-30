# GSoC 2022 Project

![](https://dhruvag2000.github.io/Blog-GSoC22/assets/images/website_header.png)

## Arduino Core API module for zephyr

Arduino Core API module for zephyr leverages the power of Zephyr under an Arduino-C++ style abtraction layer thus helping zephyr new-comers to start using it without worrying about learning about new APIs and libraries. Visit this project's [official blog](https://dhruvag2000.github.io/Blog-GSoC22/) for documentation.

## Adding Arduino Core API to Zephyr

* **Pre requisites:** It is assumed that you have zephyrproject configured and installed on your system as per the official [Get Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html). The recommended path to install is `~/zephyrproject` as specified in the guide. You may face path issues if you have zephyr installed in a custom path.

* Add following entry to `west.yml` file in `manifest/projects` subtree of Zephyr:
```
# Arduino API repository.
- name: Arduino-Core-Zephyr
  path: modules/lib/Arduino-Zephyr-API
  revision: dev
  url: https://github.com/zephyrproject-rtos/gsoc-2022-arduino-core
```

* Then, clone the repository by running

```sh
west update
```

* To "complete" the core you need to copy or symlink the api folder from [this repo](https://github.com/arduino/ArduinoCore-API.git) to the target's ``cores/arduino`` folder:
```sh
$ git clone git@github.com:arduino/ArduinoCore-API # Any location
$ ln -s /<your>/<location>/ArduinoCore-API/api cores/arduino/.
```

**Known Bug(s):**
* While compiling with the ArduinoCore-API `WCharacter.h` produces many errors. It needs to be deleted from ArduinoAPI.h (it is unused in this port.) We are looking into resolving the issue.

**Maintainers**:
- [beriberikix](https://github.com/beriberikix)
- [szczys](https://github.com/szczys) 
- [alvarowolfx](https://github.com/alvarowolfx)

**Contributor**: [DhruvaG2000](https://github.com/DhruvaG2000)

## License
Please note that the current license is Apache 2. Previously it was LGPL 2.1 but after careful review it was determined that no LGPL code or derivates was used and the more permissive license was chosen.
