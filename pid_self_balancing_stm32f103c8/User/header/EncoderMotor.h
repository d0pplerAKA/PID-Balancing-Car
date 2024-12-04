#ifndef __ENCODERMOTOR_H__
#define __ENCODERMOTOR_H__


#include "stm32f10x.h"
#include "Head.h"
#include "Delay.h"
#include "Timer.h"

#define MOTOR_L_PHASE_A                     GPIO_PinSource0
#define MOTOR_L_PHASE_B                     GPIO_PinSource1
#define MOTOR_R_PHASE_A                     GPIO_PinSource2
#define MOTOR_R_PHASE_B                     GPIO_PinSource3

#define Motor_Pin_A1                        GPIO_Pin_6
#define Motor_Pin_A2                        GPIO_Pin_7
#define Motor_Pin_B1                        GPIO_Pin_0
#define Motor_Pin_B2                        GPIO_Pin_1

#define MOTOR_DIR_L                         1
#define MOTOR_DIR_R                         0

#define MOTOR_PWM_ZONE_DEAD_L               6000
#define MOTOR_PWM_ZONE_DEAD_R               6000

#define MOTOR_PWM_ZONE_MAX                  65535

/*
    DRV8871支持至多100kHz PWM控制信号

    72M / (ARR + 1) / (PSC + 1)
            800         900     =    15000 = 100 Hz
*/
#define MOTOR_PWM_ARR					10
#define MOTOR_PWM_PSC					3600

#define MOTOR_SPEED_SAMPLE_RATE         20000

extern volatile int motor_currentSpeed;
extern int motor_l_currentSpeed;
extern int motor_r_currentSpeed;

extern volatile int PID_Median;
extern volatile float PID_output_pwm;

void Motor_Init(void);
//void Motor_phase_Init(void);
//void Motor_EXTI_Init(void);

void Motor_Set_DIR(char motor_dir);
void Motor_Set_PWM(float pwm_l, float pwm_r);

uint16_t Motor_PWM_Map_L(uint16_t pwm_target);
uint16_t Motor_PWM_Map_R(uint16_t pwm_target);

void Motor_ReadEncoder(uint8_t TIMx, volatile int *val);

#endif
