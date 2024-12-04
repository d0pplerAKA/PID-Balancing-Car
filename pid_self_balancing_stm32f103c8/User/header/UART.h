#ifndef __UART_H__
#define __UART_H__

#include "stm32f10x.h"

#include "stdio.h"
#include "Delay.h"

#include "OLED_SH1106.h"

#define BAUD_RATE       921600

void Uart1_init(unsigned int baud);
void USART1_IRQHandler(void);
void print(char *ptr);
void println(char *ptr);

#endif
