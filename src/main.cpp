#define FASTLED_ESP32_I2S

#define DEBOUNCE 50
#define BUTTON_PIN 2

#include "Network.h"
#include "LED.h"

#include <Arduino.h>

#include <Preferences.h>
#include <ArtnetWiFi.h>
#include <ezButton.h>

ArtnetWiFiReceiver artnet;
Preferences preferences;
ezButton wpsButton(BUTTON_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait Serial is ready
  Serial.setTimeout(10000);

  // storage for WiFi credentials (R/W)
  preferences.begin("wifi", false);

  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.setHostname("DJPult");
  wpsInitConfig();
  wpsButton.setDebounceTime(50);

  delay(2000); // required, without there is no output on startup
  Serial.println("Enter the WiFi SSID or wait 10 seconds");

  String ssid = Serial.readStringUntil('\n');
  String password = preferences.getString("pass", "");
  if (ssid != ""){
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.println("Enter WiFi password:");
    password = Serial.readStringUntil('\n');
    if (connectToNetwork(ssid, password)) {
      preferences.putString("ssid", ssid);
      preferences.putString("pass", password);
    }
  } else {
    ssid = preferences.getString("ssid", "");
    connectToNetwork(ssid, password);
  }
    
  if (WiFi.status() == WL_CONNECTED)
    artnet.begin();
  else
    Serial.println("Could not connect to WiFi, use WPS.");

  ledInit();

  artnet.forward(1, leds, 170);
  artnet.forward(2, leds+170, LED_HEIGHT*LED_WIDTH-170);
  artnet.subscribe(0, ledCallback);
}

void loop() {
  wpsButton.loop();
  if (wpsButton.isPressed()) wpsStart();
  if (WiFi.status() == WL_CONNECTED) {
    artnet.parse();
  }
  ledLoop();
}