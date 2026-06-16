# Day 14 – IoT Protocol Comparison: HTTP vs MQTT vs CoAP

**Zelbytes IoT Internship | Polyhouse Fleet Protocol Analysis**

---

## Overview

Polyhouse sensor nodes at Zelbytes use a mix of protocols depending on power source, data type, and network constraints:

| Use Case | Protocol |
|----------|----------|
| DHT22 telemetry every 60s | MQTT |
| Bulk log / config upload | HTTPS POST |
| Future soil moisture on CR2032 | CoAP (Day 17) |

This session compares **HTTP REST**, **MQTT**, and **CoAP** across latency, overhead, battery impact, firewall traversal, and developer ergonomics — and produces a decision matrix for choosing the right protocol per node tier.

---

## Node Tier Recommendations

### 🔌 Mains-Powered ESP32 (e.g., Zelbytes IoT Lab nodes)
**→ Use MQTT**
- Persistent TCP session eliminates per-message handshake overhead
- Low latency for live telemetry and remote commands
- Broker (port 1883 / 8883 TLS) handles fan-out to multiple subscribers
- Developer-friendly: Arduino `PubSubClient` library, easy to debug

### 🔋 Battery-Powered Sensor (CR2032 / LiPo)
**→ Use CoAP**
- UDP-based: no TCP handshake, minimal radio-on time
- Fits in a single UDP datagram — ideal for sleepy nodes
- DTLS optional for security without TLS overhead
- Designed for constrained devices (RFC 7252)

### 🌐 Config / Firmware / Log Upload
**→ Use HTTPS POST**
- Firewall-friendly (port 443, universally open)
- Large payload support, standard REST tooling
- No persistent connection needed for infrequent transfers

---

## Files in This Repo

| File | Description |
|------|-------------|
| `decision-matrix.md` | 4-criteria × 3-protocol comparison table |
| `payload-bytes-analysis.md` | Byte estimates for one DHT22 telemetry event per protocol |
| `firewall-notes.md` | Port behavior, lab vs production firewall notes |
| `README.md` | This file — summary + node tier recommendations |

---

## Connection to Day 15

The Zelbytes platform (introduced Day 15) exposes an **HTTP ingest endpoint** that complements the MQTT broker. Rule of thumb:

- **MQTT broker** → real-time telemetry, commands, dashboard feeds
- **HTTP ingest** → batch uploads, third-party integrations, webhook triggers

The decision matrix in this repo will guide which path each node tier uses when onboarding to the platform.

---

*Part of the Zelbytes Polyhouse IoT Internship Program*
