#include "modes/Text.h"
#include <cstring>

#include <LITTLEFS.h>

#define MAX_FONT 1

struct Font {
    Font(uint8_t id) : id(id) {
        switch(id % (MAX_FONT + 1)) {
            case 0:
                file = LITTLEFS.open("/font8x8.bin", "r");
                break;
            case 1:
                file = LITTLEFS.open("/font5x8.bin", "r");
                break;
        }
        // Error handling?
        file.read(&width, 1);
        file.read(&height, 1);
        buffer = new uint8_t[width];
    }

    ~Font() {
        file.close();
        delete buffer;
    }

    File file;
    uint8_t width;
    uint8_t height;
    uint8_t id;
    uint8_t* buffer;

    uint8_t* get(const char c) {
        file.seek(2 + width * static_cast<uint8_t>(c), fs::SeekSet);
        file.read(buffer, width);
        return buffer;
    }
};



// FONT, COLOR, BACKGROUND, TEXT

TextMode::TextMode(const OscMessage& m) :
    Mode(m), color(CRGB::White), background_color(CRGB::Black), cpos(0), ppos(0), display({0}) 
{
    this->mode = MODE_TEXT;
    if (m.size() > 2 && m.isInt32(2))
        this->background_color = m.arg<int32_t>(2);
    if (m.size() > 3 && m.isInt32(3))
        this->color = m.arg<int32_t>(3);
    if (m.size() > 4 && m.isInt32(4))
        this->font = new Font(m.arg<int32_t>(4));
    else
        this->font = new Font(0);
    if (m.size() > 5 && m.isStr(5))
        this->text = m.arg<String>(5);
    init();
}

TextMode::TextMode(const String& txt) :
    font(new Font(0)), text(txt), color(CRGB::White), background_color(CRGB::Black), cpos(0), ppos(0), display({0}) 
{
    this->mode = MODE_TEXT;
    this->speed = 32;
    init();
    Serial.print("Init...");
    Serial.println(text);
}


TextMode::~TextMode() {
    if (this->font) delete this->font;
}

void TextMode::init() {
    cpos = 0;
    ppos = 0;
    for (int i = 0; i < LED_WIDTH*LED_HEIGHT; ++i)
        leds[i] = background_color;
    for (int i = 0; i < LED_WIDTH + 8; ++i)
        display[i] = 0;
    if (text)
        nextChar();
}

void TextMode::nextChar() {
    memmove(display, display + font->width + 1, LED_WIDTH + MAX_CHAR_WIDTH - font->width);
    memset(display+LED_WIDTH, 0, font->width + 1);

    if (cpos > text.length()) {
        cpos = 0;
    }
    
    memcpy(display + LED_WIDTH + 1, font->get(text[cpos]), font->width);
    Serial.print(text[cpos]);
    Serial.print(" : ");
    Serial.println(*font->get(text[cpos]), HEX);
}

void TextMode::setData(const OscMessage& m) {
    if (m.address().endsWith("/font") && m.isInt32(0)) {
        if (this->font) delete this->font;
        this->font = new Font(m.arg<int32_t>(0));
        init();
    } else if (m.address().endsWith("/color") && m.isInt32(0)) {
        this->color = m.arg<int32_t>(0);
    } else if (m.address().endsWith("/background") && m.isInt32(0)){
        this->background_color = m.arg<int32_t>(0);
    } else if (m.address().endsWith("/text") && m.isStr(0)) {
        this->text = m.arg<String>(0);
        init();
    }
}

void TextMode::run(uint8_t delta) {
    ppos += (font->width * speed / 255.f + 1) * (delta / 1000.f);

    if (ppos >= (font->width + 1.5f)) {
        ppos = 0;
        cpos++;
        nextChar();
    }

    for(uint8_t x = 0; x < LED_WIDTH; ++x)
        for(uint8_t y = 0; y < LED_HEIGHT; ++y)
            leds[XY(x,y)] = display[static_cast<uint8_t>(ppos + 0.5f) + x] & (1 << y) ? color : background_color;
}