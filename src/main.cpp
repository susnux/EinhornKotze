#define FASTLED_ESP32_I2S

#define DEBOUNCE 50
#define BUTTON_PIN 2

#include "Network.h"
#include "LED.h"

#include <Arduino.h>

#include <Preferences.h>
#include <ezButton.h>
#include "LITTLEFS.h"

Preferences preferences;
ezButton wpsButton(BUTTON_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait Serial is ready
  Serial.setTimeout(10000);

  // storage for WiFi credentials (R/W)
  preferences.begin("settings", false);

  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.setHostname("DJPult");
  wpsInitConfig();
  wpsButton.setDebounceTime(50);

  delay(2000); // required, without there is no output on startup
  Serial.println("Please use LF (newline) for serial communication.");
  Serial.println("Enter the WiFi SSID or wait 10 seconds");
  Serial.print("SSID: ");

  String ssid = Serial.readStringUntil('\n');
  String password = preferences.getString("pass", "");
  if (ssid != ""){
    Serial.println(ssid);
    Serial.println("Enter WiFi password:");
    password = Serial.readStringUntil('\n');
    if (connectToNetwork(ssid, password)) {
      preferences.putString("ssid", ssid);
      preferences.putString("pass", password);
    }
  } else {
    Serial.print("\n");
    ssid = preferences.getString("ssid", "");
    connectToNetwork(ssid, password);
  }
    
  if (WiFi.status() != WL_CONNECTED)
    Serial.println("Could not connect to WiFi, use WPS.");

  Serial.println("OSC setup, change port or wait 10s.");
  uint16_t osc_rx = preferences.getUInt("osc_rx", 8000);
  Serial.print("OSC RX port (");
  Serial.print(osc_rx);
  Serial.print("): ");
  auto buffer = Serial.readStringUntil('\n');
  if (buffer) {
    auto t = buffer.toInt();
    if (t > 21) {
      osc_rx = buffer.toInt();
      Serial.println(osc_rx);
    } else {
      Serial.println("ERR");
    }
  } else
    Serial.println("");

  uint16_t osc_tx = preferences.getUInt("osc_tx", 8001);

  if(!LITTLEFS.begin()){
    Serial.println("LittleFS Mount Failed");
  }

  ledInit();

  OscWiFi.subscribe(osc_rx, "/djpult/status", statusCallback);
  
  OscWiFi.subscribe(osc_rx, "/djpult/mode", modeCallback);
  OscWiFi.subscribe(osc_rx, "/djpult/mode/*", prefsCallback);
  OscWiFi.subscribe(osc_rx, "/djpult/brightness", brightnessCallback);
  OscWiFi.subscribe(osc_rx, "/djpult/speed", speedCallback);
  Serial.println("DONE");
}

void loop() {
  wpsButton.loop();
  if (wpsButton.isPressed()) wpsStart();
  if (WiFi.status() == WL_CONNECTED) {
    OscWiFi.update();
  }
  ledLoop();
}