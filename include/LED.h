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
  FastLED.setBrightness(48);

  if (WiFi.status() == WL_CONNECTED)
    currentMode = new Blackout();
  else {
    currentMode = new TextMode("PRESS WPS");
    currentMode->speed = 64;
  }
}

void statusCallback(const OscMessage& m) {
  OscWiFi.send(m.remoteIP(), m.arg<int>(0), "/djpult/status", currentMode->mode, FastLED.getBrightness(), currentMode->speed);
}

void prefsCallback(const OscMessage& m) {
  Serial.println("PREFS");
  if (currentMode) currentMode->setData(m);
}

void modeCallback(const OscMessage& m) {
  Serial.println("CALLBACK CALLED");
  if (currentMode) {
    delete currentMode;
    currentMode = nullptr;
  }
  Serial.println(m.arg<int>(0));
  // Create new mode
  switch (m.arg<int>(0)) {
    //case MODE_MANUAL: break; // manual
    case MODE_STATIC_COLOR: currentMode = new StaticColor(m); break; // static
    case MODE_FADE: currentMode = new Fade(m); break;
    case MODE_RAINBOW: currentMode = new Rainbow(m); break; // Rainbow
    case MODE_TEXT: currentMode = new TextMode(m); break; // Text
    case MODE_FISHTANK: currentMode = new FishTank(m); break; // Fishtank
    case MODE_BLACKOUT:
    default:
      currentMode = new Blackout();
      break;
  }
}

void brightnessCallback(int& i) {
  if (0 <= i && i <= 255)
    FastLED.setBrightness(i);
}

void speedCallback(int& i) {
  if (0 <= i && i <= 255)
    currentMode->speed = i;
}

void ledLoop() {
  if (currentMode) currentMode->cycle();
}

#endif
