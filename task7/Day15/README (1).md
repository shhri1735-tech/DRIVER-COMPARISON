# Zelbytes IoT Capstone — Day 8

This repository contains firmware stubs, configuration headers, and documentation for the Zelbytes IoT platform integration capstone.

---

## ⚠️ Endpoints Not Yet Live

> **Zelbytes platform integration is not active in the current cohort.**  
> The planned endpoints below are subject to change.  
> **Monitor [careers.zelbytes.com](https://careers.zelbytes.com) portal announcements for go-live updates.**

---

## Planned Endpoints (Subject to Change)

| Transport | Endpoint |
|---|---|
| HTTP Ingest | `POST https://careers.zelbytes.com/api/v1/iot/telemetry` |
| MQTT Broker | `mqtts://careers.zelbytes.com:8883` |

Authentication: per-device JWT or API token, issued via the careers portal.

---

## Repository Structure

```
zelbytes-capstone/
├── include/
│   └── zelbytes_config.h          # Platform config with #ifdef ZELBYTES_PLATFORM guards
├── docs/
│   ├── zelbytes_platform_preview.md   # Architecture diagram (ASCII) + component summary
│   ├── sensor_bundle_api_mapping.md   # SensorBundle v1 → Zelbytes API field mapping
│   └── migration_checklist.md         # Step-by-step Mosquitto lab → production MQTTS
└── README.md
```

---

## Quick Start

### Lab Mode (default — no Zelbytes account needed)

Compile normally. `ZELBYTES_PLATFORM` is **undefined**, so the firmware targets your local Mosquitto broker on port `1883` with no authentication.

```bash
# PlatformIO example — no extra flags needed
pio run
```

### Production Mode (when endpoints go live)

1. Get your `DEVICE_TOKEN` from the careers portal.
2. Add build flags:

```ini
# platformio.ini
build_flags =
    -D ZELBYTES_PLATFORM
    -D DEVICE_TOKEN=\"your_token_here\"
```

3. Flash and monitor. Check the serial output for a successful MQTT `CONNACK`.

> **Never commit a real token to source control.** Use environment variables or a secrets manager.

---

## Key Configuration Variables

| Macro | Lab Value | Production Value |
|---|---|---|
| `ZELBYTES_HTTP_URL` | `http://localhost:8080/telemetry` | `https://careers.zelbytes.com/api/v1/iot/telemetry` |
| `ZELBYTES_MQTT_HOST` | `localhost` | `careers.zelbytes.com` |
| `ZELBYTES_MQTT_PORT` | `1883` | `8883` |
| `DEVICE_TOKEN` | `""` | Issued from careers portal |

All values are defined in `include/zelbytes_config.h`.

---

## Documentation

- **Architecture diagram:** `docs/zelbytes_platform_preview.md`
- **API field mapping:** `docs/sensor_bundle_api_mapping.md`
- **Migration checklist:** `docs/migration_checklist.md`

---

## Session Checklist (Day 8)

- [x] Architecture diagram saved to `docs/zelbytes_platform_preview.md`
- [x] Placeholder headers compile with `ZELBYTES_PLATFORM` undefined
- [x] SensorBundle field mapping table to future API spec draft
- [x] Migration checklist from lab Mosquitto to production MQTTS
- [x] README notes: endpoints coming to `careers.zelbytes.com` — monitor portal announcements

---

## Notes

- Switching from Mosquitto lab to Zelbytes production is a **config change, not a code rewrite** — `#ifdef ZELBYTES_PLATFORM` handles all branching.
- The `light` field unit (raw ADC vs lux) and exact `timestamp` format (epoch vs ISO 8601) are unconfirmed — track in portal announcements.
- Refer to `docs/migration_checklist.md` for the full step-by-step when endpoints go live.
