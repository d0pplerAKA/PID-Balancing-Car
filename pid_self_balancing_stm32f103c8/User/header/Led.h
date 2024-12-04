#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"
#include "Delay.h"

#define LED_OFF GPIO_SetBits(GPIOC, GPIO_Pin_13)    //拉高
#define LED_ON GPIO_ResetBits(GPIOC, GPIO_Pin_13)   //拉低
#define LED_REV GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(1 - (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))))

void Led_Init(void);
void blink(unsigned int times);

#endif
