#include <Grandeur.h>
#include "WiFi.h"

// WiFi credentials
const char* ssid = YourWiFiSsid;
const char* passphrase = YourWiFiPassphrase;

// Grandeur credentials
const char * apiKey = YourApiKey;
const char* token = YourDeviceToken;
const char* deviceId = YourDeviceId;

Grandeur::Project project;

void setup() {
  Serial.begin(9600);
  // This connects to the device to WiFi.
  connectToWiFi(ssid, passphrase);
  // You can initialize device configurations like this.
  project = grandeur.init(apiKey, token);
}

unsigned long current = millis();

void loop() {
  // This sends data to internet.
  if(project.isConnected() && millis() - current > 500) {
    project.device(deviceId).data().set("millis", millis());
    current = millis();
  }
  // This runs the SDK when the device WiFi is connected.
  if(WiFi.status() == WL_CONNECTED) project.loop();
}