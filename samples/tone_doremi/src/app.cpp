/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2026 TOKITA Hiroshi
 */

#include <Arduino.h>
#include "zephyrSerial.h"

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Do@D4");
  tone(D4, NOTE_C4, 1000);
  delay(2000);
  Serial.println("Re@D4");
  tone(D4, NOTE_D4, 1000);
  delay(2000);
  Serial.println("Mi@D4 - Infinity");
  tone(D4, NOTE_E4, 0);
  delay(2000);
  Serial.println("Fa@D5");
  tone(D5, NOTE_F4, 1000);
  delay(2000);
  Serial.println("So@D5");
  tone(D5, NOTE_G4, 1000);
  delay(2000);
  Serial.println("La@D5 - Infinity");
  tone(D5, NOTE_A4, 0);

  while(true) {
    delay(1000);
  }
}

