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

### 2026-07-08 (latest) — Pre-Altium Design Phase

Worked through every schematic decision from first principles before entering Altium. All values are either derived from equations or pulled directly from datasheets and verified against the reasoning, rather than copied from reference designs.

**I2C Trace Width**
- Ruled out ampacity: I2C open-drain switching currents are sub-mA, far below even minimum-trace limits.
- Ruled out trace resistance: calculated milliohm resistance is negligible against kΩ-range pull-up resistors and has no meaningful effect on the RC time constant.
- Confirmed trace capacitance is real but negligible: C = ε₀εᵣA/d, where A = L×W, so wider traces add capacitance — but numerically this is a rounding error against the 400pF budget.
- Conclusion: width is a manufacturing decision only — set comfortably above the fab minimum for etch-variance margin and hand-inspection ease.

**Trace Length Placeholder**
- Real lengths depend on Altium placement not yet done; reasoned that clustering I2C1 devices (MPU6050, SSD1306) near the STM32 on a small prototype board justifies a 3–4cm conservative placeholder.

**Bus Capacitance vs. 400pF Ceiling (UM10204 Fast-mode limit)**
- Looked up MPU6050 real C_IN; used UM10204 per-pin ceiling as stand-in for SSD1306 (datasheet publishes no value).
- Device capacitance sum ≈ 15pF, leaving ~385pF headroom.
- Placeholder trace capacitance ≈ 0.11pF (≈0.03% of headroom) — completely negligible.
- Side-track: traced pin capacitance back to PN junction physics — ESD protection diodes share the conductor/poor-conductor/conductor structure of a parallel-plate capacitor, which is why pin capacitance is a lumped, unpredictable value rather than something derivable from a clean formula.

**Pull-up Resistor Value (R_p)**
- R_p(max) = t_r / (0.8473 × C_b): slowest allowable resistor, set by the UM10204 rise-time budget (RC charging curve).
- R_p(min) = (VDD − VOL,max) / I_OL: fastest allowable resistor, set by Ohm's law across the device's internal "on" resistance during low-assertion.
- Looked up real I_OL for both devices; SSD1306 (100µA) is the weaker sink, so it governs R_p(min) — the shared bus must work regardless of which device is pulling low.
- Valid range: ~967Ω–17.6kΩ. Chose **1.5kΩ**: USB power removes any power-budget pressure, so leaning toward the fast end makes sense while still keeping healthy margin above R_min.

**W25Q128 SPI Mode**
- Chip supports Standard/Dual/Quad SPI; STM32F446RE has a separate QUADSPI peripheral capable of it.
- Chose to stay on standard SPI: the additional peripheral, new pins, different command sequences, and re-testing working Phase 1 firmware cost more than the throughput gain the actual workload would see.
- IO2(/WP) and IO3(/HOLD): active-low pins, floating inputs are dangerous, so added pull-ups to VCC (not hard-wired) so pins stay inactive but can still be safely overridden.

**STM32 Support Circuitry**

- *Crystal:* I2C and SPI are self-referenced so internal oscillator drift doesn't matter for those peripherals alone, but UART has no shared clock so drift accumulates bit-by-bit across a frame. Since the whole chip shares one clock tree, an accurate external crystal is required. Confirmed **8MHz** HSE with two load capacitors (C_L1/C_L2) to ground and a series R_EXT to limit drive current and protect the crystal from being over-driven.
- *NRST:* Active-low; idles HIGH via pull-up to VCC, button to GND for manual reset, small capacitor to GND to filter brief noise glitches without blocking a genuine sustained press.
- *BOOT0:* RM0390 table confirms BOOT0=0 selects Main Flash boot. Chose a **pull-down resistor** to GND (not a hard wire), preserving future flexibility to override via jumper for bootloader access.
- *Decoupling:* Trace inductance can't respond instantly to fast internal switching current demand, causing local voltage dips; a capacitor close to the pin acts as a local charge reservoir. Values pulled directly from datasheets:
  - STM32F446RE: 12×100nF + 1×4.7µF across 4 VDD pins; 2×2.2µF at VCAP_1/VCAP_2.
  - MPU6050: 0.1µF at VDD, 0.1µF at VLOGIC.
  - SSD1306: charge-pump caps per datasheet typical circuit.
  - W25Q128: datasheet specifies no value; justified default of **100nF**.

### 2026-06-30
- Fixed `main.c`: configured PA4 as GPIO push-pull output and drive high on init so W25Q128 CS starts deasserted
- Fixed `main.c`: `init_ok` flag prevents "All peripherals initialized" from printing if any init failed
- Fixed `w25q128.c`: added page boundary check to `W25Q128_Write` to prevent silent data corruption
- Fixed `w25q128.c`: wrapped `HAL_SPI_Transmit` and `HAL_SPI_Receive` in `W25Q128_Read` with error checks and CS high on failure
- Fixed `w25q128.c`: initialised `buffer` to `0x01` in `W25Q128_WaitBusy` to prevent false ready on SPI failure

### 2026-06-30
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
