#ifndef MANUAL_MODE_H
#define MANUAL_MODE_H

#include "Mode.h"

class Manual : public Mode {
  public:
    Manual(const OscMessage& m);
    void setData(const OscMessage& m);
  protected:
    void run(uint8_t delta);
};

Manual::Manual(const OscMessage& m) : Mode(m) {
  this->mode = MODE_MANUAL;

  for (uint16_t i = 0; i < NUM_LEDS; ++i)
    leds[i] = CRGB::Black;
}

void Manual::setData(const OscMessage& m) {
  if (m.address().endsWith("/pixels") && m.isBlob(0)) {
    auto data = m.getArgAsBlob(0);
    if (data.size() < NUM_LEDS*3)
      return; // ERROR
    for (uint16_t y = 0; y < LED_HEIGHT; ++y)
      for (uint16_t x = 0; x < LED_WIDTH; ++x)
        leds[XY(x,y)] = CRGB(data.at(y*LED_WIDTH*3 + x*3), data.at(y*LED_WIDTH*3 + x*3 + 1), data.at(y*LED_WIDTH*3 + x*3 + 2 ));
  }
}

void Manual::run(uint8_t delta) {
  // noop
}

#endif