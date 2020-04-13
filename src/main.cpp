#include <Arduino.h>
#include <Ctrl.h>

#define ENC_LEFT_PIN 2
#define ENC_RIGHT_PIN 3
#define ENC_STABLE_STATE 0x03
#define BUTTON 4

void handle_rotate(ctrl::Encoder_Event const *);
void handle_keydown(ctrl::Button_Event const *);
void handle_keyup(ctrl::Button_Event const *);

ctrl::Encoder enc(ENC_STABLE_STATE, ENC_LEFT_PIN, ENC_RIGHT_PIN);
ctrl::Button btn(HIGH, BUTTON);

bool is_pressed = false;

void setup() {
  Serial.begin(115200);

  pinMode(ENC_LEFT_PIN, INPUT_PULLUP);
  pinMode(ENC_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BUTTON, INPUT_PULLUP);

  enc.on("rotate", handle_rotate);
  btn.on("keydown", handle_keydown);
  btn.on("keyup", handle_keyup);
}

void loop() {
  enc.listen();
  btn.listen();
}

void handle_rotate(ctrl::Encoder_Event const *event) {
  static uint16_t counter = 0;
  static uint16_t pressed_counter = 0;

  if (event->positive_tick) {
    if (is_pressed) {
      Serial.println("Pressed Counter: " + String(++pressed_counter));
    } else {
      Serial.println("Counter: " + String(++counter));
    }
  }

  if (event->negative_tick) {
    if (is_pressed) {
      if (pressed_counter > 0) {
        Serial.println("Pressed Counter: " + String(--pressed_counter));
      }
    } else {
      if (counter > 0) {
        Serial.println("Counter: " + String(--counter));
      }
    }
  }
}

void handle_keydown(ctrl::Button_Event const *) { is_pressed = true; }

void handle_keyup(ctrl::Button_Event const *) { is_pressed = false; }
