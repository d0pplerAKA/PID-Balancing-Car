#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f10x.h"
#include "Delay.h"


#define I2C_SUCCESS                     0x0000
#define I2C_Error_Phase_1               0x0010      // Error code 16 in dec 16:未应答
#define I2C_Error_Phase_2               0x0020      // Error code 32 in dec 32:指令发送失败
#define I2C_Error_Phase_3               0x0030      // Error code 48 in dec 48:数据发送/读取失败


#define INPUT                           0x00
#define OUTPUT                          0x01

/* I2C_ADDR_MPU 0x68*/
#define I2C_ADDR_MPU                    0x68
#define I2C_ADDR_MPU_WRITE              0xD0    // MPU6050 Address write    0x68 << 1 | 0
#define I2C_ADDR_MPU_READ               0xD1    // MPU6050 Address read     0x68 << 1 | 1 

/*I2C MPU PIN*/
#define I2C_MPU_PIN_SCL                 GPIO_Pin_8
#define I2C_MPU_PIN_SDA                 GPIO_Pin_9

#define I2C_MPU_SCL_PINTYPE             GPIOB
#define I2C_MPU_SCL_PINAPB              RCC_APB2Periph_GPIOB

#define I2C_MPU_SDA_PINTYPE             GPIOB
#define I2C_MPU_SDA_PINAPB              RCC_APB2Periph_GPIOB

#define SCL_MPU_HIGH                    GPIO_SetBits(I2C_MPU_SCL_PINTYPE, I2C_MPU_PIN_SCL)
#define SCL_MPU_LOW                     GPIO_ResetBits(I2C_MPU_SCL_PINTYPE, I2C_MPU_PIN_SCL)

#define SDA_MPU_HIGH                    GPIO_SetBits(I2C_MPU_SDA_PINTYPE, I2C_MPU_PIN_SDA)
#define SDA_MPU_LOW                     GPIO_ResetBits(I2C_MPU_SDA_PINTYPE, I2C_MPU_PIN_SDA)

void I2C_Software_MPU_init(void);
void I2C_SCL_MPU_Init(void);
void I2C_SDA_MPU_Init(unsigned char mode);
void I2C_MPU_Start(void);
void I2C_MPU_Stop(void);
void I2C_MPU_Ack(void);
void I2C_MPU_NoAck(void);
unsigned int I2C_MPU_Wait_Ack(void);
unsigned int I2C_MPU_Software_WaitAck(unsigned int rtn);
void I2C_MPU_Send_Data(unsigned char data);
unsigned char I2C_MPU_Receive_Data(void);
unsigned int I2C_Write_MPU(unsigned char addr, unsigned char data);
unsigned int I2C_Read_MPU(unsigned char addr, unsigned char *data);

int Sensors_I2C_Write(unsigned char slave_addr, unsigned char reg_addr, 
                        unsigned char length, unsigned char const *data);

int Sensors_I2C_Read(unsigned char slave_addr, unsigned char reg_addr, 
                        unsigned char length, unsigned char *data);

unsigned int I2C_WriteBytes_MPU(unsigned char addr, unsigned char byteNum, unsigned char const *data);
unsigned int I2C_ReadBytes_MPU(unsigned char addr, unsigned char byteNum, unsigned char *data);

#endif
