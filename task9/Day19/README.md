# config_schema.md — Polyhouse Node NVS Configuration Schema

**NVS Namespace:** `polyhouse`  
**Storage:** ESP32 Non-Volatile Storage (Preferences library)  
**Updated:** via captive portal provisioning or MQTT commands

---

## Keys Reference

| Key           | Type   | Default | Valid Range / Notes                        |
|---------------|--------|---------|--------------------------------------------|
| `wifi_ssid`   | String | `""`    | Network name. Empty → provisioning mode.   |
| `wifi_pass`   | String | `""`    | ⚠️ Secret. Never printed in serial dump.   |
| `mqtt_broker` | String | `""`    | IP or hostname of MQTT broker.             |
| `node_id`     | String | `""`    | Unique node identifier e.g. `ph-node-01`. |
| `sample_iv`   | Int    | `60`    | Sensor sample interval in seconds (5–3600).|
| `cal_temp`    | Float  | `0.0`   | Temperature calibration offset in °C.      |
| `cal_hum`     | Float  | `0.0`   | Humidity calibration offset in %.          |

---

## Boot Behaviour

1. Load all keys from NVS namespace `polyhouse`.
2. If `wifi_ssid` or `mqtt_broker` is empty → enter **provisioning mode** (captive portal).
3. Otherwise connect to WiFi and MQTT using stored values.

---

## Update Methods

| Method               | Keys Updatable                        |
|----------------------|---------------------------------------|
| Captive portal       | All keys                              |
| MQTT `set_interval`  | `sample_iv`                           |
| USB serial import    | All non-secret keys (via `dump_config`)|

---

## Factory Reset

Hold **GPIO0** (BOOT button) for **5 seconds** at any time.  
All NVS keys are erased. Node reboots into provisioning mode.

---

## Security Notes

- `wifi_pass` is stored in plaintext NVS (ESP32 flash). Enable flash encryption for production.
- Serial `dump_config` command **never** prints `wifi_pass`.