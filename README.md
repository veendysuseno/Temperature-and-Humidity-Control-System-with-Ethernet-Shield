# Temperature and Humidity Control System with Ethernet Shield

## Overview

This project is a temperature and humidity control system using an Arduino-compatible microcontroller, an Ethernet shield, and the SHT1x sensor. The system automatically controls a fan and an air conditioner (AC) based on environmental conditions. It also communicates with a web server to log temperature, humidity, and the status of the fan and AC.

## Features

- Reads temperature and humidity using the SHT1x sensor.
- Controls a fan and AC based on predefined temperature and humidity thresholds.
- Provides manual or automatic control modes for both the fan and AC.
- Sends real-time temperature, humidity, and device status to a web server via HTTP.
- Logs data to a web server for remote monitoring.

## Hardware Components

- Arduino-compatible microcontroller
- Ethernet Shield (W5100/W5200/W5500)
- SHT1x Temperature and Humidity Sensor
- 16x2 Liquid Crystal Display (LCD)
- 2 Relay Modules (for fan and AC control)
- Web server (to store and view sensor data and device statuses)

## Pin Connections

- **SHT1x Sensor**:
  - Data Pin: `Pin 7`
  - Clock Pin: `Pin 6`
- **LCD**: 
  - RS: `Pin 14`
  - Enable: `Pin 15`
  - D4: `Pin 16`
  - D5: `Pin 17`
  - D6: `Pin 18`
  - D7: `Pin 19`
- **Relay for Fan**: `Pin 5`
- **Relay for AC**: `Pin 4`

## Software Requirements

- Arduino IDE (1.8.x or later)
- Arduino Ethernet library
- LiquidCrystal library
- SHT1x library

## Code Structure

- `tempHum()`: Reads temperature and humidity values from the SHT1x sensor and displays them on the LCD.
- `datakipastxt()`: Sends a request to the server to check the status of the fan and control it based on the response.
- `dataACtxt()`: Sends a request to the server to check the status of the AC and control it based on the response.
- `datalog()`: Sends temperature, humidity, and device status data to the web server.
- `connectAndRead()`: Handles HTTP communication with the server to retrieve or send data.

## Setup

1. Wire the components as per the pin connections above.
2. Upload the code (`main.ino`) to your Arduino-compatible microcontroller.
3. Ensure your web server is set up to handle the incoming requests at the specified IP address (in this case, `192.168.1.2`) and the appropriate routes are created for:
   - `/incubator/datakipas.txt`
   - `/incubator/dataAC.txt`
   - `/incubator/updated2.php`
4. Run the system and monitor the temperature and humidity via the LCD. The fan and AC will switch on or off based on the set conditions.
5. Check the web server to view logs of the temperature, humidity, and device statuses.

## HTTP Communication

- The system uses HTTP GET requests to communicate with the server.
- Temperature, humidity, and device statuses are sent in the following format:

```bash
GET /incubator/updated2.php?SUHU=<temperature>&nomer=<log_number>&STATUSK=<fan_status>&hmdty=<humidity>&STATUSAC=<AC_status> HTTP/1.0
```


## License

This project is licensed under the MIT License - see the LICENSE file for details.
