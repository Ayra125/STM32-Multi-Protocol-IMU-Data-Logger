#include "stm32f4xx_hal.h"
#include "uart.h"
#include <string.h>

HAL_StatusTypeDef UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *data, uint16_t size) {
    return HAL_UART_Transmit(huart, data, size, 500);
}

HAL_StatusTypeDef UART_TransmitString(UART_HandleTypeDef *huart, const char *str) {
    return HAL_UART_Transmit(huart, (const uint8_t *)str, (uint16_t)strlen(str), 500);
}

HAL_StatusTypeDef UART_Receive(UART_HandleTypeDef *huart, uint8_t *buf, uint16_t size) {
    return HAL_UART_Receive(huart, buf, size, 500);
}
