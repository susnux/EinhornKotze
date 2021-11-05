#ifndef STATIC_COLOR_H
#define STATIC_COLOR_H

#include "Mode.h"

class StaticColor : public Mode {
  public:
    StaticColor(const OscMessage& m);
    void setData(const OscMessage& m);
  protected:
    void run(uint8_t delta);
  private:
    CRGB color;
};

StaticColor::StaticColor(const OscMessage& m) : Mode(m), color(CRGB::Black) {
  this->mode = MODE_STATIC_COLOR;
  if (m.size() > 2 && m.isInt32(2))
    this->color = m.arg<int32_t>(2);
}

void StaticColor::setData(const OscMessage& m) {
  if (m.address().endsWith("/color") && m.isInt32(0))
    this->color = m.arg<int32_t>(0);
}

void StaticColor::run(uint8_t delta) {
  for (uint16_t i = 0; i < NUM_LEDS; ++i) leds[i] = this->color;
}

#endif