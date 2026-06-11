# Zelbytes IoT Lab – Day 9: SensorBundle JSON Schema

## Overview
Designs and validates a structured `SensorBundle` JSON schema for IoT telemetry.
Serializes sensor data using ArduinoJson and POSTs to `/api/iot-lab/v1/telemetry`.
Schema is reused in MQTT publish (Day 12+) and the Day 22 capstone.

## Schema Fields
| Field | Type | Required | Description |
|---|---|---|---|
| schema_version | integer | ✅ | Always `1` for this version |
| node_id | string | ✅ | Unique device identifier |
| ts | integer | ✅ | Unix timestamp (seconds) |
| temp_c | float | ✅ | Temperature in Celsius |
| humidity_pct | float | ✅ | Humidity percentage (0–100) |
| time_valid | boolean | ✅ | True if RTC/NTP time is synced |
| fw_version | string | ✅ | Firmware version string |

## Example Payload
```json
{
  "schema_version": 1,
  "node_id": "node_001",
  "ts": 1718000000,
  "temp_c": 28.45,
  "humidity_pct": 62.3,
  "time_valid": true,
  "fw_version": "1.0.0"
}
```

## Constraints
- Payload must be **under 512 bytes** (PubSubClient MQTT buffer limit)
- Compact JSON only — no pretty-printing
- All fields required — incomplete bundles are rejected before upload

## Dependencies
- [ArduinoJson](https://arduinojson.org/) v6.x by Benoit Blanchon
  - Install via Arduino Library Manager

## How to Run
1. Open project in Arduino IDE
2. Install ArduinoJson v6.x from Library Manager
3. Flash to your ESP32/Arduino board
4. Open Serial Monitor at **115200 baud**
5. Observe round-trip test and validation test results

## Serial Output (Expected)