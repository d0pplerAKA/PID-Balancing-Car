#include "stm32f10x.h"
#include "stdlib.h"
#include "Head.h"
#include "Delay.h"
#include "UART.h"
#include "Led.h"
#include "MPU6050.h"
#include "MPU_Rotation.h"
#include "Timer.h"
#include "OLED_SH1106.h"
#include "EncoderMotor.h"
#include "PID.h"
#include "Receiver.h"


void program_Init(void)
{
	delay(850);
	Uart1_init(BAUD_RATE);
	OLED_Init();
	OLED_Reset();
	OLED_ClearDisplay();
	OLED_setFont(FONT_8X8);

	DEBUG_OUTPUT = DEBUG_ALL;
	printf("Initializing...\n");
	OLED_GRAM_Push();
	delay(500);

	Led_Init();
	DEBUG_OUTPUT = DEBUG_ALL;
	printf("LED...\n");
	OLED_GRAM_Push();
	delay(50);

	Motor_Init();
	DEBUG_OUTPUT = DEBUG_ALL;
	printf("Motor...\n");
	OLED_GRAM_Push();
	delay(50);

	I2C_Software_MPU_init();
	DEBUG_OUTPUT = DEBUG_ALL;
	printf("I2C...\n");
	OLED_GRAM_Push();
	delay(50);
	
	Timer1_Init(71, 9999);
	Timer2_Init_EncoderA();
	Timer3_Init_PWM(MOTOR_PWM_ARR - 1, MOTOR_PWM_PSC - 1);
	Timer4_Init_EncoderB();
	DEBUG_OUTPUT = DEBUG_ALL;
	printf("Timer...\n");
	OLED_GRAM_Push();
	delay(50);

	MPU_Init();
	DEBUG_OUTPUT = UART;
	unsigned char addr;
	MPU_Get_Self_Address(&addr);
	printf("Hello World!\n");
	printf("Who Am I(MPU6050 I2C Address): 0x%x\n\n", addr);
	DEBUG_OUTPUT = DEBUG_ALL;
	printf("MPU6050...\n");
	OLED_GRAM_Push();
	delay(100);
	OLED_GRAM_Clear();

	DEBUG_OUTPUT = OLED;
	printf("Preparing DMP...\n");
	OLED_GRAM_Push();
	delay(100);
	printf("Please hold your\n");
	printf("device stably!\n");
	OLED_GRAM_Push();
	SysTick_Init();

	uint8_t res = MPU_DMP_init();
	if(res != 0)
	{
		printf("DMP init failed!\n");
		printf("EXIT Code: %d\n", res);
		printf("Restarting... \n");
		OLED_GRAM_Push();
		delay(1000);
		
		Sys_Reset();
	}
	else
	{
		printf("DMP init success!\n");
		printf("MPU6050 start!\n");
		OLED_GRAM_Push();
		delay(500);
	}

	TIM_Cmd(TIM1, ENABLE);
//	TIM_Cmd(TIM2, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);
//	TIM_Cmd(TIM4, ENABLE);

	delay(300);
	
	DEBUG_OUTPUT = DEBUG_ALL;
	printf("Finished!\n");
	OLED_GRAM_Push();
	delay(500);
	
	OLED_GRAM_Clear();
	OLED_setFont(FONT_16X16);
	
	blink(5);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

int main(void)
{
	program_Init();
	
	while(1)
	{
		//Motor_Set_PWM(PID_output_pwm - pid_orientation, PID_output_pwm + pid_orientation);
		
		DEBUG_OUTPUT = OLED;
		OLED_GRAM_Clear();
		
		printf("P:%.2f\n", mpu_rotation.pitch);
		printf("OP %.0f\n", PID_output_pwm);
		printf("PD %.0f\n", PID_Struct_degree.val_output);
		printf("PI %.0f\n", PID_Struct_motor.val_output);
		
		OLED_GRAM_Push();
		//DEBUG_OUTPUT = UART;
		//printf("p=%.2f,reference=%.2f,output=%.2f\r\n", mpu_rotation.pitch * 150, PID_Struct_motor.val_target, PID_output_pwm);
	}
}

