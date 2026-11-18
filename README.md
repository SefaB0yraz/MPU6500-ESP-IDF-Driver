# MPU6500 ESP-IDF Driver

A C++ driver for the MPU6500 6-axis motion tracking sensor, developed from scratch using ESP-IDF.

This project aims to establish low-level I2C communication with the MPU6500 and implement sensor data acquisition, configuration, and processing without relying on ready-made sensor libraries.

Current state: the sensor is woken up, read over I2C, converted to physical units, gyro bias is calibrated, and roll/pitch are estimated with a complementary filter.

## Features

- I2C master communication using the **new ESP-IDF I2C master API** (`driver/i2c_master.h`)
- Wake-up from sleep via `PWR_MGMT_1` (sleep bit cleared, clock source set to gyro X PLL)
- Startup readout of `ACCEL_CONFIG` and `GYRO_CONFIG`
- 3-axis accelerometer and gyroscope burst reads (6 bytes each, big-endian `int16`)
- Raw data conversion to **g** and **°/s**
- Gyroscope **bias calibration** at startup
- Roll and pitch from the accelerometer (`atan2`)
- **Complementary filter** fusing gyro and accelerometer
- Gyro-only yaw integration (drifts, see limitations)

## Hardware

- **Microcontroller:** ESP32
- **Sensor:** MPU6500 (6-axis accelerometer and gyroscope)
- **Development Framework:** ESP-IDF (v5.2 or newer, required for the new I2C master driver)
- **Programming Language:** C++

## Communication

The sensor communicates with the ESP32 over I2C.

| Connection | ESP32 GPIO |
| ---------- | ---------- |
| SDA        | GPIO 21    |
| SCL        | GPIO 22    |
| VCC        | 3.3V       |
| GND        | GND        |

- I2C Address: `0x68` (AD0 pin low)
- Bus Speed: `50 kHz`
- Internal ESP32 pull-ups are enabled in code (`enable_internal_pullup`)

## Registers Used

| Register       | Address | Use                                   |
| -------------- | ------- | ------------------------------------- |
| `PWR_MGMT_1`   | `0x6B`  | Wake-up, clock source (`0x01`)        |
| `GYRO_CONFIG`  | `0x1B`  | Read at startup (full-scale range)    |
| `ACCEL_CONFIG` | `0x1C`  | Read at startup (full-scale range)    |
| `ACCEL_XOUT_H` | `0x3B`  | Start of 6-byte accelerometer burst   |
| `GYRO_XOUT_H`  | `0x43`  | Start of 6-byte gyroscope burst       |

## Sensor Scaling

The code reads `ACCEL_CONFIG` and `GYRO_CONFIG` but does not write them, so the power-on defaults are assumed:

| Sensor        | Full-scale | Sensitivity       |
| ------------- | ---------- | ----------------- |
| Accelerometer | ±2 g       | 16384 LSB/g       |
| Gyroscope     | ±250 °/s   | 131 LSB/(°/s)     |

If you change the range, the divisors in `main.cpp` must be changed with it.

## Signal Processing

**Gyro bias calibration.** 100 gyro samples (10 ms apart, about 1 s) are averaged at startup and subtracted from every later reading. The board must be **stationary** during this time.

**Accelerometer angles:**

```
roll  = atan2(ay, az)
pitch = atan2(-ax, sqrt(ay² + az²))
```

**Complementary filter** (per axis, `dt` measured with `esp_timer_get_time()` every loop):

```
angle = α · (angle + ω · dt) + (1 − α) · angle_accel        α = 0.98
```

The gyro gives a fast, smooth response but drifts. The accelerometer gives a drift-free gravity reference but is noisy. The filter trusts the gyro in the short term and pulls toward the accelerometer in the long term.

| Parameter                | Value                                   |
| ------------------------ | --------------------------------------- |
| α                        | 0.98                                    |
| Loop delay               | 40 ms (`usleep`) plus I2C and print time |
| Effective time constant  | ≈ 2 s (α·dt / (1 − α) with dt ≈ 40 ms)  |
| Calibration              | 100 samples, 10 ms apart                |

`dt` is measured, not assumed, so loop jitter does not corrupt the integration directly.

## Build and Flash

```bash
idf.py set-target esp32
idf.py build
idf.py -p <PORT> flash monitor
```

## Known Limitations

- **Yaw drifts.** The MPU6500 has no magnetometer, so there is no absolute heading reference. `Z Angle` is a pure gyro integral.
- **Dynamic acceleration is not rejected.** The accelerometer angle assumes gravity is the only acceleration. Shaking or moving the board disturbs roll/pitch until the filter recovers.
- **Fixed ranges.** ±2 g and ±250 °/s are assumed, not configured by the driver.
- **No FreeRTOS integration yet.** Timing uses `usleep()` inside `app_main`, so the loop is not a proper task with a fixed period.
- **Error handling is `ESP_ERROR_CHECK` only.** Any I2C failure aborts the program.
- **Calibration is gyro only.** There is no accelerometer offset calibration.

## Current Progress

- [x] ESP-IDF project setup
- [x] I2C master initialization
- [x] MPU6500 device communication
- [x] WHO_AM_I register verification
- [x] PWR_MGMT_1 register configuration
- [x] Accelerometer X-axis raw data acquisition
- [x] Three-axis accelerometer data acquisition
- [ ] Accelerometer configuration
- [x] Raw data conversion to physical units (g)
- [x] Gyroscope data acquisition
- [ ] Sensor initialization and read functions
- [ ] Error handling improvements
- [ ] FreeRTOS task integration

## Project Structure

```
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

Working prototype: raw readout, unit conversion, gyro calibration and sensor fusion run on hardware. Driver structure, configuration and FreeRTOS integration are still to come.

## Author

Sefa Boyraz

GitHub: [SefaB0yraz](https://github.com/SefaB0yraz)
