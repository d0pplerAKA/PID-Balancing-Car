#include "EncoderMotor.h"

char motor_dir = 0;

volatile int motor_currentSpeed = 0;
int motor_l_currentSpeed = 0;
int motor_r_currentSpeed = 0;

volatile int PID_Median = 0;
volatile float PID_output_pwm = 0;

void Motor_Init(void)
{
    //Motor_phase_Init();
    //Motor_EXTI_Init();

    Motor_Set_DIR(MOTOR_DIR_L);
}
/*
void Motor_EXTI_Init(void)
{
    EXTI_InitTypeDef            EXTI_InitStruct;
    NVIC_InitTypeDef            NVIC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, MOTOR_L_PHASE_A);

    EXTI_InitStruct.EXTI_Line = EXTI_Line1;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, MOTOR_L_PHASE_B);
    
    EXTI_InitStruct.EXTI_Line = EXTI_Line2;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, MOTOR_R_PHASE_A);
    
    EXTI_InitStruct.EXTI_Line = EXTI_Line3;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, MOTOR_R_PHASE_B);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}
*/

/*
void Motor_phase_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //PWR_WakeUpPinCmd(DISABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
}
*/
void Motor_Set_DIR(char dir)
{
    motor_dir = dir;
}

void Motor_Set_PWM(float pwm_l, float pwm_r)
{
    if(mpu_rotation.pitch < 45 && mpu_rotation.pitch >= -45)
    {
        if(pwm_l > 0)
        {
            TIM_SetCompare1(TIM3, 1);
            TIM_SetCompare2(TIM3, Motor_PWM_Map_L((uint16_t) pwm_l));
        }
        else if(pwm_l == 0)
        {
            TIM_SetCompare1(TIM3, 1);
            TIM_SetCompare2(TIM3, 1);
        }
        else
        {
            TIM_SetCompare1(TIM3, Motor_PWM_Map_L((uint16_t) pwm_l * -1.0f));
            TIM_SetCompare2(TIM3, 1);
        }

        if(pwm_r < 0)
        {
            
            TIM_SetCompare3(TIM3, 1);
            TIM_SetCompare4(TIM3, Motor_PWM_Map_R((uint16_t) pwm_r * -1.0f));
        }
        else if(pwm_l == 0)
        {
            TIM_SetCompare3(TIM3, 1);
            TIM_SetCompare4(TIM3, 1);
        }
        else
        {
            TIM_SetCompare3(TIM3, Motor_PWM_Map_R((uint16_t) pwm_r));
            TIM_SetCompare4(TIM3, 1);
        }
    }
    else
    {
        TIM_SetCompare1(TIM3, 5);
        TIM_SetCompare2(TIM3, 5);
        TIM_SetCompare3(TIM3, 5);
        TIM_SetCompare4(TIM3, 5);
    }
}

uint16_t Motor_PWM_Map_L(uint16_t pwm_target)
{
    if(pwm_target == 0)
        return pwm_target;
    
    if(pwm_target > 10000)
        pwm_target = 10000;
    
    float pwm_ratio = (float) ((float)pwm_target / 10000.0f);

    uint16_t pwm_final = (uint16_t) ((float) (MOTOR_PWM_ZONE_MAX - MOTOR_PWM_ZONE_DEAD_L) * pwm_ratio);

    pwm_final += MOTOR_PWM_ZONE_DEAD_L;

    return pwm_final;
}

uint16_t Motor_PWM_Map_R(uint16_t pwm_target)
{
    if(pwm_target == 0)
        return pwm_target;
    
    if(pwm_target > 10000)
        pwm_target = 10000;
    
    float pwm_ratio = (float) ((float)pwm_target / 10000.0f);

    uint16_t pwm_final = (uint16_t) ((float) (MOTOR_PWM_ZONE_MAX - MOTOR_PWM_ZONE_DEAD_R) * pwm_ratio);

    pwm_final += MOTOR_PWM_ZONE_DEAD_R;

    return pwm_final;
}

int Motor_Phase_Map(int motor_phase)
{
    if(PID_output_pwm == 0)
        return 0;
    
    float phase_ratio = ((float) motor_phase / 100.0f);
    if(phase_ratio > 1)
        phase_ratio = 1;

    int motor_final_speed_output = (int) ((float) (10000.0f * phase_ratio));
	
	return motor_final_speed_output;
}

void Motor_ReadEncoder(uint8_t TIMx, volatile int *val)
{
    switch(TIMx)
    {
        case 2: {
            int _val = (short) TIM_GetCounter(TIM2);
            TIM_SetCounter(TIM2, 0);
            *val = _val;
            break;
        }
        case 4: {
            int _val = (short) TIM_GetCounter(TIM4);
            TIM_SetCounter(TIM4, 0);
            *val = _val;
            break;
        }
        default: *val = 0;
    }
}

/*
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        

		motor_l_pulse++;
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        

		motor_r_pulse++;
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI2_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        

        motor_l_pulse++;
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

void EXTI3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        

		motor_r_pulse++;
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}
*/
