#ifndef LED_H
#define LED_H

#define LED_PIN  4

#include "modes/Blackout.h"
#include "modes/Text.h"
#include "modes/StaticColor.h"
#include "modes/Rainbow.h"
#include "modes/Fade.h"
#include "modes/FishTank.h"

Mode* currentMode = NULL;

void ledInit() {
  FastLED.addLeds<CHIPSET, LED_PIN, RGB>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
  FastLED.setBrightness(64);

  if (WiFi.status() == WL_CONNECTED)
    currentMode = new Blackout();
  else {
    currentMode = new FishTank();//TextMode("!"/*"PRESS WPS"*/);
    currentMode->speed = 64;
  }
}

void ledCallback(const uint8_t* data, const uint16_t size) {
  if (size >= 1 && (!currentMode || currentMode->mode != data[0])) {
    // Valid size, no current mode or different one
    // destruct previous mode
    if (currentMode) {
      delete currentMode;
      currentMode = NULL;
    }
    // Create new mode
    switch (data[0]) {
      case MODE_MANUAL: break; // manual
      case MODE_STATIC_COLOR: currentMode = new StaticColor(); break; // static
      case MODE_FADE: currentMode = new Fade(); break;
      case MODE_RAINBOW: currentMode = new Rainbow(); break; // Rainbow
      case MODE_TEXT: break; // Text
      case MODE_FISHTANK: currentMode = new FishTank(); break; // Fishtank
      default:
        currentMode = new Blackout();
        break;
    }
  }
  // brightness
  if (size >= 2) FastLED.setBrightness(data[1]);
  // speed
  if (size >= 3) currentMode->speed = data[2];
  // data
  if (size > 3) currentMode->setData(data+3, size - 3);
}

void ledLoop() {
  if (currentMode) currentMode->cycle();
}

#endif
