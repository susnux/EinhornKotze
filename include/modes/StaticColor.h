#ifndef STATIC_COLOR_H
#define STATIC_COLOR_H

#include "Mode.h"

class StaticColor : public Mode {
  public:
    StaticColor();
    StaticColor(uint8_t r, uint8_t g, uint8_t b);
    void setData(const uint8_t* data, uint8_t size);
  protected:
    void run(uint8_t delta);
  private:
    CRGB color;
};

StaticColor::StaticColor() : StaticColor(0, 0, 0) {}
StaticColor::StaticColor(uint8_t r, uint8_t g, uint8_t b) {
  this->mode = MODE_STATIC_COLOR;
  this->color = CRGB(r, g, b);
};

void StaticColor::setData(const uint8_t* data, uint8_t size) {
  if (size >= 3) memcpy(&this->color, data, sizeof(CRGB));
}

void StaticColor::run(uint8_t delta) {
  for (uint16_t i = 0; i < NUM_LEDS; ++i) leds[i] = this->color;
}

#endif