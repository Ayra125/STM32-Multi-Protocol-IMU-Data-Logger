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

### 2026-06-30 (latest)
- Added JEDEC ID verification to `W25Q128_Init`: sends `0x9F`, reads manufacturer byte, returns `HAL_ERROR` if not `0xEF`
- Removed `uart.c` / `uart.h` wrapper driver; replaced `UART_Print` calls in `main.c` with direct `HAL_UART_Transmit`

### 2026-06-29
- Fixed `main.c`: SPI1 NSS changed from `SPI_NSS_HARD_OUTPUT` to `SPI_NSS_SOFT` for software-managed CS
- Fixed `ssd1306.c`: `SSD1306_WriteString` parameter changed to `const char *`
- Fixed `ssd1306.h`: updated `SSD1306_WriteString` declaration to match
- Fixed `mpu6050.c`: guard against uninitialized buffer reads on I2C failure in `ReadAccel` and `ReadGYRO`
- Fixed `ssd1306.c`: `SSD1306_WriteString` syntax error (`char const string` → `const char *string`)

### 2026-06-29
- Completed Makefile with MCU flags, source files, include paths, and build targets
- Fixed `mpu6050.c`: wrong include filename (`MPU6050_H` → `mpu6050.h`)
- Fixed `mpu6050.c`: guard against uninitialized buffer reads on I2C failure in `ReadAccel` and `ReadGYRO`
- Fixed `ssd1306.c`: missing semicolon on font array, buffer overflow in `SSD1306_Clear`, added size guard to `SSD1306_SendData`
- Fixed `ssd1306.c`: added horizontal addressing mode to `SSD1306_Init` so clear and write functions work correctly
- Fixed `ssd1306.h`: added `const` qualifier to `SSD1306_SendData` declaration
- Fixed `w25q128.h`: replaced placeholder GPIO pin/port values with correct PA4/GPIOA
- Fixed `w25q128.c`: added 3s timeout to `W25Q128_WaitBusy` to prevent MCU hang
- Fixed `w25q128.c`: `W25Q128_Write` now waits for flash to finish internal page program
- Added peripheral init calls for MPU6050, W25Q128, and SSD1306 in `main.c`

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
