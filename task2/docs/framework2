// src/day04_ntp_time.ino
#include <WiFi.h>
#include <time.h>
#include "secrets.h"

const char* NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET_SEC = 5 * 3600 + 30 * 60;  // IST
const int DAYLIGHT_OFFSET_SEC = 0;

bool timeSynced = false;

bool syncTime(uint32_t timeoutMs = 10000) {
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
  struct tm ti;
  uint32_t start = millis();
  while (!getLocalTime(&ti) && (millis() - start) < timeoutMs) {
    delay(200);
    Serial.print('.');
  }
  Serial.println();
  timeSynced = getLocalTime(&ti);
  if (timeSynced) {
    Serial.printf("NTP OK: %04d-%02d-%02d %02d:%02d:%02d\n",
                  ti.tm_year + 1900, ti.tm_mon + 1, ti.tm_mday,
                  ti.tm_hour, ti.tm_min, ti.tm_sec);
  }
  return timeSynced;
}

String isoTimestamp() {
  struct tm ti;
  if (!getLocalTime(&ti)) return String("1970-01-01T00:00:00Z");
  char buf[25];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &ti);
  return String(buf);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(300); Serial.print('.'); }
  Serial.println(WiFi.localIP());
  syncTime();
}

void loop() {
  Serial.printf("Now: %s | valid: %s\n", isoTimestamp().c_str(), timeSynced ? "yes" : "no");
  delay(5000);
}
