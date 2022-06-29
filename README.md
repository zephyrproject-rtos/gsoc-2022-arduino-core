# GSoC 2022 Project

![](https://dhruvag2000.github.io/Blog-GSoC22/assets/images/website_header.png)

## Arduino Core API module for zephyr

Arduino Core API module for zephyr leverages the power of Zephyr under an Arduino-C++ style abtraction layer thus helping zephyr new-comers to start using it without worrying about learning about new APIs and libraries. Visit this project's [official blog](https://dhruvag2000.github.io/Blog-GSoC22/) for documentation.

To "complete" the core you need to copy or symlink the api folder from [this repo](https://github.com/arduino/ArduinoCore-API.git) to the target's ``cores/arduino`` folder:
```sh
$ git clone git@github.com:arduino/ArduinoCore-API # Any location
$ ln -s /<your>/<location>/ArduinoCore-API/api cores/arduino/.
```

Maintainers:
- beriberikix
- szczys 
- alvarowolfx

Contributor(s):
[DhruvaG2000](https://github.com/DhruvaG2000)

## License
Please note that the current license is Apache 2. Previously it was LGPL 2.1 but after careful review it was determined that no LGPL code or derivates was used and the more permissive license was chosen.