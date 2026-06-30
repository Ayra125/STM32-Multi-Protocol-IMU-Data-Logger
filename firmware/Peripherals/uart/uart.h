#ifndef UART_H
#define UART_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

HAL_StatusTypeDef UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *data, uint16_t size);

HAL_StatusTypeDef UART_TransmitString(UART_HandleTypeDef *huart, const char *str);

HAL_StatusTypeDef UART_Receive(UART_HandleTypeDef *huart, uint8_t *buf, uint16_t size);

#endif
