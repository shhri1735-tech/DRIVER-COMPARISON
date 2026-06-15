Zelbytes IoT Lab - Node 89
MQTT Bidirectional Control | Day 13 Exercise

This project is part of the Zelbytes Industrial IoT Learning Platform.
It implements bidirectional MQTT communication on an ESP8266/ESP32 microcontroller.

What it does:
- Connects to WiFi and the Zelbytes MQTT broker
- Publishes sensor telemetry (temperature, humidity, soil moisture) every 60 seconds
- Subscribes to remote commands from the cloud dashboard
- Parses incoming JSON commands and executes safe actions
- Publishes an ACK response after every command
- Rejects unknown commands with an error ACK
- Ignores duplicate commands using an idempotency table

Commands supported:
- set_interval : changes the telemetry sampling rate
- trigger_misting : simulates misting relay (serial log only, lab safe)
- reboot_node : restarts the microcontroller

MQTT Topics:
- Telemetry  : zelbytes/lab/89/telemetry          (publish, QoS 0)
- Commands   : zelbytes/polyhouse/89/command       (subscribe, QoS 1)
- ACK        : zelbytes/polyhouse/89/command/ack   (publish, QoS 1)

Hardware:
- ESP8266 (NodeMCU) or ESP32
- Optional: DHT11/DHT22 and soil moisture sensor

Libraries used:
- PubSubClient by Nick O'Leary
- ArduinoJson by Benoit Blanchon

How to use:
1. Open zelbytes_node89.ino in Arduino IDE
2. Enter your WiFi name, WiFi password, and Zelbytes API key at the top
3. Install the two libraries from Library Manager
4. Select your board and upload
5. Open Serial Monitor at 115200 baud to see logs

Lab checklist completed:
- Subscribe succeeds and triggers serial handler
- set_interval command changes the interval variable
- ACK published to command/ack topic
- Duplicate command_id ignored
- Unknown command rejected with error ACK

Author: Your Name
Platform: Zelbytes Industrial IoT Learning Platform
Node ID: 89