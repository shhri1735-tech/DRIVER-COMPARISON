#include <WiFi.h>
#include <ArduinoOTA.h>
#include "secrets.h"

#define SLEEP_AFTER_MS   30000        // 30 seconds awake
#define DEEP_SLEEP_SEC   10           // sleep duration (change as needed)

unsigned long wakeTime;

void setup() {
  Serial.begin(115200);
  delay(3000);
  wakeTime = millis();              // record when we woke up
  Serial.println("--- STARTING ---");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - startTime > 15000) {
      Serial.println("\nWiFi FAILED! Going to sleep...");
      goToSleep();                  // sleep even if WiFi fails
    }
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA.setHostname("esp32-zelbytes-lab");
  ArduinoOTA.setPassword("zelbytes2026");

  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("OTA Start: updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA Complete! Rebooting...");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error [%u]: ", error);
    if      (error == OTA_AUTH_ERROR)    Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)   Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)     Serial.println("End Failed");
  });

  ArduinoOTA.begin();
  Serial.println("OTA ready! Will sleep in 30s if no OTA activity.");
}

void loop() {
  ArduinoOTA.handle();

  // ── Check if 30 seconds have passed since wake ──
  if (millis() - wakeTime >= SLEEP_AFTER_MS) {
    Serial.println("30s timeout reached. Going to deep sleep...");
    goToSleep();
  }

  Serial.println("Loop running...");
  delay(1000);                      // 1s delay so OTA stays responsive
}

// ── Deep Sleep Helper ────────────────────────────────────────
void goToSleep() {
  Serial.println("Disconnecting WiFi...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(100);

  Serial.printf("Sleeping for %d seconds. Goodnight!\n", DEEP_SLEEP_SEC);
  Serial.flush();                   // flush serial before sleeping

  esp_sleep_enable_timer_wakeup((uint64_t)DEEP_SLEEP_SEC * 1000000ULL); // microseconds
  esp_deep_sleep_start();           // ESP32 stops here, reboots on wake
}
