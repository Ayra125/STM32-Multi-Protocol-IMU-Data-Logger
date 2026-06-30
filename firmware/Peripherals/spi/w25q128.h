#ifndef W25Q128_H
#define W25Q128_H
#include "stm32f4xx_hal.h"

#define W25Q128_WRITE_ENB 0x06
#define W25Q128_WRITE_DISB 0x04
#define W25Q128_READ_DATA 0x03
#define W25Q128_SECTOR_ERASE 0x20
#define W25Q128_READ_STAT_REG_1 0x05
#define W25Q128_POWER_DOWN 0xB9
#define W25Q128_REL_POWER_DOWN 0xAB
#define W25Q128_PAGE_PROGRAM 0x02
#define W25Q128_JEDEC_ID 0x9F
#define W25Q128_MFR_ID 0xEF
#define W25Q128_MEM_TYPE 0x40
#define W25Q128_CAPACITY 0x18
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA

HAL_StatusTypeDef W25Q128_Init(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef W25Q128_Write(SPI_HandleTypeDef *hspi,uint32_t address,uint8_t *data,uint16_t size);

HAL_StatusTypeDef W25Q128_Read(SPI_HandleTypeDef *hspi, uint32_t address, uint8_t *data, uint16_t size);

HAL_StatusTypeDef W25Q128_EraseSector(SPI_HandleTypeDef *hspi,uint32_t address);

HAL_StatusTypeDef W25Q128_WaitBusy(SPI_HandleTypeDef *hspi);
#endif