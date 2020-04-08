#include <Arduino.h>
#include <Ctrl.h>

#define PIN_A 2
#define PIN_B 3
#define ENC_STABLE_STATE 0x03

void handleRotate(ctrl::Encoder_Event const *);

ctrl::Encoder enc(ENC_STABLE_STATE, PIN_A, PIN_B);

void setup() {
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("Starting...");

  enc.on("rotate", handleRotate);
}

void loop() { enc.listen(); }

void handleRotate(ctrl::Encoder_Event const *event) {
  static uint16_t counter = 0;

  if (event->positive_tick) {
    Serial.println("Counter: " + String(++counter));
  }

  if (event->negative_tick && counter > 0) {
    Serial.println("Counter: " + String(--counter));
  }
}
