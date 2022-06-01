#include "Common.h"

/* C++ prototypes */
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t makeWord(uint16_t w) { return w; }
uint16_t makeWord(uint8_t h, uint8_t l) { return (h << 8) | l; }

void delay(unsigned long ms)
{
  k_sleep(K_MSEC(ms));
}

void delayMicroseconds(unsigned int us)
{
  k_sleep(K_USEC(us));
}