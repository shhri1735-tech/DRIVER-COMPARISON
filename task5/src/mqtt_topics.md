# MQTT Topic Namespace — Zelbytes Polyhouse Fleet

## Topic Structure
zelbytes/polyhouse/{node_id}/{type}

## Topic Tree

zelbytes/
└── polyhouse/
    ├── node001/
    │   ├── telemetry     ← sensor readings (temp, humidity)
    │   ├── command       ← actuator commands (irrigate, fan)
    │   └── status        ← online / offline (LWT + retained)
    ├── node002/
    │   ├── telemetry
    │   ├── command
    │   └── status
    ├── node003/
    │   ├── telemetry
    │   ├── command
    │   └── status
    └── broadcast/
        └── command       ← send command to ALL nodes at once

## Wildcard Subscriptions

| Pattern                          | Matches                              |
|----------------------------------|--------------------------------------|
| zelbytes/polyhouse/+/telemetry   | All node telemetry only              |
| zelbytes/polyhouse/+/command     | All node commands only               |
| zelbytes/polyhouse/node001/#     | Everything from node001              |
| zelbytes/#                       | Entire fleet (logging/debug only)    |

## Notes
- `+` matches exactly one level (single node)
- `#` matches everything below (use carefully — high traffic)


# Broker vs Client — My Explanation

## Broker (Mosquitto)
The broker is the central message router. It does not produce or 
consume data — it only receives messages from publishers and forwards 
them to the right subscribers based on topic matching. It runs 24/7 
as a server (e.g. on a Raspberry Pi or cloud VM). It also stores 
retained messages and fires LWT payloads when clients drop.

## Client (ESP32, dashboard, scripts)
Any device that connects TO the broker. A client can be a publisher 
(sends data), a subscriber (receives data), or both at the same time.
The ESP32 in our polyhouse is a publisher — it sends sensor readings.
The dashboard is a subscriber — it listens for those readings.
Clients never talk directly to each other.

## Key difference
  Broker  = post office (routes letters, stores undelivered ones)
  Client  = person (sends or receives letters through the post office)