#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>

void connectToWiFi(const char* ssid, const char* passphrase) {
  Serial.begin(9600);
  // Disconnecting WiFi if it"s already connected.
  WiFi.disconnect();
  // Setting it to Station mode which basically scans for nearby WiFi routers.
  WiFi.mode(WIFI_STA);
  // Begin connecting to WiFi
  WiFi.begin(ssid, passphrase);
  Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid, passphrase);
}

#endif /* WIFI_H */