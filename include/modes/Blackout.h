#ifndef BLACKOUT_H
#define BLACKOUT_H

#include "Mode.h"

class Blackout : public Mode {
  public:
    Blackout() {
      this->mode = MODE_BLACKOUT;
    };
    void run(uint8_t delta) {
      for (uint16_t i = 0; i < NUM_LEDS; ++i) leds[i] = CRGB::Black;
      usleep(50);
    }
};

#endif