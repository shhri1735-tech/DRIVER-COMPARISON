# SensorBundle v1 → Zelbytes API Field Mapping

**Status:** Draft spec — API contract is not yet finalised.  
**Schema version:** `v1`  
**Target endpoint:** `POST https://careers.zelbytes.com/api/v1/iot/telemetry`

---

## Day 8 SensorBundle JSON Schema (v1)

```json
{
  "device_id":    "esp32-001",
  "timestamp":    1720000000,
  "schema":       "v1",
  "temperature":  24.5,
  "humidity":     60.2,
  "pressure":     1013.25,
  "light":        512,
  "motion":       false,
  "battery_mv":   3700,
  "rssi":         -67
}
```

---

## Field Mapping Table

| SensorBundle v1 Field | Type | Unit | Zelbytes API Field (planned) | API Type | Notes |
|---|---|---|---|---|---|
| `device_id` | string | — | `device.id` | string | Matches the token's registered device ID |
| `timestamp` | integer | Unix epoch (s) | `timestamp` | ISO 8601 string | Convert: `new Date(ts*1000).toISOString()` |
| `schema` | string | — | `meta.schema_version` | string | Sent as `"v1"` |
| `temperature` | float | °C | `sensors.temperature_c` | float | No unit change |
| `humidity` | float | % RH | `sensors.humidity_pct` | float | No unit change |
| `pressure` | float | hPa | `sensors.pressure_hpa` | float | No unit change |
| `light` | integer | raw ADC | `sensors.light_raw` | integer | API may later expect lux — TBD |
| `motion` | boolean | — | `sensors.motion_detected` | boolean | No change |
| `battery_mv` | integer | mV | `device.battery_mv` | integer | Nested under `device` object |
| `rssi` | integer | dBm | `device.rssi_dbm` | integer | Nested under `device` object |

---

## Transformed Payload (Zelbytes API format, planned)

```json
{
  "device": {
    "id":          "esp32-001",
    "battery_mv":  3700,
    "rssi_dbm":   -67
  },
  "timestamp": "2024-07-03T12:26:40.000Z",
  "meta": {
    "schema_version": "v1"
  },
  "sensors": {
    "temperature_c":    24.5,
    "humidity_pct":     60.2,
    "pressure_hpa":     1013.25,
    "light_raw":        512,
    "motion_detected":  false
  }
}
```

---

## HTTP Request Template (planned)

```http
POST /api/v1/iot/telemetry HTTP/1.1
Host: careers.zelbytes.com
Authorization: Bearer <DEVICE_TOKEN>
Content-Type: application/json

{ ...payload above... }
```

Expected success response:
```json
{ "status": "ok", "received": "2024-07-03T12:26:40.000Z" }
```

---

## Fields Not Yet Confirmed

| Item | Question |
|---|---|
| `light` unit | Will API expect raw ADC or calibrated lux? |
| `timestamp` format | Epoch int or ISO 8601 string? |
| Batch ingest | Can multiple records be sent in one POST as an array? |
| API versioning | Will `v1` be stable or bump to `v2` before launch? |
| Error codes | 4xx/5xx schema for invalid token or malformed payload? |

> Track answers in the careers portal announcements or ask your mentor.
