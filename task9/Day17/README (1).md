# Day 21 — CoAP Protocol for ESP8266 Deep Sleep Nodes

## What This Session Is About
Learning how battery-powered ESP8266 sensor nodes in a polyhouse (smart greenhouse) should send their data using CoAP instead of MQTT — and understanding why it saves battery life.

---

## What I Learned

### CoAP Basics
- CoAP (Constrained Application Protocol) runs over **UDP**, not TCP
- Uses REST-like verbs: GET, POST, PUT — similar to HTTP but much lighter
- Designed for devices that sleep most of the time and wake up briefly to send data

### Three Message Types
| Type | Behaviour | Best For |
|------|-----------|----------|
| **CON** (Confirmable) | Waits for ACK, retries if no reply | Critical commands (e.g. open valve) |
| **NON** (Non-Confirmable) | Fire and forget, no ACK needed | Periodic sensor readings |
| **OBSERVE** | Server pushes updates to client | Real-time dashboard without a broker |

### CoAP vs MQTT — When to Use Which
- **CoAP** → battery nodes, no broker available, periodic bursts (our DHT22/soil sensors)
- **MQTT** → mains-powered nodes, many subscribers, always-on connection (CO2, valves, fan)

---

## Key Resource URI
```
GET coap://<device-ip>/sensors/temp  →  returns current DHT22 temperature (°C)
```

---

## Files in This Session
| File | Description |
|------|-------------|
| `CoAP_Overview.docx` | Full CoAP write-up with CON/NON/OBSERVE definitions and CoAP vs MQTT table |
| `CoAP_vs_MQTT_UseCase.docx` | Which polyhouse sensors use CoAP vs MQTT and why |
| `Power_Budget_CoAP_vs_MQTT.docx` | Battery life comparison table for 55s wake cycle on ESP8266 |

---

## Power Budget Summary
- CoAP saves ~330ms per wake cycle (no TCP connect, no broker handshake)
- Battery life on 2000 mAh: **CoAP ~14.2 days** vs **MQTT ~13.9 days**
- Small per-cycle saving adds up significantly over weeks of field deployment

---

## Optional (if time permits)
- [ ] Test CoAP GET against public server `coap.me` using Copper browser plugin or `libcoap` CLI
- [ ] Screenshot the response and add it to this folder

---

## Next Session
**Day 22 — Deep Sleep Timer Configuration**
Using `esp_deep_sleep_start()` in ESP-IDF to implement the 55-second wake cycle that this CoAP pattern depends on.
