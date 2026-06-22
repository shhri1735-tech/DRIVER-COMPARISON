# ESP32 IoT Capstone Node (Node ID: 89)

A production-ready ESP32 firmware implementation utilizing a DHT22 sensor to securely collect environmental data, transmit structured telemetry via HTTPS, parse remote commands, and optimize power management using Non-Volatile Storage (NVS) and Deep Sleep.

---

## 🚀 Features

* **Wi-Fi Connectivity & NTP Sync:** Automated network attachment and synchronization with NTP servers for precise ISO 8601 timestamps.
* **Secure Telemetry Streaming:** Transmits real-time Temperature and Humidity JSON payloads via HTTPS POST to the Zelbytes IoT gateway.
* **Dynamic Remote Command Processing:** Fetches down-link configurations directly from the platform edge (`set_interval`, `IRRIGATE`, `STOP`, `reboot`).
* **Persistent Configuration (NVS):** Stores vital state variables—such as custom reporting intervals—directly in onboard flash memory so parameters persist across power cycles.
* **Ultra-Low Power Optimization:** Leverages ESP32 hardware deep-sleep cycles to conserve power between configured sensor broadcasts.
* **MQTT-Style Bridge Signaling:** Fires downstream ACK payloads back to the platform using specialized HTTP-to-MQTT bridge topics[cite: 1].

---

## 🛠 Hardware Configuration

| Component | ESP32 Pin Assignment | Description |
| :--- | :--- | :--- |
| **ESP32 Development Board** | — | Core MCU[cite: 1] |
| **DHT22 Sensor VCC** | `3.3V` | Device Power Supply |
| **DHT22 Sensor Data** | `GPIO 4` | Configured Digital I/O Pin |
| **DHT22 Sensor GND** | `GND` | Common Ground Reference |

> ⚠️ **Note:** Ensure a $4.7\text{k}\Omega$ to $10\text{k}\Omega$ pull-up resistor is installed between the DHT22 Data and VCC pins if your sensor breakout board does not include one.

---

## 📡 API & Platform Overview

This device interacts natively with the **Zelbytes IoT Platform**:

* **API Base URL:** `https://careers.zelbytes.com/api/iot-lab/v1`[cite: 1]
* **Target Telemetry Topic:** `zelbytes/lab/89/telemetry`[cite: 1]
* **Target Acknowledgment Topic:** `zelbytes/lab/89/ack`

### Structured JSON Telemetry Payload Schema Example
```json
{
  "node_id": "89",
  "topic": "zelbytes/lab/89/telemetry",
  "timestamp": "2026-06-22T15:30:00Z",
  "data": {
    "temperature": 24.5,
    "humidity": 62.1
  }
}