#include "w25q128.h"

HAL_StatusTypeDef W25Q128_Init(SPI_HandleTypeDef *hspi){

    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    uint8_t cmd = W25Q128_REL_POWER_DOWN;
    HAL_SPI_Transmit(hspi, &cmd, 1, 500);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    return HAL_OK;
}

HAL_StatusTypeDef W25Q128_Write(SPI_HandleTypeDef *hspi,uint32_t address,uint8_t *data,uint16_t size){
    
    if(W25Q128_WaitBusy(hspi) != HAL_OK) return HAL_ERROR;
    uint8_t cmd = W25Q128_WRITE_ENB;
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hspi, &cmd, 1, 500);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
    uint8_t meta[4]={
        W25Q128_PAGE_PROGRAM,
        (address >> 16) & 0xFF,
        (address >> 8) & 0xFF,
        address & 0xFF,
    };
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hspi, meta, 4, 500);
     HAL_SPI_Transmit(hspi, data, size, 500);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
    return HAL_OK;
}

HAL_StatusTypeDef W25Q128_Read(SPI_HandleTypeDef *hspi, uint32_t address, uint8_t *data, uint16_t size){
    if(W25Q128_WaitBusy(hspi) != HAL_OK) return HAL_ERROR;
    uint8_t meta[4]={
        W25Q128_READ_DATA,
        (address >> 16) & 0xFF,
        (address >> 8) & 0xFF,
        address & 0xFF,
    };
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hspi, meta, 4, 500);
     HAL_SPI_Receive(hspi, data, size, 500);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
    return HAL_OK;

}

HAL_StatusTypeDef W25Q128_EraseSector(SPI_HandleTypeDef *hspi, uint32_t address){
    // Transaction 1 - Write Enable
    uint8_t cmd = W25Q128_WRITE_ENB;
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hspi, &cmd, 1, 500);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

    // Transaction 2 - Sector Erase + address
    uint8_t erase[4] = {
        W25Q128_SECTOR_ERASE,
        (address >> 16) & 0xFF,
        (address >> 8) & 0xFF,
        address & 0xFF
    };
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hspi, erase, 4, 500);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

    // Wait until done
    return W25Q128_WaitBusy(hspi);
}
HAL_StatusTypeDef W25Q128_WaitBusy(SPI_HandleTypeDef *hspi){
    while(1){
        uint8_t cmd = W25Q128_READ_STAT_REG_1;
        uint8_t buffer;
        HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET); // CS low
        HAL_SPI_Transmit(hspi, &cmd, 1, 500);
        HAL_SPI_Receive(hspi, &buffer, 1, 500);
        HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);   // CS high
        if((buffer & 0x01) == 0) return HAL_OK;
    }
}