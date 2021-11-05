#ifndef FADE_H
#define FADE_H

#include "Mode.h"

class Fade : public Mode {
  public:
    Fade(const OscMessage& m);
  protected:
    void run(uint8_t delta);
  private:
    float position;
};

Fade::Fade(const OscMessage& m) : Mode(m) {
  this->mode = MODE_FADE;
}

void Fade::run(uint8_t delta) {
  uint16_t r = static_cast<uint16_t>(this->position + 128) % 383;
  uint16_t b = static_cast<uint16_t>(this->position + 256) % 383;
  for (uint16_t a = 0; a < NUM_LEDS; ++a)
    leds[a] = CRGB(quadwave8(r > 255 ? 0 : r), quadwave8(this->position > 255 ? 0 : this->position), quadwave8(b > 255 ? 0 : b));
  this->position += (delta * 383) / (-297500.f * this->speed / 255 + 300000);
  if (this->position >= 383) this->position -= 383;
}

#endif