#include <DHT.h>

// ── Pin & sensor config ──────────────────────────────────
#define DHTPIN    4          // GPIO 4 (data line with 10kΩ pull-up)
#define DHTTYPE   DHT22
DHT dht(DHTPIN, DHTTYPE);

// ── SensorBundle: holds one reading + validity flag ──────
struct SensorBundle {
  float temperature;   // degrees Celsius
  float humidity;      // percent RH
  bool  valid;         // false if checksum failed or NaN returned
};

// ── Median filter storage (last 3 good samples) ──────────
const int FILTER_SIZE = 3;
float tempHistory[FILTER_SIZE] = {0, 0, 0};
float humHistory[FILTER_SIZE]  = {0, 0, 0};
int   histIndex = 0;
int   histCount = 0;   // tracks how many valid samples we have so far

// ── Read with retry (up to maxTries attempts) ────────────
SensorBundle readSensor(int maxTries = 3) {
  SensorBundle result = {0.0, 0.0, false};

  for (int attempt = 1; attempt <= maxTries; attempt++) {
    delay(2100);  // DHT22 needs 2 s minimum between reads

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // isnan() catches NaN (failed reads); dht.getStatus() catches checksum errors
    if (isnan(t) || isnan(h)) {
      Serial.print("[WARN] Read failed on attempt ");
      Serial.println(attempt);
      continue;   // retry
    }

    // Plausibility check: reject physically impossible values
    if (t < -40 || t > 80 || h < 0 || h > 100) {
      Serial.print("[WARN] Out-of-range on attempt ");
      Serial.println(attempt);
      continue;
    }

    // Valid reading obtained
    result.temperature = t;
    result.humidity    = h;
    result.valid       = true;
    break;
  }

  if (!result.valid) {
    Serial.println("[ERROR] All attempts failed — skipping cycle.");
  }

  return result;
}

// ── Helper: median of 3 floats ───────────────────────────
float median3(float a, float b, float c) {
  if ((a <= b && b <= c) || (c <= b && b <= a)) return b;
  if ((b <= a && a <= c) || (c <= a && a <= b)) return a;
  return c;
}

// ── Apply median filter; returns filtered value ──────────
float applyMedian(float* history, float newVal) {
  history[histIndex % FILTER_SIZE] = newVal;
  if (histCount < FILTER_SIZE) return newVal;  // not enough history yet
  return median3(history[0], history[1], history[2]);
}

// ── Setup ────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.println("[INFO] DHT22 starting — waiting 2 s for sensor warm-up...");
  delay(2000);  // sensor warm-up time
  Serial.println("[INFO] Ready.");
}

// ── Main loop ────────────────────────────────────────────
void loop() {
  SensorBundle reading = readSensor(3);  // up to 3 retries

  if (reading.valid) {
    // Store in history and get filtered values
    float filteredTemp = applyMedian(tempHistory, reading.temperature);
    float filteredHum  = applyMedian(humHistory,  reading.humidity);

    histIndex++;
    histCount++;

    // Print structured output matching future JSON field names
    Serial.print("{");
    Serial.print("\"temperature_c\":");  Serial.print(filteredTemp, 2);
    Serial.print(",\"humidity_pct\":");  Serial.print(filteredHum, 2);
    Serial.print(",\"raw_temp\":");      Serial.print(reading.temperature, 2);
    Serial.print(",\"raw_hum\":");       Serial.print(reading.humidity, 2);
    Serial.print(",\"filtered\":");      Serial.print(histCount >= FILTER_SIZE ? "true" : "false");
    Serial.println("}");
  }

  // No delay here — readSensor() already waits 2.1 s internally per attempt
}
