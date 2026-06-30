#include "stm32f4xx_hal.h"
#include "mpu6050.h"

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c){
    uint8_t data[2]={MPU6050_PWR_MGMT_1, 0x00};
     

    return HAL_I2C_Master_Transmit(hi2c, MPU6050_ADDR<<1,data,2,500);
}

HAL_StatusTypeDef MPU6050_ReadAccel(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z){

 uint8_t buffer[6];
    HAL_StatusTypeDef status =HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR<<1,MPU6050_ACCEL_XOUT_H
        ,I2C_MEMADD_SIZE_8BIT,buffer,6,500);
    if(status != HAL_OK) return status;
    *x = (int16_t)(buffer[0] << 8 | buffer[1]);
    *y = (int16_t)(buffer[2] << 8 | buffer[3]);
    *z = (int16_t)(buffer[4] << 8 | buffer[5]);
    

    return status;

}

HAL_StatusTypeDef MPU6050_ReadGYRO(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z){
    uint8_t buffer[6];
    HAL_StatusTypeDef status =HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR<<1,MPU6050_GYRO_XOUT_H
        ,I2C_MEMADD_SIZE_8BIT,buffer,6,500);
    if(status != HAL_OK) return status;
    *x = (int16_t)(buffer[0] << 8 | buffer[1]);
    *y = (int16_t)(buffer[2] << 8 | buffer[3]);
    *z = (int16_t)(buffer[4] << 8 | buffer[5]);
    

    return status;

}


