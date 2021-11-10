#ifndef RAINBOW_H
#define RAINBOW_H

#include "Mode.h"

class Rainbow : public Mode {
  public:
    Rainbow(const OscMessage& m);
    void setData(const OscMessage& m);
  protected:
    void run(uint8_t delta);
  private:
    float position;
    bool vertical;
};

Rainbow::Rainbow(const OscMessage& m) : Mode(m), position(0), vertical(true) {
  this->mode = MODE_RAINBOW;
  if (m.size() > 2 && m.isBool(2)) this->vertical = m.arg<bool>(2);
}

void Rainbow::setData(const OscMessage& m) {
  if (m.address().endsWith("/vertical") && m.isBool(0))
    this->vertical = m.arg<bool>(0);
}

void Rainbow::run(uint8_t delta) {
  uint8_t max = this->vertical ? LED_HEIGHT : LED_WIDTH;
  for (uint8_t i = 0; i < max; ++i) {
    uint16_t g = static_cast<uint16_t>(position + (383.f / max) * i) % 383;
    uint16_t r = (g + 128) % 383;
    uint16_t b = (g + 256) % 383;
    for (uint8_t j = 0; j < (this->vertical ? LED_WIDTH : LED_HEIGHT); ++j) {
      leds[this->vertical ? XY(j, i) : XY(i, j)] = CRGB(quadwave8(r > 255 ? 0 : r), quadwave8(g > 255 ? 0 : g), quadwave8(b > 255 ? 0 : b));
    }
  }
  if (this->speed > 0) {
    this->position += (delta * 383) / (-297500.f * this->speed / 254 + 300000);
    if (this->position >= 383) this->position -= 383;
  }
}

#endif