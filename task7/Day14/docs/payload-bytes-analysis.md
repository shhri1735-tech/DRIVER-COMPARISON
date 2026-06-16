# Payload Bytes Analysis

**One DHT22 Telemetry Event — Bytes on Wire per Protocol**

---

## Sample JSON Payload

This is the actual JSON body sent from an ESP32 DHT22 node:

```json
{"device":"esp32-node-01","sensor":"DHT22","temp":28.5,"hum":65.2,"ts":1718000000}
```

**Raw JSON size: ~80 bytes**

---

## Protocol-by-Protocol Breakdown

### 1. HTTP POST (HTTPS on port 443)

```
POST /api/v1/telemetry HTTP/1.1
Host: iot.zelbytes.com
Content-Type: application/json
Content-Length: 80
Authorization: Bearer <token>

{"device":"esp32-node-01","sensor":"DHT22","temp":28.5,"hum":65.2,"ts":1718000000}
```

| Component | Bytes |
|-----------|-------|
| HTTP request line | ~35 |
| Host header | ~25 |
| Content-Type header | ~31 |
| Content-Length header | ~20 |
| Authorization header | ~50 (JWT token) |
| Blank line | 2 |
| JSON body | ~80 |
| **HTTP total** | **~243 bytes** |
| TLS 1.3 handshake (one-time, cold) | ~3000–5000 bytes |
| TCP SYN/SYN-ACK/ACK | ~180 bytes |
| **Total cold (first request)** | **~3500–5500 bytes** |
| **Total warm (keep-alive reuse)** | **~250–300 bytes** |

> ⚠️ On battery nodes without keep-alive, every publish pays the full cold cost.

---

### 2. MQTT PUBLISH (port 1883 / 8883)

Topic: `zelbytes/polyhouse/node01/dht22`  
QoS: 1 (at least once)

```
MQTT Fixed Header:    2 bytes
Packet Identifier:    2 bytes  (QoS 1)
Topic length field:   2 bytes
Topic string:        38 bytes  (zelbytes/polyhouse/node01/dht22)
Payload (JSON):      80 bytes
─────────────────────────────
PUBLISH total:      124 bytes
PUBACK response:      4 bytes
```

| Component | Bytes |
|-----------|-------|
| CONNECT (one-time, session start) | ~50–80 bytes |
| PUBLISH message | ~124 bytes |
| PUBACK (QoS 1 ACK) | 4 bytes |
| **Per-event cost (after connect)** | **~128 bytes** |
| **Cold start (including CONNECT)** | **~210 bytes** |

> ✅ Persistent session means CONNECT is paid once; subsequent publishes are ~128 bytes each.

---

### 3. CoAP PUT (UDP port 5683)

URI: `coap://iot.zelbytes.com/telemetry/node01`

```
CoAP Fixed Header:    4 bytes
Token:                4 bytes
URI-Path options:    ~25 bytes
Content-Format opt:   2 bytes
Payload Marker:       1 byte
Payload (JSON):      80 bytes
─────────────────────────────
CoAP total:         116 bytes
```

| Component | Bytes |
|-----------|-------|
| CoAP message (CON type) | ~116 bytes |
| ACK response | 4 bytes |
| **Per-event cost** | **~120 bytes** |
| **No session setup overhead** | **0 bytes** |

> ✅ Best for sleepy nodes: one UDP datagram in, one ACK back, radio off.

---

## Summary Comparison

| Protocol | Per-Event Bytes | Cold Start Cost | Persistent Session? |
|----------|----------------|-----------------|---------------------|
| HTTP POST | ~250 bytes | ~5000 bytes (TLS) | Optional (keep-alive) |
| MQTT (QoS 1) | ~128 bytes | ~210 bytes | Yes (broker) |
| CoAP (CON) | ~120 bytes | 0 bytes | No (stateless UDP) |

---

## Battery Impact Estimate (CR2032 node, 1 event/60s)

Assuming ESP32 radio-on current ~80 mA:

| Protocol | Radio-On Time per Event | Daily Energy (1 event/min) |
|----------|------------------------|---------------------------|
| HTTP (cold) | ~800 ms (TCP+TLS+transfer) | ~92 mAs |
| MQTT | ~50 ms (publish+PUBACK) | ~5.8 mAs |
| CoAP | ~20 ms (UDP burst+ACK) | ~2.3 mAs |

> CoAP uses **~40× less radio energy** than cold HTTP for the same payload.

---

*Zelbytes IoT Internship — Day 14*
