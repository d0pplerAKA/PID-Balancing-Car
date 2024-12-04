#include "Delay.h"


void delay(unsigned int ms)
{
    ms *= 799;

    while(ms > 0)
    {
        //for(i_loop = 10; i_loop > 0; i_loop--)
        delay_1us();
        
        ms--;
    }
}

void delay_i2c(unsigned int ss)
{
    while(ss > 0)
    {
        delay_1us();
        //delay_1us();
        //delay_1us();
        //delay_1us();
        //delay_1us();
        //delay_1us();

        ss--;
    }
}

void delay_1us(void)
{
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();
}

void mdelay(unsigned long num_ms)
{
    delay(num_ms);
}
