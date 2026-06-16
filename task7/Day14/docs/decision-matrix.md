# Protocol Decision Matrix

**Zelbytes Polyhouse IoT — HTTP REST vs MQTT vs CoAP**

---

## Decision Matrix (4 Criteria × 3 Protocols)

| Criteria | HTTP REST | MQTT | CoAP |
|----------|-----------|------|------|
| **Latency** | High — new TCP+TLS handshake per request (~200–500 ms cold) | Low — persistent TCP session, publish in ~10–50 ms | Very Low — single UDP datagram, ~5–20 ms |
| **Bytes on Wire (per event)** | ~500–1200 bytes (HTTP headers + JSON body) | ~50–100 bytes (fixed 2-byte header + topic + payload) | ~20–50 bytes (4-byte fixed header + payload, binary optional) |
| **Battery Impact** | High — radio stays on for full TCP lifecycle per request | Medium — radio on for keepalive pings, off between publishes | Very Low — radio on only for one UDP burst; sleepy node support |
| **Firewall / NAT Traversal** | Easy — port 443 (HTTPS) universally open | Moderate — port 1883 (plain) often blocked; 8883 (TLS) better; may need broker in DMZ | Hard — UDP port 5683 often blocked; DTLS on 5684; needs UDP passthrough or proxy |
| **Developer Ergonomics** | Excellent — curl, Postman, standard REST libraries everywhere | Good — PubSubClient (Arduino), Mosquitto, MQTT Explorer; topic design needs planning | Fair — fewer libraries; `libcoap`, `aiocoap`; debugging harder |
| **Connection Model** | Stateless request/response | Stateful persistent session (broker-mediated) | Stateless request/response over UDP (with optional confirmable messages) |
| **Transport** | TCP (reliable) | TCP (reliable) | UDP (unreliable by default; confirmable mode adds ACK) |

---

## Zelbytes Use-Case Mapping

| Scenario | Recommended Protocol | Reason |
|----------|----------------------|--------|
| DHT22 temp/humidity every 60 s | **MQTT** | Low overhead, persistent session, broker handles fan-out |
| Bulk log upload to server | **HTTPS POST** | Firewall-safe, large payload, infrequent |
| Soil moisture on CR2032 (Day 17) | **CoAP** | Sleepy node, minimal radio-on, UDP fits in one datagram |
| OTA firmware update | **HTTPS** | Reliable TCP, port 443 open, standard tooling |
| Real-time actuator commands | **MQTT** | Sub-second latency, QoS guarantees via broker |

---

## When CoAP Beats MQTT

CoAP is the better choice when:
- Node runs on coin-cell battery (CR2032) — UDP minimizes radio-on time
- Payload fits in ~1 UDP datagram (< 1280 bytes after DTLS overhead)
- Network supports UDP (local LAN or private APN)
- Multicast needed (CoAP supports UDP multicast; MQTT does not)
- Node spends most of its time asleep (sleepy node / low-power mode)

---

## When HTTP Complements MQTT (Zelbytes Platform Context)

The Zelbytes HTTP ingest endpoint (see Day 15) is NOT a replacement for MQTT — it fills specific gaps:

- Third-party systems that can only speak REST
- Webhook-triggered data pushes
- One-time bulk historical uploads
- Integration with cloud services (AWS API Gateway, GCP Pub/Sub via HTTP)

> Rule: **MQTT for real-time telemetry → HTTP for everything else that doesn't need a persistent connection.**

---

*Zelbytes IoT Internship — Day 14*
