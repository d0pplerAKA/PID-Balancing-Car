#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f10x.h"

//#define DELAY_TIME                  10000

#define I2CX_DELAY                      2
#define I2CX_TIMEOUT                    2000

void delay(unsigned int ms);
void delay_i2c(unsigned int ss);
void delay_1us(void);

void mdelay(unsigned long num_ms);


#endif
