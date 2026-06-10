# ESP32 OTA + Deep Sleep

ESP32 project with WiFi, OTA updates, and deep sleep after 30 seconds.

## Setup
1. Clone the repo
2. Copy `secrets.h.example` → `secrets.h`
3. Fill in your WiFi credentials
4. Flash via USB first, then use OTA for future uploads

## OTA
- Hostname: `esp32-zelbytes-lab`
- Port: 3232

## Dependencies
- WiFi.h (built-in)
- ArduinoOTA.h (built-in)

## Deep Sleep
- Stays awake 30 seconds after boot
- Sleeps for 10 seconds (configurable)
