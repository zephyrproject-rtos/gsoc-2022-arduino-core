# Using external Arduino Libraries

A sample has been provided here using [ADXL345: ArduinoCore-Demo](https://github.com/DhruvaG2000/ArduinoCore-Demo).

Let's assume the structure of your application for simplicity to be,

```tree
├── blinky_arduino
│   ├── CMakeLists.txt
│   ├── prj.conf
│   ├── README.rst
│   └── src
│       └── main.cpp
```


## Update contents of src folder
Paste your library's source files like ``ADXL345.h`` and ``ADXL345.cpp`` in your project's src folder. 

## Update CMakeLists
As we can see, there is a Top-level CMakelists which needs to be updated with your external library's source files.
For example, we add `target_sources(app PRIVATE src/ADXL345.cpp)`  to CMakeLists.txt.

## Update main.cpp
Finally, paste your required code using the library in ``main.c`` and include that library's header using
```c
#include "ADXL345.h"
```
