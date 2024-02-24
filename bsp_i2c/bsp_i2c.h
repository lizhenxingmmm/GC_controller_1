#ifndef _BSP_I2C_H
#define _BSP_I2C_H

#include <inttypes.h>

#define BSP_I2C_WR 0 
#define BSP_I2C_RD 1 

/* 定义I2C总线连接的GPIO端口时钟控制 */

#define I2Cx_SDA_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面3行代码即可任意改变SCL和SDA的引脚 */
#define BSP_GPIO_PORT_I2C GPIOB    /* GPIO端口 */
#define BSP_I2C_SCL_PIN GPIO_PIN_8 /* 连接到SCL时钟线的GPIO */
#define BSP_I2C_SDA_PIN GPIO_PIN_9 /* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 0                                                                /* 条件编译： 1 选择GPIO的库函数实现IO读写 */
#define BSP_I2C_SCL_1() digitalH(BSP_GPIO_PORT_I2C, BSP_I2C_SCL_PIN) /* SCL = 1 */
#define BSP_I2C_SCL_0() digitalL(BSP_GPIO_PORT_I2C, BSP_I2C_SCL_PIN) /* SCL = 0 */

#define BSP_I2C_SDA_1() digitalH(BSP_GPIO_PORT_I2C, BSP_I2C_SDA_PIN)         /* SDA = 1 */
#define BSP_I2C_SDA_0() digitalL(BSP_GPIO_PORT_I2C, BSP_I2C_SDA_PIN)         /* SDA = 0 */
	
	//#define BSP_I2C_SDA_READ()  GPIO_ReadInputDataBit(BSP_GPIO_PORT_I2C, BSP_I2C_SDA_PIN)	/* 读SDA口线状态 */
#define BSP_I2C_SDA_READ() ((BSP_GPIO_PORT_I2C->IDR & BSP_I2C_SDA_PIN) != 0) /* ?SDA???? */

#else                                                                              /* 这个分支选择直接寄存器操作实现IO读写 */
/*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
#define BSP_I2C_SCL_1() BSP_GPIO_PORT_I2C->BSRR = (uint32_t)BSP_I2C_SCL_PIN        /* SCL = 1 */
#define BSP_I2C_SCL_0() BSP_GPIO_PORT_I2C->BSRR = (uint32_t)BSP_I2C_SCL_PIN << 16U /* SCL = 0 */

#define BSP_I2C_SDA_1() BSP_GPIO_PORT_I2C->BSRR = (uint32_t)BSP_I2C_SDA_PIN        /* SDA = 1 */
#define BSP_I2C_SDA_0() BSP_GPIO_PORT_I2C->BSRR = (uint32_t)BSP_I2C_SDA_PIN << 16U /* SDA = 0 */

#define BSP_I2C_SDA_READ() ((BSP_GPIO_PORT_I2C->IDR & BSP_I2C_SDA_PIN) != 0) /* 读SDA口线状态 */
#endif

/* 直接操作寄存器的方法控制IO */
#define digitalH(p, i) \
    {                  \
        p->BSRR = i;   \
    }  // 设置为高电平
#define digitalL(p, i)               \
    {                                \
        p->BSRR = (uint32_t)i << 16; \
    } // 输出低电平

void i2c_CfgGpio(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t IIC_MEM_Read(uint8_t* data,uint8_t i2c_address,uint8_t mem_address);

typedef enum
{
    IIC_OK=1,
    IIC_ERROR=0
}IIC_STATE;

#endif

