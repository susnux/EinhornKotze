#define ESP_WPS_MODE      WPS_TYPE_PBC
#define ESP_MANUFACTURER  "ESPRESSIF"
#define ESP_MODEL_NUMBER  "ESP-WROOM-32"
#define ESP_MODEL_NAME    "ESPRESSIF IOT"
#define ESP_DEVICE_NAME   "DJ PULT"

#define CONNECTION_RETRIES 10

#include "WiFi.h"
#include "esp_wps.h"

static esp_wps_config_t config;

bool connectToNetwork(String& ssid, String& password) {
  if (ssid == "") return false;
  if (password == "") WiFi.begin(ssid.c_str());
  else WiFi.begin(ssid.c_str(), password.c_str());
 
  static uint8_t tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
    if (++tries > CONNECTION_RETRIES) return false;
  }
  Serial.print("Connected to network, IP: ");
  Serial.println(WiFi.localIP());
  return true;
}


void wpsInitConfig(){
  config.wps_type = ESP_WPS_MODE;
  strcpy(config.factory_info.manufacturer, ESP_MANUFACTURER);
  strcpy(config.factory_info.model_number, ESP_MODEL_NUMBER);
  strcpy(config.factory_info.model_name, ESP_MODEL_NAME);
  strcpy(config.factory_info.device_name, ESP_DEVICE_NAME);
}

void wpsStart(){
    if(!esp_wifi_wps_enable(&config)){
      esp_wifi_wps_start(0);
    }
}

String wpspin2string(uint8_t a[]){
  char wps_pin[9];
  for(int i=0;i<8;i++){
    wps_pin[i] = a[i];
  }
  wps_pin[8] = '\0';
  return (String)wps_pin;
}

void WiFiEvent(system_event_id_t event){
  switch(event){
    case SYSTEM_EVENT_STA_DISCONNECTED:
      // Disconnected from station, attempting reconnection
      WiFi.reconnect();
      break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
      Serial.println("WPS Successfull, stopping WPS and connecting to: " + String(WiFi.SSID()));
      esp_wifi_wps_disable();
      delay(10);
      WiFi.begin();
      break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
      //WPS Timedout, retrying
      esp_wifi_wps_disable();
      wpsStart();
      break;
    default:
      break;
  }
}