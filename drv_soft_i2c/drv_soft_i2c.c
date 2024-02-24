#include "drv_soft_i2c.h"


void IIC_delay(void)
{
	u8 t=3;
	while(t--);
}

void I2C_Start(void)
{
	SDA_H;
	SCL_H;
	IIC_delay();
	SDA_L;
	IIC_delay();
	SCL_L;
	IIC_delay();
}

void I2C_Stop(void)
{
	SDA_L;
	SCL_H;
	IIC_delay();
	SDA_H;
}

void I2C_WaitAck(void) 
{
	SDA_H;
	IIC_delay();
	SCL_H;
	IIC_delay();
	SCL_L;
	IIC_delay();
}

void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		{
			SDA_H;
    }
		else
		{
			SDA_L;
    }
		IIC_delay();
		SCL_H;
		IIC_delay();
		SCL_L;
		dat<<=1;
  }
}

void i2c_mem_write(u8 data,u8 i2c_address,u8 mem_address)
{
    I2C_Start();
    Send_Byte(i2c_address);
    I2C_WaitAck();
    Send_Byte(mem_address);
    I2C_WaitAck();
    Send_Byte(data);
    I2C_WaitAck();
    I2C_Stop();
}

u8 Read_Byte(void)
{
    u8 i;
    u8 value;
    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        SCL_H;
        IIC_delay();
        if (SDA_READ==GPIO_PIN_SET)
        {
            value++;
        }
        SCL_L;
        IIC_delay();
    }
    return value;
}

void i2c_mem_read(u8* data,u8 i2c_address,u8 mem_address)
{
    I2C_Start();
    Send_Byte(i2c_address);
    I2C_WaitAck();
    Send_Byte(mem_address);
    I2C_WaitAck();
    *data=Read_Byte();
    I2C_WaitAck();
    I2C_Stop();
}

