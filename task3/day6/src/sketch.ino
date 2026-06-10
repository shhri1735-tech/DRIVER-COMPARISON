#include <WiFi.h>
#include <ArduinoOTA.h>
#include "secrets.h"

#define OTA_HOSTNAME "esp32-zelbytes-lab"
#define OTA_PASSWORD "zelbytes2026"

int otaProgress = 0;

void connectWiFi();
void setupOTA();

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("================================");
  Serial.println("  Day 06 - OTA Update Manager");
  Serial.println("  Zelbytes IoT Lab");
  Serial.println("================================");
  connectWiFi();
  setupOTA();
  Serial.println("Ready. Waiting for OTA...");
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - startTime > 15000) {
      Serial.println("\nWiFi failed!");
      return;
    }
  }
  Serial.println("\nWiFi connected ✓");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI       : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void setupOTA() {
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH)
                  ? "sketch" : "filesystem";
    Serial.println("OTA Start: " + type);
    otaProgress = 0;
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA Complete ✓");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    int percent = progress / (total / 100);
    if (percent != otaProgress) {
      otaProgress = percent;
      Serial.print("OTA Progress: ");
      Serial.print(percent);
      Serial.println("%");
    }
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.print("OTA Error: ");
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth failed - wrong password!");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin failed!");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect failed!");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive failed!");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End failed!");
    }
  });

  ArduinoOTA.begin();

  Serial.println("========================");
  Serial.println("OTA ready ✓");
  Serial.print("Hostname : ");
  Serial.println(OTA_HOSTNAME);
  Serial.println("Password : set");
  Serial.println("Open OTA : rejected");
  Serial.println("========================");
}

void loop() {
  ArduinoOTA.handle();
  Serial.println("Loop running - OTA listening...");
  delay(5000);
}