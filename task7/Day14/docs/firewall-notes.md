# Firewall & NAT Notes

**MQTT vs HTTPS — Port Behavior in Lab vs Production**

---

## Port Reference

| Protocol | Default Port | Secure Port | Transport |
|----------|-------------|-------------|-----------|
| HTTP | 80 | 443 (HTTPS) | TCP |
| MQTT | 1883 | 8883 (MQTT+TLS) | TCP |
| CoAP | 5683 | 5684 (CoAP+DTLS) | UDP |

---

## Lab Environment (Zelbytes IoT Lab)

### Ports Open in Lab

| Port | Protocol | Status | Notes |
|------|----------|--------|-------|
| 443 | HTTPS | ✅ Open | Standard web traffic; always allowed |
| 1883 | MQTT (plain) | ✅ Open | Lab router allows outbound; no enterprise firewall |
| 8883 | MQTT+TLS | ✅ Open | Preferred even in lab for TLS habit |
| 5683 | CoAP (UDP) | ✅ Open | Lab has no UDP blocking |
| 5684 | CoAP+DTLS (UDP) | ✅ Open | Same as above |

**Lab conclusion:** All protocols work freely. Ideal for benchmarking and learning without firewall interference.

---

## Production Farm Environment

### Ports Open in Production

| Port | Protocol | Status | Notes |
|------|----------|--------|-------|
| 443 | HTTPS | ✅ Open | Always open — standard web traffic |
| 1883 | MQTT (plain) | ❌ Often Blocked | Most enterprise/ISP firewalls block non-standard TCP ports |
| 8883 | MQTT+TLS | ⚠️ Sometimes Blocked | Better than 1883; some farms allow this |
| 5683 | CoAP (UDP) | ❌ Blocked | UDP non-80/443 almost always blocked by ISP/router NAT |
| 5684 | CoAP+DTLS (UDP) | ❌ Blocked | Same issue as CoAP plain |

**Production conclusion:**
- **Only HTTPS (443) is guaranteed to work** across all farm network configurations
- MQTT requires broker placement in DMZ or use of WebSocket tunneling on port 443 (`MQTT over WSS`)
- CoAP requires private APN, VPN, or LoRaWAN gateway — not practical over public internet without UDP passthrough

---

## NAT Behavior

### MQTT over NAT
- MQTT uses persistent TCP connection from ESP32 → broker
- NAT table keeps the TCP session alive as long as keepalive pings flow
- Default ESP32 `PubSubClient` keepalive: **15 seconds** — increase to 60s to reduce traffic
- If NAT drops idle TCP sessions (common after ~5 min), node must reconnect — handle with `reconnect()` loop

```cpp
// ESP32 reconnect pattern
void reconnectMQTT() {
  while (!client.connected()) {
    if (client.connect("esp32-node-01")) {
      client.subscribe("zelbytes/commands/#");
    } else {
      delay(5000);
    }
  }
}
```

### HTTP over NAT
- Stateless — each request opens a new TCP connection
- NAT has no session to maintain between requests — no reconnect logic needed
- HTTPS on 443 passes through virtually all NAT without issues

### CoAP over NAT
- UDP is connectionless — NAT has no session to track
- Many NAT devices drop UDP state after 30–60 seconds of inactivity
- Sleepy CoAP nodes must re-establish path after sleep > NAT timeout
- Solution: use CoAP observe with short max-age, or switch to MQTT for command path

---

## MQTT Broker Placement Options for Production

| Option | Pros | Cons |
|--------|------|------|
| Cloud broker (HiveMQ, EMQX Cloud) | No infra management, TLS built-in | Monthly cost, latency |
| Self-hosted in DMZ | Full control, private data | Requires IT setup, open 8883 inbound |
| MQTT over WSS (port 443) | Works through any firewall | Slight overhead, broker must support WebSockets |
| MQTT via VPN | Most secure | Requires VPN client on each node |

**Zelbytes recommendation:** Use cloud broker (EMQX/HiveMQ) on port 8883 (TLS). Fall back to MQTT over WSS on 443 if 8883 is blocked.

---

## Summary: Which Protocol Where

| Scenario | Best Protocol | Why |
|----------|--------------|-----|
| Lab development / testing | Any | All ports open |
| Farm on home/consumer router | MQTT over 8883 or HTTPS | NAT-friendly TCP |
| Farm on enterprise/ISP network | HTTPS or MQTT over WSS/443 | Only 443 guaranteed |
| Private farm LAN (no internet) | MQTT plain 1883 | No firewall, low overhead |
| Battery node, private LAN | CoAP | UDP fine on LAN, no NAT issue |

---

*Zelbytes IoT Internship — Day 14*
