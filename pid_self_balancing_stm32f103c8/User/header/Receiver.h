#ifndef __RECEIVER_H__
#define __RECEIVER_H__


#include "stm32f10x.h"


#define NRF_Pin_CE          GPIO_Pin_11//GPIOA
#define NRF_Pin_CS          GPIO_Pin_12
#define NRF_Pin_MOSI        GPIO_Pin_15
#define NRF_Pin_MISO        GPIO_Pin_14
#define NRF_Pin_SCK         GPIO_Pin_13

#define NRF_Timeout         200

void NRF_Init(void);



#endif
