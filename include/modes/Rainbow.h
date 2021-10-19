#ifndef RAINBOW_H
#define RAINBOW_H

#include "Mode.h"

class Rainbow : public Mode {
  public:
    Rainbow();
    void setData(const uint8_t* data, uint8_t size);
  protected:
    void run(uint8_t delta);
  private:
    float position;
    bool horizontal;
};

Rainbow::Rainbow() : position(0), horizontal(true) {
  this->mode = MODE_RAINBOW;
}

void Rainbow::setData(const uint8_t* data, uint8_t size) {
  if (size >= 1) this->horizontal = data[0] <= 127;
}


void Rainbow::run(uint8_t delta) {
  uint8_t max = this->horizontal ? LED_WIDTH : LED_HEIGHT;
  for (uint8_t i = 0; i < max; ++i) {
    uint16_t g = static_cast<uint16_t>(position + (383.f / max) * i) % 383;
    uint16_t r = (g + 128) % 383;
    uint16_t b = (g + 256) % 383;
    for (uint8_t j = 0; j < (this->horizontal ? LED_HEIGHT : LED_WIDTH); ++j) {
      leds[this->horizontal ? XY(i, j) : XY(j, i)] = CRGB(quadwave8(r > 255 ? 0 : r), quadwave8(g > 255 ? 0 : g), quadwave8(b > 255 ? 0 : b));
    }
  }
  if (this->speed > 0) {
    this->position += (delta * 383) / (-297500.f * this->speed / 254 + 300000);
    if (this->position >= 383) this->position -= 383;
  }
}

#endif