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
  if ( y & 0x01) {
    // Odd rows run backwards
    uint8_t reverseX = (LED_WIDTH - 1) - x;
    i = ((LED_HEIGHT - 1 - y) * LED_WIDTH) + reverseX;
  } else {
    // Even rows run forwards
    i = ((LED_HEIGHT - 1 - y) * LED_WIDTH) + x;
  }
  return i;
}