/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>

namespace {

#ifdef CONFIG_PWM

#define PWM_DT_SPEC(n,p,i) PWM_DT_SPEC_GET_BY_IDX(n, i),
#define PWM_PINS(n,p,i) DT_PROP_BY_IDX(n, p, i),

const struct pwm_dt_spec arduino_pwm[] =
	{ DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), pwms, PWM_DT_SPEC) };

/* pwm-pins node provides a mapping digital pin numbers to pwm channels */
const pin_size_t arduino_pwm_pins[] =
	{ DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), pwm_pins, PWM_PINS) };

size_t pwm_pin_index(pin_size_t pinNumber) {
  for(size_t i=0; i<ARRAY_SIZE(arduino_pwm_pins); i++) {
    if (arduino_pwm_pins[i] == pinNumber) {
      return i;
    }
  }
  return (size_t)-1;
}

#endif //CONFIG_PWM

#ifdef CONFIG_ADC

#define ADC_DT_SPEC(n,p,i) ADC_DT_SPEC_GET_BY_IDX(n, i),
#define ADC_PINS(n,p,i) DT_PROP_BY_IDX(n, p, i),
#define ADC_CH_CFG(n,p,i) arduino_adc[i].channel_cfg,

const struct adc_dt_spec arduino_adc[] =
  { DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, ADC_DT_SPEC) };

/* io-channel-pins node provides a mapping digital pin numbers to adc channels */
const pin_size_t arduino_analog_pins[] =
  { DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channel_pins, ADC_PINS) };

struct adc_channel_cfg channel_cfg[ARRAY_SIZE(arduino_analog_pins)] =
  { DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, ADC_CH_CFG) };

size_t analog_pin_index(pin_size_t pinNumber) {
  for(size_t i=0; i<ARRAY_SIZE(arduino_analog_pins); i++) {
    if (arduino_analog_pins[i] == pinNumber) {
      return i;
    }
  }
  return (size_t)-1;
}

#endif //CONFIG_ADC

}

void yield(void) {
  k_yield();
}

/*
 *  The ACTIVE_HIGH flag is set so that A low physical
 *  level on the pin will be interpreted as value 0.
 *  A high physical level will be interpreted as value 1
 */
void pinMode(pin_size_t pinNumber, PinMode pinMode) {
  if (pinMode == INPUT) { // input mode
    gpio_pin_configure_dt(&arduino_pins[pinNumber],
                          GPIO_INPUT | GPIO_ACTIVE_HIGH);
  } else if (pinMode == INPUT_PULLUP) { // input with internal pull-up
    gpio_pin_configure_dt(&arduino_pins[pinNumber],
                          GPIO_INPUT | GPIO_PULL_UP | GPIO_ACTIVE_HIGH);
  } else if (pinMode == INPUT_PULLDOWN) { // input with internal pull-down
    gpio_pin_configure_dt(&arduino_pins[pinNumber],
                          GPIO_INPUT | GPIO_PULL_DOWN | GPIO_ACTIVE_HIGH);
  } else if (pinMode == OUTPUT) { // output mode
    gpio_pin_configure_dt(&arduino_pins[pinNumber],
                          GPIO_OUTPUT_LOW | GPIO_ACTIVE_HIGH);
  }
}

void digitalWrite(pin_size_t pinNumber, PinStatus status) {
  gpio_pin_set_dt(&arduino_pins[pinNumber], status);
}

PinStatus digitalRead(pin_size_t pinNumber) {
  return (gpio_pin_get_dt(&arduino_pins[pinNumber]) == 1) ? HIGH : LOW;
}

void delay(unsigned long ms) { k_sleep(K_MSEC(ms)); }

void delayMicroseconds(unsigned int us) { k_sleep(K_USEC(us)); }

unsigned long micros(void) {
  return k_cyc_to_us_floor32(k_cycle_get_32());
}

unsigned long millis(void) { return k_uptime_get_32(); }

#ifdef CONFIG_PWM

void analogWrite(pin_size_t pinNumber, int value)
{
  size_t idx = pwm_pin_index(pinNumber);

  if (idx >= ARRAY_SIZE(arduino_pwm) ) {
    return;
  }

  if (((uint32_t)value) > arduino_pwm[idx].period) {
    value = arduino_pwm[idx].period;
  } else if (value < 0) {
    value = 0;
  }

  /*
   * A duty ratio determines by the period value defined in dts
   * and the value arguments. So usually the period value sets as 255.
   */
  (void)pwm_set_cycles(arduino_pwm[idx].dev, arduino_pwm[idx].channel,
		       arduino_pwm[idx].period, value, arduino_pwm[idx].flags);
}

#endif

#ifdef CONFIG_ADC

void analogReference(uint8_t mode)
{
  /*
   * The Arduino API not clearly defined what means of
   * the mode argument of analogReference().
   * Treat the value as equivalent to zephyr's adc_reference.
   */
  size_t idx;
  for (size_t i=0; i<ARRAY_SIZE(channel_cfg); i++) {
    channel_cfg[i].reference = static_cast<adc_reference>(mode);
  }
}

int analogRead(pin_size_t pinNumber)
{
  int err;
  int16_t buf;
  struct adc_sequence seq = { .buffer = &buf, .buffer_size = sizeof(buf) };
  size_t idx = analog_pin_index(pinNumber);

  if (idx >= ARRAY_SIZE(arduino_adc) ) {
    return -EINVAL;
  }

  /*
   * ADC that is on MCU supported by Zephyr exists
   * only 16bit resolution, currently.
   */
  if (arduino_adc[idx].resolution > 16) {
    return -ENOTSUP;
  }

  err = adc_channel_setup(arduino_adc[idx].dev, &arduino_adc[idx].channel_cfg);
  if (err < 0) {
    return err;
  }

  seq.channels = BIT(arduino_adc[idx].channel_id);
  seq.resolution = arduino_adc[idx].resolution;
  seq.oversampling = arduino_adc[idx].oversampling;

  err = adc_read(arduino_adc[idx].dev, &seq);
  if (err < 0) {
    return err;
  }

  return buf;
}

#endif
