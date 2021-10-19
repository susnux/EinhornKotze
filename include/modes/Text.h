#ifndef TEXT_MODE_H
#define TEXT_MODE_H

#include "Mode.h"
#include <cstring>

#define TEXT_BITS_SIZE (LED_WIDTH / 8 + 1)

class TextMode : public Mode {
  public:
    TextMode();
    TextMode(const String& txt);
    ~TextMode();
    void setData(const uint8_t* data, uint8_t size);
  protected:
    void run(uint8_t delta);
  private:
    uint8_t cpos;
    float ppos;
    CRGB color;
    char* text;
    uint8_t bits[TEXT_BITS_SIZE];
    void nextChar();
    void initBits();
};

#endif