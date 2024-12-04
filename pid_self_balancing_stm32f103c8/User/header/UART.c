#include "UART.h"

void Uart1_init(unsigned int baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	
	/* USART1 TX (PA9)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA9		Remap -> PB6
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1 RX (PA10)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA10	Remap -> PB7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);

	/* USART1 NVIC */
	#ifdef VECT_TAB_RAM
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	#else
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	// 打开串口中断
	USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{
	unsigned char SBUF;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		SBUF = USART_ReceiveData(USART1);
		USART_SendData(USART1, SBUF);
	}
}

void print(char *ptr)
{
	while(*ptr)
	{
		USART_SendData(USART1, *ptr);
		delay_i2c(10);
		ptr++;
	}
}

void println(char *ptr)
{
	while(*ptr)
	{
		USART_SendData(USART1, *ptr);
		delay_i2c(10);
		ptr++;
	}

	USART_SendData(USART1, '\n');
}
