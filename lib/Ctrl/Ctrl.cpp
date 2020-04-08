#include "Ctrl.h"

using namespace ctrl;

Encoder::Encoder(uint8_t stable_state, uint8_t l_pin, uint8_t r_pin) {
  this->stable_state = stable_state;
  this->prev_unstable_state = stable_state;
  this->l_pin = l_pin;
  this->r_pin = r_pin;
  this->unstable_signal = 0;
  this->is_stable_state = true;
  this->is_new_unstable_state = false;
  this->tick = false;
  this->positive_tick = false;
  this->negative_tick = false;
}

void Encoder::listen(Encoder_Handler callback) {
  static Encoder_Event event;

  this->read();

  if (this->positive_tick) {
    event.positive_tick = true;
    event.negative_tick = false;

    callback(event);

    return;
  }

  if (this->negative_tick) {
    event.positive_tick = false;
    event.negative_tick = true;

    callback(event);

    return;
  }
}

void Encoder::on(char *event_name, Encoder_Handler callback) {
  // this->handlers.rotate = callback;
}

uint8_t Encoder::get_new_state() {
  uint8_t state = 0;

  state = digitalRead(this->l_pin);
  state <<= 1;
  state += digitalRead(this->r_pin);

  return state;
}

void Encoder::reset_stable_state() {
  if (this->is_stable_state) {
    this->is_stable_state = false;
  }
}

void Encoder::set_unstable_signal(uint8_t state) {
  if (!this->is_stable_state && state != this->prev_unstable_state) {
    this->prev_unstable_state = state;
    this->unstable_signal <<= 2;
    this->unstable_signal += state;
  }
}

void Encoder::reset_tick() {
  this->tick = false;
  this->prev_unstable_state = this->stable_state;
  this->unstable_signal = 0;
  this->positive_tick = false;
  this->negative_tick = false;
}

void Encoder::set_tick() {
  this->tick = true;
  this->is_stable_state = true;

  if (this->unstable_signal == 0x12) {
    this->negative_tick = false;
    this->positive_tick = true;
  }

  if (this->unstable_signal == 0x21) {
    this->positive_tick = false;
    this->negative_tick = true;
  }
}

bool Encoder::is_unstable_state(uint8_t state) {
  return state != this->stable_state;
}

bool Encoder::is_new_tick(uint8_t state) {
  return !this->is_stable_state && state == this->stable_state;
}

bool Encoder::is_same_tick(uint8_t state) {
  return this->is_stable_state && state == this->stable_state && this->tick;
}

void Encoder::read() {
  const uint8_t state = this->get_new_state();

  if (this->is_unstable_state(state)) {
    this->reset_stable_state();
    this->set_unstable_signal(state);

    return;
  }

  if (this->is_same_tick(state)) {
    this->reset_tick();

    return;
  }

  if (this->is_new_tick(state)) {
    this->set_tick();

    return;
  }
}
