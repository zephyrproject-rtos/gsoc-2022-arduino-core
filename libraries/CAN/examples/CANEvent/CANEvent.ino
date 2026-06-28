/*
 * CANEvent example — event-driven callback with LED feedback.
 *
 * Demonstrates:
 * - Registering a callback for CAN message reception
 * - Callback executed in worker thread by the CAN library (not ISR context)
 * - Filtering on specific CAN ID
 * - Non-blocking LED control via delayed work queue
 * 
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * SPDX-License-Identifier: Apache-2.0
 */

#include <CAN.h>

static uint32_t const CAN_ID_RX = 0x123;
static struct k_work_delayable led_off_work;

static void led_off_handler(struct k_work *work) {
  (void)work;
  digitalWrite(LED_BUILTIN, LOW);
}

void onCanMessage(CanFDMsg const &msg, void *user_data) {
  (void)user_data;

  /* Turn LED on to indicate message received. */
  digitalWrite(LED_BUILTIN, HIGH);

  /* Print received message to Serial. */
  Serial.print("RX: ");
  Serial.println(msg);

  /* Schedule LED off after 100ms using work queue (non-blocking). */
  k_work_reschedule_for_queue(&k_sys_work_q, &led_off_work, K_MSEC(100));
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  k_work_init_delayable(&led_off_work, led_off_handler);

  /* Configure LED pin. */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  /* Configure filter: only CAN_ID_RX (0x123), standard 11-bit. */
  int filter_id = CAN.addReceiveFilter(CAN_ID_RX, 0x7FF, false);
  if (filter_id < 0) {
    Serial.print("addReceiveFilter() failed: ");
    Serial.println(filter_id);
    for (;;) {}
  }

  /* Initialize CAN bus. */
  if (!CAN.begin(CanBitRate::BR_500k)) {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }

  /* Register callback for CAN reception. */
  CAN.onReceive(onCanMessage);

  Serial.println("CANEvent example ready. Waiting for messages on ID 0x123 ...");
}

void loop() {
  /* Everything happens via CAN callback.
   * Loop is intentionally empty (event-driven pattern). */
}
