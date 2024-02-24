#ifndef DRV_SOFT_I2C_H
#define DRV_SOFT_I2C_H
#include "gpio.h"

#define SCL_H HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET)
#define SCL_L HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET)

#define SDA_H HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET)
#define SDA_L HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET)

#define SDA_READ HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

typedef enum
{
    MASTER_READ=1,
    MASTER_WRITE=0
}I2C_STATE;

typedef unsigned char u8;

void i2c_mem_read(u8* data,u8 i2c_address,u8 mem_address);
void i2c_mem_write(u8 data,u8 i2c_address,u8 mem_address);

#endif
