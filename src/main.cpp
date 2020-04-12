#include <Arduino.h>
#include <Ctrl.h>

#define ENC_LEFT_PIN 2
#define ENC_RIGHT_PIN 3
#define ENC_STABLE_STATE 0x03
#define BUTTON 4

void handle_rotate(ctrl::Encoder_Event const *);
void handle_click(ctrl::Button_Event const *);

ctrl::Encoder enc(ENC_STABLE_STATE, ENC_LEFT_PIN, ENC_RIGHT_PIN);
ctrl::Button btn(HIGH, BUTTON);

void setup() {
  Serial.begin(115200);

  pinMode(ENC_LEFT_PIN, INPUT_PULLUP);
  pinMode(ENC_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BUTTON, INPUT_PULLUP);

  enc.on("rotate", handle_rotate);
  btn.on("click", handle_click);
}

void loop() {
  enc.listen();
  btn.listen();
}

void handle_rotate(ctrl::Encoder_Event const *event) {
  static uint16_t counter = 0;

  if (event->positive_tick) {
    ++counter;
  }

  if (event->negative_tick && counter > 0) {
    --counter;
  }

  Serial.println("Counter: " + String(counter));
}

void handle_click(ctrl::Button_Event const *event) {
  static uint8_t counter = 0;

  Serial.println("Button Counter: " + String(++counter));
}
