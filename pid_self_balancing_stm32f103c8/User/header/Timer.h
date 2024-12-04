#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"
#include "MPU6050.h"
#include "MPU_Rotation.h"
#include "EncoderMotor.h"
#include "PID.h"

#define TIMER_SYS_TICK          1000       // 1000 * 72Mhz

extern volatile unsigned long current_Time;
extern volatile unsigned long ct;

void Timer1_Init(unsigned short int arr, unsigned short int psc);
void TIM1_UP_IRQHandler(void);   //TIM1中断

void Timer2_Init_EncoderA(void);
void TIM2_IRQHandler(void);     //TIM2中断

void Timer3_Init_PWM(unsigned short int arr, unsigned short int psc);
//void TIM3_IRQHandler(void);     //TIM3中断

void Timer4_Init_EncoderB(void);
void TIM4_IRQHandler(void);     //TIM4中断

void SysTick_Init(void);
void SysTick_Handler(void);

void Timer_getSystemTick(unsigned long *system_tick);
void get_tick(unsigned long *count);

/**
 * 
 * （计数周期 + 1） * （分频系数 + 1） / （计时器频率），定时器频率一般情况下都是默认的从AHB二分频之后再倍频得到的，所以说还是72M； 
 *  比如说，要定时100ms，即可写为：计数周期 = 999，分频系数 = 7199 ，即为（999 + 1） * （7199 + 1） / （72,000,000），因为
 * 
*/

#endif
