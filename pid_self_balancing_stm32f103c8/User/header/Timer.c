#include "Timer.h"

volatile unsigned long current_Time = 0;
volatile unsigned long ct = 0;


void Timer1_Init(unsigned short int arr, unsigned short int psc)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能  
    TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载寄存器周期值  
    TIM_TimeBaseStructure.TIM_Prescaler = psc;//设置预分频值 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式  
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数设置  
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //参数初始化  
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);//清中断标志位  
    TIM_ITConfig(      //使能或者失能指定的TIM中断  
    TIM1,            //TIM1  
    TIM_IT_Update  | //TIM 更新中断源  
    TIM_IT_Trigger,  //TIM 触发中断源   
    ENABLE           //使能  
    );  
    //设置优先级  
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占优先级0级  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //从优先级0级  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);   
}

void Timer2_Init_EncoderA(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能APB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	PWR_WakeUpPinCmd(DISABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period =  65535; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清中断标志位  

	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update ,//更新中断
		ENABLE  //使能
		);

	TIM_SetCounter(TIM2, 0);
}

void Timer4_Init_EncoderB(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能APB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = 65535; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清中断标志位

	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM4
		TIM_IT_Update ,//更新中断
		ENABLE  //使能
	);

	TIM_SetCounter(TIM4, 0);		 
}

/**********/
void Timer3_Init_PWM(unsigned short int arr, unsigned short int psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = Motor_Pin_A1 | Motor_Pin_A2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = Motor_Pin_B1 | Motor_Pin_B2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);

	//TIM_Cmd(TIM3, ENABLE);
}

void TIM1_UP_IRQHandler(void)   //TIM1中断
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM1中断发生与否:TIM1 中断源 
	{
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM1 中断源

        /* 这里写中断 */
        Holy_fuck();
        //MPU_MahonyFilter(&mpu, &mpu_rotation);
        //MPU_Attitude_Fusion(&mpu_rotation, MPU_KalmanFilter_Roll(&mpu), MPU_KalmanFilter_Pitch(&mpu));
	}
}

void TIM2_IRQHandler(void)   //TIM2中断
{ 
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)  // 
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  // 
	}
}

void TIM4_IRQHandler(void)   //TIM4中断
{ 
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)  // 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  // 
	}
}

void SysTick_Init(void)
{
	if(SysTick_Config(SystemCoreClock / TIMER_SYS_TICK))
	{
		while(1);
	}
	//SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                         //若无法启动则关闭
}
/*
void SysTick_Handler(void)
{
	current_Time++;
	ct++;

	if(ct == 10)
	{
		Motor_ReadEncoder(2, &motor_l_currentSpeed);
		Motor_ReadEncoder(3, &motor_r_currentSpeed);

		ct = 0;
	}
}
*/
void Timer_getSystemTick(unsigned long *system_tick)
{
	*system_tick = current_Time;
}

void get_tick(unsigned long *count)
{
	Timer_getSystemTick(count);
}
