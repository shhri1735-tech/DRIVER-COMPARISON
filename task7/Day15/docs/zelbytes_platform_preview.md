# Zelbytes IoT Platform Architecture (Preview)

> **Status:** Planned — not live in current cohort.  
> Endpoints are subject to change. Monitor `careers.zelbytes.com` portal for announcements.

---

## Platform Data Flow

```
┌─────────────────────────────────────────────────────────────────────┐
│                        FIELD DEVICES                                │
│                                                                     │
│   ┌──────────────┐    ┌──────────────┐    ┌──────────────┐         │
│   │  Sensor Node │    │  Sensor Node │    │  Sensor Node │   ...   │
│   │  (ESP32 /    │    │  (ESP32 /    │    │  (ESP32 /    │         │
│   │   Arduino)   │    │   Arduino)   │    │   Arduino)   │         │
│   └──────┬───────┘    └──────┬───────┘    └──────┬───────┘         │
│          │                   │                   │                  │
│          └───────────────────┴───────────────────┘                  │
│                              │                                      │
│                    Device Token (JWT)                               │
└──────────────────────────────┼──────────────────────────────────────┘
                               │
               ┌───────────────┴───────────────┐
               │                               │
               ▼                               ▼
   ┌─────────────────────┐         ┌─────────────────────┐
   │   MQTT Broker       │         │   HTTP Ingest       │
   │                     │         │                     │
   │  mqtts://           │         │  POST               │
   │  careers.zelbytes   │         │  careers.zelbytes   │
   │  .com:8883          │         │  .com/api/v1/iot/   │
   │                     │         │  telemetry          │
   │  TLS + Device Token │         │  Bearer Token Auth  │
   └──────────┬──────────┘         └──────────┬──────────┘
              │                               │
              └───────────────┬───────────────┘
                              │
                              ▼
               ┌──────────────────────────┐
               │      STORAGE LAYER       │
               │                          │
               │  Time-series DB          │
               │  (Zelbytes backend)      │
               │                          │
               │  • Raw telemetry records │
               │  • Device registry       │
               │  • Token management      │
               └──────────────┬───────────┘
                              │
                              ▼
               ┌──────────────────────────┐
               │      DASHBOARD           │
               │                          │
               │  careers.zelbytes.com    │
               │                          │
               │  • Live sensor feeds     │
               │  • Historical charts     │
               │  • Device management     │
               │  • Token issuance        │
               └──────────────────────────┘
```

---

## Component Summary

| Layer | Technology | Endpoint / Location |
|---|---|---|
| Device Firmware | ESP32 / Arduino (C/C++) | On-device |
| MQTT Transport | MQTTS (TLS, port 8883) | `mqtts://careers.zelbytes.com:8883` |
| HTTP Transport | HTTPS REST | `POST https://careers.zelbytes.com/api/v1/iot/telemetry` |
| Auth | Per-device JWT or API token | Issued via careers portal |
| Storage | Zelbytes cloud backend | Managed |
| Dashboard | Web UI | `https://careers.zelbytes.com` |

---

## Authentication Model

Zelbytes plans **per-device JWT or API token** authentication (exact scheme TBD).

- Tokens are issued through the **careers portal**
- Each physical device carries a unique `DEVICE_TOKEN`
- Tokens are passed as:
  - **MQTT:** In the CONNECT packet password field
  - **HTTP:** As `Authorization: Bearer <DEVICE_TOKEN>` header

---

## Lab vs Production Comparison

| Config | Mosquitto Lab (current) | Zelbytes Production (planned) |
|---|---|---|
| MQTT Host | `localhost` or local IP | `careers.zelbytes.com` |
| MQTT Port | `1883` (plain) | `8883` (TLS) |
| HTTP URL | Local mock / none | `https://careers.zelbytes.com/api/v1/iot/telemetry` |
| Auth | None / basic | Device JWT or API token |
| TLS | No | Yes |
| Token Source | N/A | careers.zelbytes.com portal |

Switching from lab to production is a **config change only** — firmware uses `#ifdef ZELBYTES_PLATFORM` placeholders so no code rewrite is needed.
