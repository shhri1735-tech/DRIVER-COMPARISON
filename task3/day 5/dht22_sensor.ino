// src/day05_dht22.ino
#include <DHT.h>

#define DHT_PIN  4
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

struct Reading {
  float tempC;
  float humidityPct;
  bool valid;
};

Reading readDhtRobust() {
  Reading r = {NAN, NAN, false};
  for (int attempt = 0; attempt < 3; attempt++) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t) && !isnan(h) && h >= 0 && h <= 100) {
      r.tempC = t;
      r.humidityPct = h;
      r.valid = true;
      return r;
    }
    delay(250);
  }
  return r;
}

float median3(float a, float b, float c) {
  if (a > b) { float t = a; a = b; b = t; }
  if (b > c) { float t = b; b = c; c = t; }
  if (a > b) { float t = a; a = b; b = t; }
  return b;
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(2000);  // DHT warm-up
}

void loop() {
  static float tHist[3] = {NAN, NAN, NAN};
  static float hHist[3] = {NAN, NAN, NAN};
  static uint8_t idx = 0;

  Reading r = readDhtRobust();
  if (r.valid) {
    tHist[idx % 3] = r.tempC;
    hHist[idx % 3] = r.humidityPct;
    idx++;
    float tMed = median3(tHist[0], tHist[1], tHist[2]);
    float hMed = median3(hHist[0], hHist[1], hHist[2]);
    Serial.printf("{\"temp_c\":%.1f,\"humidity_pct\":%.1f,\"valid\":true}\n", tMed, hMed);
  } else {
    Serial.println(F("{\"valid\":false,\"error\":\"dht_read_fail\"}"));
  }
  delay(2000);
}

