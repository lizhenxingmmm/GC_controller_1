#include "bsp_i2c.h"
#include "stm32f4xx.h"
#include <stdio.h>


static void i2c_Delay(void)
{
    uint8_t i;
    for (i = 0; i < 10; i++)
        ;
}


void i2c_Start(void)
{
    
    BSP_I2C_SDA_1();
    BSP_I2C_SCL_1();
    i2c_Delay();
    BSP_I2C_SDA_0();
    i2c_Delay();
    BSP_I2C_SCL_0();
    i2c_Delay();
}


void i2c_Stop(void)
{
    BSP_I2C_SDA_0();
    BSP_I2C_SCL_1();
    i2c_Delay();
    BSP_I2C_SDA_1();
}


void i2c_SendByte(uint8_t _ucByte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            BSP_I2C_SDA_1();
        }
        else
        {
            BSP_I2C_SDA_0();
        }
        i2c_Delay();
        BSP_I2C_SCL_1();
        i2c_Delay();
        BSP_I2C_SCL_0();
        if (i == 7)
        {
            BSP_I2C_SDA_1(); 
        }
        _ucByte <<= 1; 
        i2c_Delay();
    }
}


uint8_t i2c_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        BSP_I2C_SCL_1();
        i2c_Delay();
        if (BSP_I2C_SDA_READ())
        {
            value++;
        }
        BSP_I2C_SCL_0();
        i2c_Delay();
    }
    return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
    uint8_t re;

    BSP_I2C_SDA_1(); 
    i2c_Delay();
    BSP_I2C_SCL_1(); 
    i2c_Delay();
    if (BSP_I2C_SDA_READ()) 
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    BSP_I2C_SCL_0();
    i2c_Delay();
    return re;
}

void i2c_Ack(void)
{
    BSP_I2C_SDA_0(); 
    i2c_Delay();
    BSP_I2C_SCL_1(); 
    i2c_Delay();
    BSP_I2C_SCL_0();
    i2c_Delay();
    BSP_I2C_SDA_1(); 
}


void i2c_NAck(void)
{
    BSP_I2C_SDA_1(); 
    i2c_Delay();
    BSP_I2C_SCL_1();
    i2c_Delay();
    BSP_I2C_SCL_0();
    i2c_Delay();
}


void i2c_CfgGpio(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    I2Cx_SCL_GPIO_CLK_ENABLE();
    I2Cx_SDA_GPIO_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB9     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = BSP_I2C_SCL_PIN | BSP_I2C_SDA_PIN;
    ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(BSP_GPIO_PORT_I2C, &GPIO_InitStruct);
   
    i2c_Stop();
}

uint8_t IIC_MEM_Read(uint8_t* data,uint8_t i2c_address,uint8_t mem_address)
{
    i2c_Start();
    i2c_SendByte(i2c_address);
    if(i2c_WaitAck())
    {return IIC_ERROR;}

        i2c_SendByte(mem_address);
        if(i2c_WaitAck())
        {return IIC_ERROR;}

        *data=i2c_ReadByte();
        i2c_Stop();
    return IIC_OK;
}

