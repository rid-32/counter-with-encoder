#include <Arduino.h>

#ifndef CTRL
#define CTRL

namespace ctrl {
struct Encoder_Event {
  bool positive_tick;
  bool negative_tick;
};

typedef void (*Encoder_Handler)(Encoder_Event const *event);

struct Encoder_Handlers {
  Encoder_Handler rotate;
};

class Encoder {
private:
  uint8_t stable_state, l_pin, r_pin, prev_unstable_state, unstable_signal;
  bool is_stable_state, is_new_unstable_state, tick, positive_tick,
      negative_tick;
  Encoder_Handlers handlers;

  uint8_t get_new_state();
  void reset_stable_state();
  void set_unstable_signal(uint8_t);
  void reset_tick();
  void set_tick();
  bool is_unstable_state(uint8_t);
  bool is_new_tick(uint8_t);
  bool is_same_tick(uint8_t);
  void read();

public:
  Encoder(uint8_t, uint8_t, uint8_t);

  void listen();
  void on(const char *event_name, Encoder_Handler);
};
} // namespace ctrl

#endif
