# MPU6500 ESP-IDF Driver

A C++ driver for the MPU6500 6-axis motion tracking sensor, developed from scratch using ESP-IDF.

This project aims to establish low-level I2C communication with the MPU6500 and implement sensor data acquisition, configuration, and processing without relying on ready-made sensor libraries.

## Hardware

- **Microcontroller:** ESP32
- **Sensor:** MPU6500 (6-axis accelerometer and gyroscope)
- **Development Framework:** ESP-IDF
- **Programming Language:** C++

## Communication

The sensor communicates with the ESP32 over I2C.

| Connection | ESP32 GPIO |
|---|---|
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| VCC | 3.3V |
| GND | GND |

- I2C Address: `0x68`
- Bus Speed: `100 kHz`

## Current Progress

- [x] ESP-IDF project setup
- [x] I2C master initialization
- [x] MPU6500 device communication
- [x] WHO_AM_I register verification
- [x] PWR_MGMT_1 register configuration
- [x] Accelerometer X-axis raw data acquisition
- [ ] Three-axis accelerometer data acquisition
- [ ] Accelerometer configuration
- [ ] Raw data conversion to physical units (g)
- [ ] Gyroscope data acquisition
- [ ] Sensor initialization and read functions
- [ ] Error handling improvements
- [ ] FreeRTOS task integration

## Project Structure

```text
MPU6500-ESP-IDF-Driver/
├── main/
│   └── main.cpp
├── CMakeLists.txt
├── README.md
└── LICENSE
```

## Development Goals

The main goal is to develop a reusable and modular sensor driver while gaining practical experience in:

- Embedded C++ programming
- I2C communication protocols
- Register-level sensor control
- ESP-IDF peripheral APIs
- Real-time operating systems (FreeRTOS)

## Status

**Work in Progress**

This project is actively being developed. Features will be implemented and documented incrementally.

## Author

Sefa Boyraz

GitHub: [SefaB0yraz](https://github.com/SefaB0yraz)
