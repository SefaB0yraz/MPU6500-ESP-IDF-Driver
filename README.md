# MPU6500 ESP-IDF Driver

**A learning-focused MPU6500 sensor driver implementation using C++, ESP-IDF and ESP32.**

This project is a hands-on learning exercise focused on understanding low-level sensor communication, register configuration, data acquisition and basic orientation estimation.

The goal is to develop my own understanding of sensor drivers and build a reusable foundation for future embedded systems projects.

## Hardware

- ESP32
- MPU6500 6-axis IMU
  - 3-axis accelerometer
  - 3-axis gyroscope

## Development Environment

- Language: C++
- Framework: ESP-IDF v5.3.1
- Communication Protocol: I2C

## Implemented Features

- [x] I2C master initialization using ESP-IDF
- [x] MPU6500 device initialization and wake-up
- [x] Register configuration and reading
- [x] 3-axis accelerometer data acquisition
- [x] 3-axis gyroscope data acquisition
- [x] Raw sensor data conversion into physical units
- [x] Gyroscope bias calibration
- [x] Angular velocity integration for angle estimation
- [x] Accelerometer-based roll and pitch estimation
- [x] Complementary filter implementation
- [ ] Physical validation and refinement of orientation estimation
- [ ] Refactoring into a modular C++ driver structure
- [ ] FreeRTOS integration

## Sensor Configuration

Current configuration:

| Parameter | Configuration |
|---|---|
| I2C Address | 0x68 |
| I2C Clock | 50 kHz |
| Accelerometer Range | ±2g |
| Gyroscope Range | ±250 °/s |
| Accelerometer Sensitivity | 16384 LSB/g |
| Gyroscope Sensitivity | 131 LSB/(°/s) |

## Orientation Estimation

The implementation uses both accelerometer and gyroscope measurements.

**Gyroscope:**
- Measures angular velocity.
- Bias calibration is performed at startup.
- Angular velocity is integrated over time to estimate angular displacement.

**Accelerometer:**
- Raw measurements are converted into g.
- Roll and pitch are estimated using trigonometric calculations.

**Complementary Filter:**

A complementary filter combines gyroscope integration with accelerometer-based angle estimation to obtain a more stable roll and pitch estimate.

The filter is currently implemented and undergoing physical validation.

## Current Development Status

The sensor communication, data acquisition, calibration and basic orientation estimation pipeline are implemented.

The next development stage focuses on validating the angle estimation, improving the software structure and integrating FreeRTOS for more structured sensor data acquisition.

## Project Motivation

Rather than relying entirely on ready-made sensor libraries, I wanted to understand how an IMU communicates with a microcontroller and how raw sensor measurements can be processed into meaningful physical quantities.

This project is part of my learning journey in embedded software development, with a particular interest in aerospace and control systems.

---

**Technologies:** C++ · ESP-IDF · ESP32 · MPU6500 · I2C · Sensor Fusion
