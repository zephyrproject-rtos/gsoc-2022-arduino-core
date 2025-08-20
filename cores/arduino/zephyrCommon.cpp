/*
 * Copyright (c) 2022 Dhruva Gole
 * Copyright (c) 2025 TOKITA Hiroshi
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include "zephyrInternal.h"

namespace {

#define DEVICE_GPIO(n, p, i) DEVICE_DT_GET(DT_PROP_BY_IDX(n, p, i))
constexpr const struct device *gpios[] = {
  DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), gpio_ports, DEVICE_GPIO, (,))
};

#define PROP_NGPIOS(n, p, i) DT_PROP(DT_PROP_BY_IDX(n, p, i), ngpios)
constexpr uint32_t pins[] = {
  DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), gpio_ports, PROP_NGPIOS, (,))
};

constexpr inline const struct device *local_gpio_port_r(pin_size_t pin,
                                                        const struct device *const *ctrl,
                                                        const uint32_t accum,
							const uint32_t *end, size_t n) {
  return (n == 0)                 ? nullptr
         : (pin < accum + end[0]) ? ctrl[0]
                                  : local_gpio_port_r(pin, ctrl + 1, accum + end[0], end + 1, n - 1);
}

constexpr inline size_t port_index_r(const struct device *target,
                                     const struct device *const *table, pin_size_t idx, size_t n) {
  return (n == 0)               ? size_t(-1)
         : (target == table[0]) ? idx
                                : port_index_r(target, table + 1, idx + 1, n - 1);
}

constexpr inline const struct device *local_gpio_port(pin_size_t gpin) {
  return local_gpio_port_r(gpin, gpios, 0, pins, ARRAY_SIZE(gpios));
}

constexpr inline pin_size_t port_idx(pin_size_t gpin) {
  return port_index_r(local_gpio_port(gpin), gpios, 0, ARRAY_SIZE(gpios));
}

constexpr inline pin_size_t end_accum_r(const uint32_t accum, const uint32_t *end, size_t n) {
  return (n == 0) ? accum : end_accum_r(accum + end[0], end + 1, n - 1);
}

constexpr inline pin_size_t end_accum(size_t n) {
  return end_accum_r(0, pins, n);
}

constexpr inline pin_size_t local_gpio_pin(pin_size_t gpin) {
  return port_idx(gpin) == pin_size_t(-1) ? pin_size_t(-1) : gpin - end_accum(port_idx(gpin));
}

constexpr inline pin_size_t global_gpio_pin_(size_t port_idx, pin_size_t lpin) {
  return port_idx == size_t(-1) ? size_t(-1) : end_accum(port_idx) + lpin;
}

constexpr inline pin_size_t global_gpio_pin(const struct device *lport, pin_size_t lpin) {
  return global_gpio_pin_(port_index_r(lport, gpios, 0, ARRAY_SIZE(gpios)), lpin);
}

inline int global_gpio_pin_configure(pin_size_t pinNumber, int flags) {
  return gpio_pin_configure(local_gpio_port(pinNumber), local_gpio_pin(pinNumber), flags);
}

template <class N, class Head> constexpr const N max_in_list(const N max, const Head &head)
{
  return (max >= head) ? max : head;
}

template <class N, class Head, class... Tail>
constexpr const N max_in_list(const N max, const Head &head, const Tail &...tail)
{
  return max_in_list((max >= head) ? max : head, tail...);
}

#define GPIO_NGPIOS(n, p, i) DT_PROP(DT_GPIO_CTLR_BY_IDX(n, p, i), ngpios)
constexpr int max_ngpios = max_in_list(
  0, DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), digital_pin_gpios, GPIO_NGPIOS, (, )));

/*
 * GPIO callback implementation
 */

struct gpio_port_callback {
  struct gpio_callback callback;
  voidFuncPtr handlers[max_ngpios];
} port_callback[ARRAY_SIZE(gpios)];

struct gpio_port_callback *find_gpio_port_callback(const struct device *dev)
{
  for (size_t i = 0; i < ARRAY_SIZE(gpios); i++) {
    if (dev == gpios[i]) {
      return &port_callback[i];
    }
  }

  return nullptr;
}

void set_interrupt_handler(pin_size_t pinNumber, voidFuncPtr func) {
  struct gpio_port_callback *pcb = find_gpio_port_callback(local_gpio_port(pinNumber));

  if (pcb) {
    pcb->handlers[local_gpio_pin(pinNumber)] = func;
  }
}

void handle_gpio_callback(const struct device *port, struct gpio_callback *cb, uint32_t pins) {
  struct gpio_port_callback *pcb = CONTAINER_OF(cb, struct gpio_port_callback, callback);

  for (uint32_t i = 0; i < max_ngpios; i++) {
    if (pins & BIT(i) && pcb->handlers[i]) {
      pcb->handlers[i]();
    }
  }
}

#ifdef CONFIG_PWM

#define PWM_DT_SPEC(n, p, i) PWM_DT_SPEC_GET_BY_IDX(n, i)

const struct pwm_dt_spec arduino_pwm[] = {
  DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), pwms, PWM_DT_SPEC, (,))
};

/* pwm-pins node provides a mapping digital pin numbers to pwm channels */
const pin_size_t arduino_pwm_pins[] = {
  DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), pwm_pin_gpios, ZARD_GLOBAL_GPIO_NUM, (,))
};

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

#define ADC_DT_SPEC(n, p, i) ADC_DT_SPEC_GET_BY_IDX(n, i)
#define ADC_CH_CFG(n, p, i) arduino_adc[i].channel_cfg

const struct adc_dt_spec arduino_adc[] = {
  DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), io_channels, ADC_DT_SPEC, (,))
};

/* io-channel-pins node provides a mapping digital pin numbers to adc channels */
const pin_size_t arduino_analog_pins[] = {
  DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), adc_pin_gpios, ZARD_GLOBAL_GPIO_NUM, (,))
};

struct adc_channel_cfg channel_cfg[] = {
  DT_FOREACH_PROP_ELEM_SEP(DT_PATH(zephyr_user), io_channels, ADC_CH_CFG, (,))
};

size_t analog_pin_index(pin_size_t pinNumber) {
  for(size_t i=0; i<ARRAY_SIZE(arduino_analog_pins); i++) {
    if (arduino_analog_pins[i] == pinNumber) {
      return i;
    }
  }
  return (size_t)-1;
}

#endif //CONFIG_ADC

static unsigned int irq_key;
static bool interrupts_disabled = false;
}  // namespace

void yield(void) {
  k_yield();
}

/*
 *  The ACTIVE_HIGH flag is set so that A low physical
 *  level on the pin will be interpreted as value 0.
 *  A high physical level will be interpreted as value 1
 */
void pinMode(pin_size_t pinNumber, PinMode pinMode) {
  if (pinMode == INPUT) {
    global_gpio_pin_configure(pinNumber, GPIO_INPUT | GPIO_ACTIVE_HIGH);
  } else if (pinMode == INPUT_PULLUP) {
    global_gpio_pin_configure(pinNumber, GPIO_INPUT | GPIO_PULL_UP | GPIO_ACTIVE_HIGH);
  } else if (pinMode == INPUT_PULLDOWN) {
    global_gpio_pin_configure(pinNumber, GPIO_INPUT | GPIO_PULL_DOWN | GPIO_ACTIVE_HIGH);
  } else if (pinMode == OUTPUT) {
    global_gpio_pin_configure(pinNumber, GPIO_OUTPUT_LOW | GPIO_ACTIVE_HIGH);
  }
}

void digitalWrite(pin_size_t pinNumber, PinStatus status) {
  gpio_pin_set(local_gpio_port(pinNumber), local_gpio_pin(pinNumber), status);
}

PinStatus digitalRead(pin_size_t pinNumber) {
  return (gpio_pin_get(local_gpio_port(pinNumber), local_gpio_pin(pinNumber)) == 1) ? HIGH : LOW;
}

#ifndef MAX_TONE_PINS
#define MAX_TONE_PINS DT_PROP_LEN(DT_PATH(zephyr_user), digital_pin_gpios)
#endif

#define TOGGLES_PER_CYCLE 2ULL

static struct pin_timer {
  struct k_timer timer;
  uint32_t count;
  pin_size_t pin;
  bool infinity;
} arduino_pin_timers[MAX_TONE_PINS];

void tone_expiry_cb(struct k_timer *timer) {
  struct pin_timer *pt = CONTAINER_OF(timer, struct pin_timer, timer);

  if (pt->count == 0) {
    k_timer_stop(timer);
    gpio_pin_set(local_gpio_port(pt->pin), local_gpio_pin(pt->pin), 0);
  } else {
    gpio_pin_toggle(local_gpio_port(pt->pin), local_gpio_pin(pt->pin));
    if (!pt->infinity) {
      pt->count--;
    }
  }
}

void tone(pin_size_t pinNumber, unsigned int frequency, unsigned long duration) {
  struct k_timer *timer;
  k_timeout_t timeout;

  if (pinNumber >= MAX_TONE_PINS) {
    return;
  }

  timer = &arduino_pin_timers[pinNumber].timer;

  pinMode(pinNumber, OUTPUT);
  k_timer_stop(&arduino_pin_timers[pinNumber].timer);

  if (frequency == 0) {
    gpio_pin_set(local_gpio_port(pinNumber), local_gpio_pin(pinNumber), 0);
    return;
  }

  timeout = K_NSEC(NSEC_PER_SEC / (TOGGLES_PER_CYCLE * frequency));
  if (timeout.ticks == 0) {
    timeout.ticks = 1;
  }

  arduino_pin_timers[pinNumber].infinity = (duration == 0);
  arduino_pin_timers[pinNumber].count = (uint64_t)duration * frequency *
				        (MSEC_PER_SEC / TOGGLES_PER_CYCLE);
  arduino_pin_timers[pinNumber].pin = pinNumber;
  k_timer_init(timer, tone_expiry_cb, NULL);

  gpio_pin_set(local_gpio_port(pinNumber), local_gpio_pin(pinNumber), 0);
  k_timer_start(timer, timeout, timeout);
}

void noTone(pin_size_t pinNumber) {
  k_timer_stop(&arduino_pin_timers[pinNumber].timer);
  gpio_pin_set(local_gpio_port(pinNumber), local_gpio_pin(pinNumber), 0);
}

void delay(unsigned long ms) {
  k_sleep(K_MSEC(ms));
}

void delayMicroseconds(unsigned int us) {
  k_busy_wait(us);
}

unsigned long micros(void) {
  return k_cyc_to_us_floor32(k_cycle_get_32());
}

unsigned long millis(void) {
  return k_uptime_get_32();
}

#ifdef CONFIG_PWM

void analogWrite(pin_size_t pinNumber, int value)
{
  size_t idx = pwm_pin_index(pinNumber);

  if (idx >= ARRAY_SIZE(arduino_pwm)) {
    return;
  }

  if (!pwm_is_ready_dt(&arduino_pwm[idx])) {
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
  (void)pwm_set_pulse_dt(&arduino_pwm[idx], value);
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

void attachInterrupt(pin_size_t pinNumber, voidFuncPtr callback, PinStatus pinStatus)
{
  struct gpio_port_callback *pcb;
  gpio_flags_t intmode = 0;

  if (!callback) {
    return;
  }

  if (pinStatus == LOW) {
    intmode |= GPIO_INT_LEVEL_LOW;
  } else if (pinStatus == HIGH) {
    intmode |= GPIO_INT_LEVEL_HIGH;
  } else if (pinStatus == CHANGE) {
    intmode |= GPIO_INT_EDGE_BOTH;
  } else if (pinStatus == FALLING) {
    intmode |= GPIO_INT_EDGE_FALLING;
  } else if (pinStatus == RISING) {
    intmode |= GPIO_INT_EDGE_RISING;
  } else {
    return;
  }

  pcb = find_gpio_port_callback(local_gpio_port(pinNumber));
  __ASSERT(pcb != nullptr, "gpio_port_callback not found");

  set_interrupt_handler(pinNumber, callback);

  if (pcb->callback.handler == NULL) {
    gpio_init_callback(&pcb->callback, handle_gpio_callback, 0);
    gpio_add_callback(local_gpio_port(pinNumber), &pcb->callback);
  }

  enableInterrupt(pinNumber);
  gpio_pin_interrupt_configure(local_gpio_port(pinNumber), local_gpio_pin(pinNumber), intmode);
}

void detachInterrupt(pin_size_t pinNumber)
{
  gpio_pin_interrupt_configure(local_gpio_port(pinNumber), local_gpio_pin(pinNumber), 0);
  disableInterrupt(pinNumber);
  set_interrupt_handler(pinNumber, nullptr);
}

#ifndef CONFIG_MINIMAL_LIBC_RAND

#include <stdlib.h>

void randomSeed(unsigned long seed) {
	srand(seed);
}

long random(long min, long max) {
	return rand() % (max - min) + min;
}

long random(long max) {
	return rand() % max;
}

#endif

unsigned long pulseIn(pin_size_t pinNumber, uint8_t state, unsigned long timeout) {
  const struct device* port = local_gpio_port(pinNumber);
  const size_t pin = local_gpio_pin(pinNumber);
  struct k_timer timer;
  int64_t start, end, delta = 0;

  if (!device_is_ready(port)) {
    return 0;
  }

  k_timer_init(&timer, NULL, NULL);
  k_timer_start(&timer, K_MSEC(timeout), K_NO_WAIT);

  while (gpio_pin_get(port, pin) == state && k_timer_status_get(&timer) == 0);
  if (k_timer_status_get(&timer) > 0) {
    goto cleanup;
  }

  while (gpio_pin_get(port, pin) != state && k_timer_status_get(&timer) == 0);
  if (k_timer_status_get(&timer) > 0) {
    goto cleanup;
  }

  start = k_uptime_ticks();
  while (gpio_pin_get(port, pin) == state && k_timer_status_get(&timer) == 0);
  if (k_timer_status_get(&timer) > 0) {
    goto cleanup;
  }
  end = k_uptime_ticks();

  delta = k_ticks_to_us_floor64(end - start);

cleanup:
  k_timer_stop(&timer);
  return (unsigned long)delta;
}

void enableInterrupt(pin_size_t pinNumber) {
  struct gpio_port_callback *pcb = find_gpio_port_callback(local_gpio_port(pinNumber));

  if (pcb) {
    pcb->callback.pin_mask |= BIT(local_gpio_pin(pinNumber));
  }
}

void disableInterrupt(pin_size_t pinNumber) {
  struct gpio_port_callback *pcb = find_gpio_port_callback(local_gpio_port(pinNumber));

  if (pcb) {
    pcb->callback.pin_mask &= ~BIT(local_gpio_pin(pinNumber));
  }
}

void interrupts(void) {
  if (interrupts_disabled) {
    irq_unlock(irq_key);
    interrupts_disabled = false;
  }
}

void noInterrupts(void) {
  if (!interrupts_disabled) {
    irq_key = irq_lock();
    interrupts_disabled = true;
  }
}

int digitalPinToInterrupt(pin_size_t pin) {
  struct gpio_port_callback *pcb =
      find_gpio_port_callback(local_gpio_port(pin));

  return (pcb) ? pin : -1;
}
