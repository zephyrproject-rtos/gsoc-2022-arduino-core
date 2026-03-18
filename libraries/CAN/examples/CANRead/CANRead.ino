/*
 * CANRead example — receive CAN frames and print them to Serial.
 * Use ENABLE_CANFD define to switch between CAN and CAN FD modes (CAN FD mode requires CAN controller with FD support).
 * 
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * SPDX-License-Identifier: Apache-2.0
 */

#include <CAN.h>

//#define ENABLE_CANFD

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

void loop() {
  if (CAN.available()) {
#ifdef ENABLE_CANFD
    CanFDMsg const msg = CAN.readFD();
#else
    CanMsg const msg = CAN.read();
#endif
    Serial.println(msg);
    digitalWrite(LED_BUILTIN, led);
    led = !led;
  }
}
