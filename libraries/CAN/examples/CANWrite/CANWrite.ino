/*
 * CANWrite example — transmit a CAN frame once per second.
 * Use ENABLE_CANFD define to switch between CAN and CAN FD modes (CAN FD mode requires CAN controller with FD support).
 *
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * SPDX-License-Identifier: Apache-2.0
 */

#include <CAN.h>

//#define ENABLE_CANFD

static uint32_t const CAN_ID = 0x123;
bool led = false;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

#ifdef ENABLE_CANFD
  if (!CAN.beginFD(CanBitRate::BR_500k, 1000000)) {
    Serial.println("CAN.beginFD(...) failed.");
    for (;;) {}
  }
#else
  if (!CAN.begin(CanBitRate::BR_500k)) {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }
#endif
 pinMode(LED_BUILTIN, OUTPUT);
}

static uint8_t msg_cnt = 0;

void loop() {
#ifdef ENABLE_CANFD
  uint8_t msg_data[] = {0xCA, 0xFE, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0};
#else
  uint8_t msg_data[] = {0xCA, 0xFE, 0, 0, 0, 0, 0, 0};
#endif

  digitalWrite(LED_BUILTIN, led);
  led = !led;

  memcpy((void *)(msg_data + 2), &msg_cnt, sizeof(msg_cnt));
#ifdef ENABLE_CANFD
  CanFDMsg msg(CanStandardId(CAN_ID), sizeof(msg_data), msg_data);
  if (int const rc = CAN.writeFD(msg); rc <= 0) {
#else
  CanMsg msg(CanStandardId(CAN_ID), sizeof(msg_data), msg_data);
  if (int const rc = CAN.write(msg); rc <= 0) {
#endif
    Serial.print("CAN.write(...) failed with error code ");
    Serial.println(rc);
    digitalWrite(LED_BUILTIN, HIGH);
    for (;;) {}
  }

  if (msg_cnt < 0xFE)
    msg_cnt++;
  else
    msg_cnt = 0;
  delay(1000);
}
