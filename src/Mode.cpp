#include "modes/Mode.h"

CRGB leds[NUM_LEDS];

Mode::Mode(const OscMessage& m)
{
  if (m.size() > 1 && m.isInt32(1)) this->speed = m.arg<int>(1);
}

void Mode::cycle() {
  unsigned long current = millis();
  uint16_t diff = current - this->lastMillis;
  if (diff > 10) {
    this->run(diff);
    this->lastMillis = current;
    FastLED.show();
  }
}

void Mode::run(uint8_t delta) {
  // OVERRIDE!!!
}

uint16_t XY( uint8_t x, uint8_t y) {
  uint16_t i;
#ifdef ORIGIN_UPPER_LEFT
  // Origin at upper left, default is lower left
  y = LED_HEIGHT - 1 - y;
#endif
  if (y & 1)
    i = y * LED_WIDTH + (LED_WIDTH - 1 - x);
  else
    i = y * LED_WIDTH + x;
  return i;
}
