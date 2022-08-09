#pragma once

#include <Arduino.h>
#include <api/Print.h>

namespace arduino {

class ZephyrSerial {
    char pvt_c;
public:
    size_t begin(unsigned long int baudrate);  //TODO

    size_t print(const char c);

    size_t println(const char c);
    size_t println(const char* ptr);
    size_t println(void);

};

}   // namespace arduino

extern arduino::ZephyrSerial Serial;