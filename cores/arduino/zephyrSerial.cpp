#include <zephyrSerial.h>
#include <Arduino.h>
#include <zephyr/sys/printk.h>

size_t arduino::ZephyrSerial::begin(unsigned long int baudrate){
    return 0;
}

size_t arduino::ZephyrSerial::print(char ch){
    printk("%c",ch);
    return 0;
}
size_t arduino::ZephyrSerial::println(const char* ptr) {
    printf("\n%s\n", ptr);
    return 0;
}
size_t arduino::ZephyrSerial::println(char c){
    // pvt_c = c;
    printf("\n%c\n",c);
    return 0;
}

size_t arduino::ZephyrSerial::println(void){
    printf("\n");
    return 0;
}

arduino::ZephyrSerial Serial;