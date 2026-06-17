# Day 8 — HTTP POST vs MQTT Latency Comparison

## Test Setup
- **Device:** ESP32 with fake DHT22 sensor readings
- **Network:** Wi-Fi (2.4GHz)
- **Server:** careers.zelbytes.com
- **Samples:** 5 readings, 60 seconds apart

---

## Results

### HTTP POST (Day 8)
| Sample | Latency (ms) |
|--------|-------------|
| 1      | ___ ms      |
| 2      | ___ ms      |
| 3      | ___ ms      |
| 4      | ___ ms      |
| 5      | ___ ms      |
| **Avg**| **___ ms**  |

### MQTT Publish (Day 12)
| Sample | Latency (ms) |
|--------|-------------|
| 1      | ___ ms      |
| 2      | ___ ms      |
| 3      | ___ ms      |
| 4      | ___ ms      |
| 5      | ___ ms      |
| **Avg**| **___ ms**  |

---

## Comparison Table

| Protocol  | Avg Latency | Connection Type      | Persistent Connection |
|-----------|-------------|----------------------|-----------------------|
| HTTP POST | ___ ms      | New TCP+TLS per call | ❌ No                 |
| MQTT      | ___ ms      | Broker via TCP       | ✅ Yes                |

---

## Why HTTP POST is Slower

Every HTTP POST call does this from scratch:
1. TCP handshake
2. TLS handshake (because HTTPS)
3. Send request
4. Wait for server response
5. Close connection

MQTT stays **permanently connected** to the broker, so publishing
is just sending a small packet — no handshake overhead each time.

---

## Conclusion

> MQTT is faster for repeated sensor publishing because it avoids
> the TCP+TLS setup cost on every message. HTTP POST is simpler to
> set up and works fine for low-frequency data (every 60s), but
> adds ~___ms of overhead per publish compared to MQTT.