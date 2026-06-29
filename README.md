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

### 2026-05-11
- Added `DEVLOG.md` development log

### 2026-05-09
- Added W25Q128 datasheet to `docs/datasheets/`

### 2026-05-08
- Completed W25Q128 SPI flash memory driver
- Completed SSD1306 OLED display driver

### 2026-05-07
- Implemented SSD1306 clear function
- Added SSD1306 driver skeleton

### 2026-05-06
- Added SSD1306 header file

### 2026-05-05
- Implemented MPU6050 IMU driver over I2C
- Added MPU6050 header file and I2C address defines
- Added datasheets for SSD1306, MPU6050, and STM32F446RE

### 2026-05-04
- Added CubeMX generated HAL boilerplate for STM32F446RE

### 2026-04-30
- Initialized project structure with driver and peripheral directories
- Added README files for firmware, hardware, and docs directories
