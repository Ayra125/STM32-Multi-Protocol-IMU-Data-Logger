#include "SSD1306_H"
#include <string.h>
HAL_StatusTypeDef SSD1306_Init(I2C_HandleTypeDef *hi2c){
if(SSD1306(*hi2c,SSD1306_SET_MUX_RATIO)!=HAL_OK) return HAL_ERROR;
if(SSD1306(*hi2c,0x3F)!=HAL_OK) return HAL_ERROR;
if(SSD1306(*hi2c,0xD3)!=HAL_OK) return HAL_ERROR;
if(SSD1306(*hi2c,0xA8)!=HAL_OK) return HAL_ERROR;
if(SSD1306(*hi2c,0x00)!=HAL_OK) return HAL_ERROR;
if(SSD1306(*hi2c,0x40)!=HAL_OK) return HAL_ERROR;
return HAL_OK;
}

HAL_StatusTypeDef SSD1306_SendCommand(I2C_HandleTypeDef *hi2c,uint8_t cmd){
      uint8_t data[2]={SSD1306_CMD, cmd};

    return HAL_I2C_Master_Transmit(hi2c, SSD1306_ADDR <<1,data,2,500);
}

HAL_StatusTypeDef SSD1306_SendData(I2C_HandleTypeDef *hi2c,uint8_t *data,uint16_t size){
    uint8_t buff[129];
    buff[0]=SSD1306_DATA;
    memcpy(&buff[1],data,size);
    return HAL_I2C_Master_Transmit(hi2c, SSD1306_ADDR <<1,buff,size+1,500);
}

HAL_StatusTypeDef SSD1306_Clear(I2C_HandleTypeDef *hi2c){


}

HAL_StatusTypeDef SSD1306_WriteString(I2C_HandleTypeDef *hi2c,char *string){


}