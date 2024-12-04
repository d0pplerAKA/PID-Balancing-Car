#include "Head.h"

MPU6050_Raw_Data mpu;
MPU_Rotation mpu_rotation;

int DEBUG_OUTPUT = 0;


void DEBUG_UART(unsigned char data)
{
    USART_SendData(USART1, data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void DEBUG_OLED(unsigned char data)
{
    if(data == '\n')
    {
        OLED_newLine();
    }
    else if(data == ' ')
    {
        OLED_space();
    }
    else
    {
        //32 ~ 126

        if(data >= '!' && data <= '/')
            OLED_Print_Symbol1(data);
        else if(data >= '0' && data <= '9')
            OLED_Print_Number(data);
        else if(data >= ':' && data <= '@')
            OLED_Print_Symbol2(data);
        else if(data >= 'A' && data <= 'Z')
            OLED_Print_Letter_Caps(data);
        else if(data >= '[' && data <= ']')
            OLED_Print_Symbol3(data);
        else if(data >= 'a' && data <= 'z')
            OLED_Print_Letter_noCaps(data);
        else if(data >= '{' && data <= '~')
            OLED_Print_Symbol4(data);
        else
            OLED_error();
    }
}

void Sys_Reset(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

int fputc(int data, FILE *f)
{
	if(DEBUG_OUTPUT == OLED)
    {
        DEBUG_OLED((unsigned char) data);
    }
    else if(DEBUG_OUTPUT == UART)
    {
        DEBUG_UART((unsigned char) data);
    }
    else
    {
        DEBUG_UART((unsigned char) data);
        DEBUG_OLED((unsigned char) data);
    }
	
	return data;
}
