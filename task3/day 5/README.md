# DHT22 ESP32 Temperature & Humidity Sensor

Reads temperature (°C) and relative humidity (%) from a DHT22 sensor on ESP32.
Implements retry logic, median filtering, and structured serial output ready for
Zelbytes IoT Lab (Day 7) and MQTT (Day 12).

## Wiring

![Wiring Diagram](docs/wiring_diagram.png)

| DHT22 Pin | ESP32 Pin | Notes |
|-----------|-----------|-------|
| VCC (Pin 1) | 3.3V | Never use 5V |
| DATA (Pin 2) | GPIO 4 | 10kΩ pull-up to 3.3V required |
| GND (Pin 4) | GND | |

## Features

- Retry up to 3 times per cycle on checksum or NaN failure
- Median filter over last 3 good samples rejects electrical noise spikes
- SensorBundle struct with validity flag — never crashes loop()
- JSON-formatted serial output matching future field names

## Serial Output

![Serial Output](docs/serial_output.png)

```json
{"temperature_c":28.55,"humidity_pct":65.25,"raw_temp":28.70,"raw_hum":65.10,"filtered":true}
```

## Setup

1. Wire DHT22 to ESP32 as shown above
2. Install **DHT sensor library** by Adafruit in Arduino IDE
3. Open `src/dht22_sensor/dht22_sensor.ino`
4. Select your board (ESP32 Dev Module) and port
5. Upload and open Serial Monitor at **115200 baud**

## Dependencies

- [DHT sensor library by Adafruit](https://github.com/adafruit/DHT-sensor-library)
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Unified_Sensor)

## License

MIT
