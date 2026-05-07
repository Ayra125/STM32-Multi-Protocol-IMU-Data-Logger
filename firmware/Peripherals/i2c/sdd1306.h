#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f4xx_hal.h"

// I2C address
#define SSD1306_ADDR  0x3C

// Display ON/OFF
#define SSD1306_DISP_OFF        0xAE
#define SSD1306_DISP_ON         0xAF

// Clock / timing
#define SSD1306_SET_CLK_DIV     0xD5
#define SSD1306_SET_MUX_RATIO   0xA8
#define SSD1306_SET_DISP_OFFSET 0xD3
#define SSD1306_SET_DISP_START  0x40

// Charge pump
#define SSD1306_SET_CHARGE_PUMP 0x8D
#define SSD1306_CHARGE_PUMP_ON  0x14

// Memory addressing
#define SSD1306_MEM_ADDR_MODE   0x20
#define SSD1306_COL_ADDR        0x21
#define SSD1306_PAGE_ADDR       0x22

// Segment remap: controls left/right orientation
#define SSD1306_SEG_REMAP_NORMAL  0xA0
#define SSD1306_SEG_REMAP_INV     0xA1

// COM output scan direction: controls up/down orientation
#define SSD1306_COM_SCAN_NORMAL   0xC0
#define SSD1306_COM_SCAN_INV      0xC8

// COM pins hardware configuration
#define SSD1306_SET_COM_PINS      0xDA

// Common COM pin settings
#define SSD1306_COM_PINS_SEQ      0x02
#define SSD1306_COM_PINS_ALT      0x12

// Contrast control
#define SSD1306_SET_CONTRAST      0x81

// Display from RAM / entire display control
#define SSD1306_DISPLAY_RAM       0xA4
#define SSD1306_DISPLAY_ALL_ON    0xA5
// comand and data
#define SSD1306_CMD   0x00
#define SSD1306_DATA  0x40

#endif