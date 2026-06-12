# Polling Interval: 30 seconds
Chosen to balance data freshness vs. battery/bandwidth usage.

## HTTP Polling (what we use today)
- ESP32 sends GET request every 30–60s
- Simple to implement, works through firewalls
- Latency: up to 30–60s behind real-time
- Battery impact: moderate (radio active per request)

## MQTT Push (Days 12–13)
- Broker pushes data instantly when available
- Latency: near real-time (< 1s)
- Battery impact: lower (persistent lightweight connection)
- Better for time-critical commands (e.g. emergency irrigation stop)

## When to prefer MQTT over HTTP poll:
- Real-time alerts needed (flood sensor, fire alarm)
- Many devices publishing frequently
- Battery-constrained devices
- Two-way command delivery