#include "modes/Text.h"

uint8_t fontpage[] = {0,0,0,0,0,0,0,0,  0,0,0,0x3A,0,0,0,0};


TextMode::TextMode() : TextMode("") {
}

TextMode::TextMode(const String& txt) :
    cpos(0), ppos(LED_WIDTH), color(CRGB::White), text(new char[txt.length()]), bits({0})
{
    this->mode = MODE_TEXT;
    memcpy(this->text, txt.c_str(), txt.length());

    initBits();
}


TextMode::~TextMode() {
  if (this->text) delete this->text;
}

void TextMode::initBits() {
    for (int8_t i = 0; i < TEXT_BITS_SIZE; ++i)
        memcpy(bits, fontpage + ((i + cpos >= strlen(text) ? 0x20 : text[cpos + i])*8), 8);
}

void TextMode::nextChar() {
    memmove(bits, bits+8, TEXT_BITS_SIZE - 8);
    memcpy(bits + TEXT_BITS_SIZE - 8, fontpage + ((TEXT_BITS_SIZE - 1 + cpos >= strlen(text) ? 0x20 : text[TEXT_BITS_SIZE - 1 + cpos])*8), 8);
}

void TextMode::setData(const uint8_t* data, uint8_t size) {
    if (size > 2) memcpy(&this->color, data, sizeof(CRGB));
    if (size > 3) {
        const uint8_t length = strnlen(reinterpret_cast<const char*>(data), size - 3);
  
        if (this->text)
            delete this->text;
        this->text = new char[length];
        memcpy(this->text, data, length);
    }
}

void TextMode::run(uint8_t delta) {
    ppos -= static_cast<float>(this->speed * 4 * 8 + 1 * 8) / delta;

    if (ppos < -7.5f) {
        ppos = 0;
        cpos += 1;
        if (cpos == strlen(text)) {
            cpos = 0;
            ppos = LED_WIDTH;
            initBits();
        } else {
            nextChar();
        }
    }

    for(uint8_t y = 0; y < LED_HEIGHT; ++y)
        for(uint8_t x = 0; x < LED_WIDTH; ++x)
            if (x >= ppos && (bits[static_cast<uint8_t>(x - ppos + 0.5f)] & (1<<y))) leds[XY(x,y)] = color;
            else leds[XY(x,y)] = CRGB::Black;
}