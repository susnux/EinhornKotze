#ifndef TEXT_MODE_H
#define TEXT_MODE_H

#include "Mode.h"
#define MAX_CHAR_WIDTH 8

struct Font;

class TextMode : public Mode {
  public:
    TextMode(const OscMessage& m);
    TextMode(const String& txt);
    ~TextMode();
    void setData(const OscMessage& m);
  protected:
    void run(uint8_t delta);
  private:
    Font* font;
    String text;
    
    CRGB color;
    CRGB background_color;
    
    uint8_t cpos;
    float ppos;
    
    uint8_t display[LED_WIDTH + MAX_CHAR_WIDTH + 1];
    
    void nextChar();
    void init();
};

#endif