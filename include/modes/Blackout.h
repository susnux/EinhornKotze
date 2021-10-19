#ifndef BLACKOUT_H
#define BLACKOUT_H

#include "Mode.h"

class Blackout : public Mode {
  public:
    Blackout() {this->mode = MODE_BLACKOUT;};
    void run() {
      for (uint8_t i = 0; i < NUM_LEDS; ++i) leds[i] = CRGB::Black;
    }
};

#endif