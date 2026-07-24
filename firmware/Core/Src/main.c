/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "w25q128.h"
#include "stm32f4xx_hal.h"
#include "mpu6050.h"
#include <string.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */
/* Led set up*/


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
uint8_t init_ok = 1;
if (MPU6050_Init(&hi2c1) != HAL_OK)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)"MPU6050 Init Failed\r\n", strlen("MPU6050 Init Failed\r\n"), 100);
    init_ok = 0;
}
if (W25Q128_Init(&hspi1) != HAL_OK)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)"W25Q128 Init Failed\r\n", strlen("W25Q128 Init Failed\r\n"), 100);
    init_ok = 0;
}
if (SSD1306_Init(&hi2c1) != HAL_OK)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)"SSD1306 Init Failed\r\n", strlen("SSD1306 Init Failed\r\n"), 100);
    init_ok = 0;
}
if(W25Q128_EraseSector(&hspi1,0x000000) != HAL_OK){

    HAL_UART_Transmit(&huart2, (uint8_t*)"EraseSector failed\r\n", strlen("EraseSector failed\r\n"), 100);
    init_ok=0;
  }
if (init_ok)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)"All peripherals initialized\r\n", strlen("All peripherals initialized\r\n"), 100);
}
  /* USER CODE END 2 */
 
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int16_t gx, gy, gz, x, y, z;
  char bufferA[60];
  char bufferG[60];
  uint32_t nextWriteLog=0;
  while (1)
  {
    if(nextWriteLog>=4096){
      HAL_StatusTypeDef EraseSectorA = W25Q128_EraseSector(&hspi1, 0x000000);
      if(EraseSectorA != HAL_OK) HAL_UART_Transmit(&huart2, (uint8_t*)"EraseSectorA failed\r\n", strlen("EraseSectorA failed\r\n"), 100);
      if(EraseSectorA == HAL_OK)nextWriteLog = 0;
    }
    HAL_StatusTypeDef accelStatus = MPU6050_ReadAccel(&hi2c1, &x, &y, &z);
    HAL_StatusTypeDef gyroStatus = MPU6050_ReadGYRO(&hi2c1,&gx, &gy, &gz);
    HAL_StatusTypeDef clear = SSD1306_Clear(&hi2c1);
if (accelStatus == HAL_OK && clear == HAL_OK) {
    snprintf(bufferA,sizeof(bufferA),"MPU6050 Read Accel: Ax:%d Ay:%d, Az:%d",x,y,z);
    HAL_StatusTypeDef SetCursorA = SSD1306_SetCursor(&hi2c1,0,0);
    HAL_StatusTypeDef WriteStringA = SSD1306_WriteString(&hi2c1,bufferA);
    if(SetCursorA == HAL_OK && WriteStringA == HAL_OK){
      HAL_UART_Transmit(&huart2,(uint8_t*)bufferA,strlen(bufferA),100);
    }
  if(SetCursorA != HAL_OK) HAL_UART_Transmit(&huart2, (uint8_t*)"SetCursorA failed\r\n", strlen("SetCursorA failed\r\n"), 100);
  if(WriteStringA != HAL_OK) HAL_UART_Transmit(&huart2, (uint8_t*)"WriteStringA failed\r\n", strlen("WriteStringA failed\r\n"), 100);
} 
if (gyroStatus == HAL_OK && clear == HAL_OK) {
    snprintf(bufferG,sizeof(bufferG),"MPU6050 Read Gyro: Gx:%d Gy:%d, Gz:%d",gx,gy,gz);
     HAL_StatusTypeDef SetCursorG =  SSD1306_SetCursor(&hi2c1,1,0);
     HAL_StatusTypeDef WriteStringG = SSD1306_WriteString(&hi2c1,bufferG);
     if(SetCursorG == HAL_OK && WriteStringG == HAL_OK){
      HAL_UART_Transmit(&huart2,(uint8_t*)bufferG,strlen(bufferG),100);
     }
  if(SetCursorG != HAL_OK) HAL_UART_Transmit(&huart2, (uint8_t*)"SetCursorG failed\r\n", strlen("SetCursorG failed\r\n"), 100);
  if(WriteStringG != HAL_OK) HAL_UART_Transmit(&huart2, (uint8_t*)"WriteStringG failed\r\n", strlen("WriteStringG failed\r\n"), 100);
} 
  if(accelStatus != HAL_OK)HAL_UART_Transmit(&huart2, (uint8_t*)"ReadAccel fail\r\n", strlen("ReadAccel fail\r\n"), 100);
  if(gyroStatus != HAL_OK)HAL_UART_Transmit(&huart2, (uint8_t*)"ReadGyro fail\r\n", strlen("ReadGyro fail\r\n"), 100);
  if (clear != HAL_OK) HAL_UART_Transmit(&huart2, (uint8_t*)"Clear failed\r\n", strlen("Clear failed\r\n"), 100);
   if(accelStatus == HAL_OK && gyroStatus == HAL_OK){
    int16_t record[6];
    record[0]=x;
    record[1]=y;
    record[2]=z;
    record[3]=gx;
    record[4]=gy;
    record[5]=gz;
     HAL_StatusTypeDef W25Q128_WriteS =  W25Q128_Write(&hspi1,nextWriteLog,(uint8_t*)record,12);
    if(W25Q128_WriteS != HAL_OK) HAL_UART_Transmit(&huart2, (uint8_t*)"W25Q128_Write fail\r\n", strlen("W25Q128_Write fail\r\n"), 100);
    else if(W25Q128_WriteS == HAL_OK) nextWriteLog += 16;
   } 
   HAL_Delay(100);
  }
 
  /* USER CODE END 3 */
}

 /*
  * @brief System Clock Configuration
  * @retval None
*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
