#ifndef MODE_H
#define MODE_H

#define FASTLED_ESP32_I2S
#include <FastLED.h>
#include <ArduinoOSCWiFi.h>

#define LED_HEIGHT 8
#define LED_WIDTH 38
#define NUM_LEDS (LED_HEIGHT * LED_WIDTH)

#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define MODE_BLACKOUT 0
#define MODE_MANUAL 1
#define MODE_STATIC_COLOR 2
#define MODE_FADE 3
#define MODE_RAINBOW 4
#define MODE_TEXT 5
#define MODE_FISHTANK 6

uint16_t XY( uint8_t x, uint8_t y);

extern CRGB leds[NUM_LEDS];

class Mode {
  public:
    Mode() = default;
    Mode(const OscMessage& m);
    virtual ~Mode() = default;
    uint8_t mode;
    uint8_t speed;
    /** Called if there is any additional data */
    virtual void setData(const OscMessage& m) {};
    /** Called every cycle*/
    void cycle();
  protected:
    /** Override this for your own purpose, called at most every 10ms (100 Hz) */
    virtual void run(uint8_t delta);
    unsigned long lastMillis;
};

#endif