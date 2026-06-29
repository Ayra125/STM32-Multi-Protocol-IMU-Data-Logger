# Embedded Communication Protocol Bridge

Custom STM32-based embedded system integrating I2C, SPI, and UART peripherals for sensor data acquisition, display, and communication.

## Hardware

- **MCU:** STM32F446RE (Nucleo-64)
- **Protocols:** I2C, SPI, UART

## Drivers Implemented

| Peripheral | Protocol | Status |
|---|---|---|
| MPU6050 (IMU) | I2C | Complete |
| SSD1306 (OLED Display) | I2C | Complete |
| W25Q128 (Flash Memory) | SPI | Complete |

## Project Structure

```
firmware/
├── Core/           # STM32 HAL core (main, startup, system files)
├── Drivers/        # STM32 HAL & CMSIS drivers
├── Peripherals/    # Custom peripheral drivers
│   ├── i2c/        # MPU6050, SSD1306
│   └── spi/        # W25Q128
└── Makefile        # Build system
docs/
├── datasheets/     # Component datasheets (MPU6050, SSD1306, W25Q128, STM32F446RE)
└── wiring/         # Wiring diagrams
tests/
└── loopback/       # Protocol loopback tests
```

## Changelog

### 2026-06-28
- Added `Makefile` for firmware build system
- Fixed `ssd1306.h` filename (was misspelled as `sdd1306.h`)

### Earlier
- Completed W25Q128 SPI flash memory driver
- Completed SSD1306 OLED display driver (init, clear, draw functions)
- Implemented MPU6050 IMU driver over I2C
- Added SSD1306 driver skeleton and header
- Added development log (`DEVLOG.md`)
- Added datasheets for all components
