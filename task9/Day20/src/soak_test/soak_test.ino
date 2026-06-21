#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include "secrets.h"

#define DHT_PIN     4
#define DHT_TYPE    DHT22
#define PUBLISH_INTERVAL_MS  60000UL        // publish every 60 seconds
#define SOAK_DURATION_MS    (24UL * 60 * 60 * 1000)  // 24 hours in ms

DHT dht(DHT_PIN, DHT_TYPE);

unsigned long lastPub    = 0;
unsigned long soakStart  = 0;
unsigned long publishCount   = 0;
unsigned long failCount      = 0;
unsigned long dhtFailCount   = 0;
unsigned long wifiDropCount  = 0;
bool soakComplete = false;

// ─── WiFi reconnect with drop counter ────────────────────────────────────────
void ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  wifiDropCount++;
  Serial.printf("[WIFI] Disconnected! Reconnecting... (drop #%lu)\n", wifiDropCount);

  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long t = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - t > 30000) {
      Serial.println("[WIFI] Reconnect timeout after 30s — will retry next cycle");
      return;
    }
    delay(200);
  }
  Serial.println("[WIFI] Reconnected");
}

// ─── RSSI helpers ─────────────────────────────────────────────────────────────
String rssiQuality(int rssi) {
  if (rssi >= -60) return "Excellent";
  if (rssi >= -70) return "Good";
  if (rssi >= -80) return "Fair";
  return "Poor";
}

int readRSSI() {
  return WiFi.RSSI();   // returns dBm, e.g. -65
}

// ─── MQTT publish (your original function, unchanged) ─────────────────────────
bool publishMqtt(float t, float h) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  if (!http.begin(client, "https://careers.zelbytes.com/api/iot-lab/v1/mqtt/publish"))
    return false;

  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Iot-Lab-Key", IOT_LAB_KEY);

  String body = "{\"topic\":\"" + String(MQTT_TOPIC) + "\","
                "\"payload\":\"{\\\"temp_c\\\":" + String(t, 1) +
                ",\\\"humidity_pct\\\":" + String(h, 1) + "}\"}";

  unsigned long startTime = millis();
  int code = http.POST(body);
  unsigned long latency = millis() - startTime;

  String resp = http.getString();
  http.end();

  Serial.printf("[MQTT] HTTP %d | latency %lu ms | %s\n", code, latency, resp.c_str());
  return code == 200 || code == 202;
}

// ─── Publish with RSSI payload added ─────────────────────────────────────────
bool publishSoakReading(float t, float h, int rssi) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  if (!http.begin(client, "https://careers.zelbytes.com/api/iot-lab/v1/mqtt/publish"))
    return false;

  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Iot-Lab-Key", IOT_LAB_KEY);

  // payload now includes rssi, uptime, and counts for the soak log
  unsigned long uptimeSec = (millis() - soakStart) / 1000;

  String payload = "{\\\"temp_c\\\":" + String(t, 1) +
                   ",\\\"humidity_pct\\\":" + String(h, 1) +
                   ",\\\"rssi_dbm\\\":" + String(rssi) +
                   ",\\\"rssi_quality\\\":\\\"" + rssiQuality(rssi) + "\\\"" +
                   ",\\\"uptime_sec\\\":" + String(uptimeSec) +
                   ",\\\"publish_count\\\":" + String(publishCount) +
                   ",\\\"fail_count\\\":" + String(failCount) +
                   ",\\\"wifi_drops\\\":" + String(wifiDropCount) + "}";

  String body = "{\"topic\":\"" + String(MQTT_TOPIC) + "\","
                "\"payload\":\"" + payload + "\"}";

  unsigned long startTime = millis();
  int code = http.POST(body);
  unsigned long latency = millis() - startTime;

  http.end();

  Serial.printf("[MQTT] HTTP %d | latency %lu ms\n", code, latency);
  return code == 200 || code == 202;
}

// ─── Print soak summary to Serial ────────────────────────────────────────────
void printSoakSummary() {
  unsigned long elapsed = millis() - soakStart;
  Serial.println("\n============================================================");
  Serial.println("           24-HOUR SOAK TEST — FINAL SUMMARY");
  Serial.println("============================================================");
  Serial.printf("  Duration         : %lu h %lu m\n",
                elapsed / 3600000, (elapsed % 3600000) / 60000);
  Serial.printf("  Total publishes  : %lu\n", publishCount);
  Serial.printf("  Failed publishes : %lu\n", failCount);
  Serial.printf("  DHT read fails   : %lu\n", dhtFailCount);
  Serial.printf("  WiFi drops       : %lu\n", wifiDropCount);
  Serial.printf("  Current RSSI     : %d dBm (%s)\n",
                readRSSI(), rssiQuality(readRSSI()).c_str());
  Serial.printf("  Expected reads   : %lu\n", SOAK_DURATION_MS / PUBLISH_INTERVAL_MS);
  float successRate = (publishCount > 0)
                      ? (float)(publishCount - failCount) / publishCount * 100.0f
                      : 0;
  Serial.printf("  Success rate     : %.1f%%\n", successRate);
  Serial.println(successRate >= 95.0 ? "  RESULT           : *** PASS ***"
                                     : "  RESULT           : *** FAIL — check logs ***");
  Serial.println("============================================================\n");
}

// ─── Print per-reading log line (copy this into your soak log sheet) ─────────
void printLogLine(float t, float h, int rssi, bool ok) {
  unsigned long elapsed  = millis() - soakStart;
  unsigned long hours    = elapsed / 3600000;
  unsigned long minutes  = (elapsed % 3600000) / 60000;

  Serial.printf("[SOAK T+%02luh%02lum] Temp: %.1f°C | Hum: %.1f%% | "
                "RSSI: %d dBm (%s) | MQTT: %s | "
                "Publishes: %lu | Fails: %lu | WiFi drops: %lu\n",
                hours, minutes, t, h, rssi,
                rssiQuality(rssi).c_str(),
                ok ? "OK" : "FAIL",
                publishCount, failCount, wifiDropCount);
}

// ─────────────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();
  delay(3000);

  Serial.println("\n[SOAK] Starting 24-hour soak test...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(200);

  Serial.printf("[WIFI] Connected. IP: %s | RSSI: %d dBm (%s)\n",
                WiFi.localIP().toString().c_str(),
                readRSSI(), rssiQuality(readRSSI()).c_str());

  soakStart = millis();
  Serial.printf("[SOAK] Soak start time recorded. Will run for 24 hours.\n\n");
}

void loop() {
  // ── Soak complete — just idle and print reminder ──────────────────────────
  if (soakComplete) {
    delay(60000);
    Serial.println("[SOAK] Test complete. Review summary above. Power off node.");
    return;
  }

  // ── Check if 24 hours elapsed ─────────────────────────────────────────────
  if (millis() - soakStart >= SOAK_DURATION_MS) {
    soakComplete = true;
    printSoakSummary();
    return;
  }

  // ── Publish every 60 seconds ─────────────────────────────────────────────
  if (millis() - lastPub >= PUBLISH_INTERVAL_MS || lastPub == 0) {
    lastPub = millis();

    ensureWiFi();

    int rssi = readRSSI();
    float t  = dht.readTemperature();
    float h  = dht.readHumidity();

    bool dhtOk = !isnan(t) && !isnan(h);
    if (!dhtOk) {
      dhtFailCount++;
      Serial.printf("[DHT] Read failed (#%lu) — using fallback values\n", dhtFailCount);
      t = 24.1; h = 68.2;   // fallback so MQTT/latency test still runs
    }

    publishCount++;
    bool ok = publishSoakReading(t, h, rssi);
    if (!ok) failCount++;

    printLogLine(t, h, rssi, ok);
  }
}
