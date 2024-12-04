#include "I2c.h"

void I2C_Software_MPU_init(void)
{
    I2C_SCL_MPU_Init();
    I2C_SDA_MPU_Init(OUTPUT);

    delay(25);
}

void I2C_SCL_MPU_Init(void)
{
    RCC_APB2PeriphClockCmd(I2C_MPU_SCL_PINAPB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = I2C_MPU_PIN_SCL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_MPU_SCL_PINTYPE, &GPIO_InitStructure);

    SCL_MPU_HIGH;
}

void I2C_SDA_MPU_Init(unsigned char mode)
{
    RCC_APB2PeriphClockCmd(I2C_MPU_SDA_PINAPB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    if(mode == INPUT)
    {
        GPIO_InitStructure.GPIO_Pin = I2C_MPU_PIN_SDA;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(I2C_MPU_SDA_PINTYPE, &GPIO_InitStructure);
    }
    else 
    {
        GPIO_InitStructure.GPIO_Pin = I2C_MPU_PIN_SDA;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        SDA_MPU_HIGH;
    }
}

void I2C_MPU_Start(void)
{
    I2C_SDA_MPU_Init(OUTPUT);

    SDA_MPU_HIGH;
    SCL_MPU_HIGH;
    delay_i2c(I2CX_DELAY);
    SDA_MPU_LOW;
    SCL_MPU_LOW;
}

void I2C_MPU_Stop(void)
{
    I2C_SDA_MPU_Init(OUTPUT);

    SDA_MPU_LOW;
    SCL_MPU_HIGH;
    delay_i2c(I2CX_DELAY);
    SDA_MPU_HIGH;
    delay_i2c(I2CX_DELAY);
}

void I2C_MPU_Ack(void)
{
    I2C_SDA_MPU_Init(OUTPUT);

    SDA_MPU_LOW;
    delay_i2c(I2CX_DELAY);

    SCL_MPU_HIGH;
    SCL_MPU_LOW;
    delay_i2c(I2CX_DELAY);
}

void I2C_MPU_NoAck(void)
{
    I2C_SDA_MPU_Init(OUTPUT);

    SDA_MPU_HIGH;
    delay_i2c(I2CX_DELAY);

    SCL_MPU_HIGH;
    SCL_MPU_LOW;
    delay_i2c(I2CX_DELAY);
}

unsigned int I2C_MPU_Wait_Ack(void)
{
    unsigned int flag = 0;
    
    I2C_SDA_MPU_Init(INPUT);
    SCL_MPU_HIGH;
    delay_i2c(I2CX_DELAY);

    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1)
    {
        flag++;

        if(flag == I2CX_TIMEOUT)
            return 0;
    }

    SCL_MPU_LOW;
    delay_i2c(I2CX_DELAY);

    return 1;
}

unsigned int I2C_MPU_Software_WaitAck(unsigned int rtn)
{
    if(I2C_MPU_Wait_Ack() == 0)
    {
        I2C_MPU_Stop();
        
        return rtn;
    }

    return 0;
}

void I2C_MPU_Send_Data(unsigned char data)
{
    I2C_SDA_MPU_Init(OUTPUT);

    for(uint8_t i = 0; i < 8; i++)
    {   
        if((data & 0x80) >> 7)
        {
            SDA_MPU_HIGH;
        }
        else
        {
            SDA_MPU_LOW;
        }
        data <<= 1;
        SCL_MPU_HIGH;
        delay_i2c(I2CX_DELAY);

        SCL_MPU_LOW;
    }
}

unsigned char I2C_MPU_Receive_Data(void)
{
    unsigned char data = 0x00;

    I2C_SDA_MPU_Init(INPUT);
    delay_i2c(I2CX_DELAY);

    for(uint8_t i = 0; i < 8; i++)
    {
        SCL_MPU_HIGH;
        
        data = (data << 1) | GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);

        SCL_MPU_LOW;
        delay_i2c(I2CX_DELAY);
    }

    return data;
}

unsigned int I2C_Write_MPU(unsigned char addr, unsigned char data)
{
    unsigned int error_return = 0;
    I2C_MPU_Start();

    I2C_MPU_Send_Data(I2C_ADDR_MPU_WRITE);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_1);
    if(error_return != 0)
        return error_return;

    I2C_MPU_Send_Data(addr);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_2);
    if(error_return != 0)
        return error_return;
    
    I2C_MPU_Send_Data(data);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_3);
    if(error_return != 0)
        return error_return;

    I2C_MPU_Stop();

    return 0;
}

unsigned int I2C_Read_MPU(unsigned char addr, unsigned char *data)
{
    unsigned int error_return = 0;

    I2C_MPU_Start();

    I2C_MPU_Send_Data(I2C_ADDR_MPU_WRITE);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_1);
    if(error_return != 0)
        return error_return;

    I2C_MPU_Send_Data(addr);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_2);
    if(error_return != 0)
        return error_return;

    I2C_MPU_Start();

    I2C_MPU_Send_Data(I2C_ADDR_MPU_READ);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_3);
    if(error_return != 0)
        return error_return;

    *data = I2C_MPU_Receive_Data();

    I2C_MPU_NoAck();

    I2C_MPU_Stop();

    return 0;
}


/* The following functions must be defined for this platform:
 * i2c_write(unsigned char slave_addr, unsigned char reg_addr,
 *      unsigned char length, unsigned char const *data)
 * i2c_read(unsigned char slave_addr, unsigned char reg_addr,
 *      unsigned char length, unsigned char *data)
 * delay_ms(unsigned long num_ms)
 * get_ms(unsigned long *count)
 * reg_int_cb(void (*cb)(void), unsigned char port, unsigned char pin)
 * labs(long x)
 * fabsf(float x)
 * min(int a, int b)
 */

int Sensors_I2C_Write(unsigned char slave_addr, unsigned char reg_addr, 
                        unsigned char length, unsigned char const *data)
{
    return I2C_WriteBytes_MPU(reg_addr, length, data);
}

int Sensors_I2C_Read(unsigned char slave_addr, unsigned char reg_addr, 
                        unsigned char length, unsigned char *data)
{
    return I2C_ReadBytes_MPU(reg_addr, length, data);   
}

unsigned int I2C_WriteBytes_MPU(unsigned char addr, unsigned char byteNum, unsigned char const *data)
{
    uint8_t counter = byteNum;
    unsigned int error_return = 0;

    I2C_MPU_Start();
    
    I2C_MPU_Send_Data(I2C_ADDR_MPU_WRITE);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_1);
    if(error_return != 0)
        return error_return;

    I2C_MPU_Send_Data(addr);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_2);
    if(error_return != 0)
        return error_return;
    
    while(counter--)
    {
        I2C_MPU_Send_Data(*(data++));
        error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_3);
        if(error_return != 0)
            return error_return;
    }

    I2C_MPU_Stop();


    return 0;
}

unsigned int I2C_ReadBytes_MPU(unsigned char addr, unsigned char byteNum, unsigned char *data)
{
    uint8_t counter = byteNum;
    unsigned int error_return = 0;

    I2C_MPU_Start();

    I2C_MPU_Send_Data(I2C_ADDR_MPU_WRITE);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_1);
    if(error_return != 0)
        return error_return;

    I2C_MPU_Send_Data(addr);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_2);
    if(error_return != 0)
        return error_return;

    I2C_MPU_Start();

    I2C_MPU_Send_Data(I2C_ADDR_MPU_READ);
    error_return = I2C_MPU_Software_WaitAck(I2C_Error_Phase_3);
    if(error_return != 0)
        return error_return;

    while(counter--)
    {
        *(data++) = I2C_MPU_Receive_Data();
        counter == 0 ? I2C_MPU_NoAck() : I2C_MPU_Ack();
    }

    I2C_MPU_Stop();

    return 0;
}
