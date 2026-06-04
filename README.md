DHT22 → ESP32
VCC   → 3.3V
DATA  → GPIO 4
GND   → GND

## Tools & Environment

- **IDE:** Arduino IDE 2.x
- **Board:** ESP32 Dev Module
- **Upload Speed:** 115200
- **Flash Mode:** DIO
- **Partition Scheme:** Default 4MB

## References

- [Espressif ESP32 Datasheet](https://www.espressif.com/en/products/socs/esp32)
- [Arduino ESP32 Core](https://github.com/espressif/arduino-esp32)
- [DHT22 Sensor Datasheet](https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf)

- # ESP32 Node - Driver Comparison & Setup

## Board Decision: Why I Chose ESP32 over ESP8266

After comparing both boards, I chose the **ESP32** for my IoT node project.

## ESP8266 vs ESP32 Comparison

| Feature         | ESP8266        | ESP32              | Winner    |
|-----------------|----------------|--------------------|-----------|
| CPU Cores       | 1 (80/160 MHz) | 2 (up to 240 MHz)  | ESP32 ✅  |
| RAM             | ~80 KB         | ~520 KB            | ESP32 ✅  |
| Flash Memory    | 1–4 MB         | 4–16 MB            | ESP32 ✅  |
| GPIO Pins       | 11 usable      | 30+                | ESP32 ✅  |
| Wi-Fi           | ✅ Yes          | ✅ Yes              | Tie       |
| Bluetooth       | ❌ No           | ✅ Yes (BLE + BT)  | ESP32 ✅  |
| ADC Channels    | 1              | 18                 | ESP32 ✅  |
| Price           | ~$2–3          | ~$4–6              | ESP8266   |
| DHT22 Support   | ✅ Yes          | ✅ Yes              | Tie       |
| Arduino Support | ✅ Yes          | ✅ Yes              | Tie       |

## Reasons for Choosing ESP32

- **Dual-core processor** handles sensor reading and Wi-Fi simultaneously
- **More GPIO pins** gives flexibility for future sensors
- **Bluetooth (BLE)** support opens up more connectivity options
- **More RAM** means stable performance with larger sketches
- **Future-proof** for expanding the project

## USB Driver Installed

| Driver  | Chip    | Used For              |
|---------|---------|-----------------------|
| CP210x  | CP2102  | Silicon Labs based boards |
| CH340   | CH340G  | Most budget ESP32 boards  |

> Check your board label to confirm which chip it uses.

## Board Setup Checklist

- [x] Arduino core for ESP32 (Espressif) installed via Boards Manager
- [x] USB serial driver installed (CP210x or CH340)
- [x] Blink sketch uploads without errors
- [x] Serial monitor shows board identity lines
- [x] Pin map documented (DHT22)
- [x] Git repo initialized with README

## DHT22 Pin Map (ESP32)

| DHT22 Pin | ESP32 Pin | Notes                        |
|-----------|-----------|------------------------------|
| VCC       | 3.3V      | Do NOT connect to 5V         |
| DATA      | GPIO4     | Add 10kΩ pull-up to 3.3V    |
| GND       | GND       | Common ground                |

### Wiring Diagram (Text)
