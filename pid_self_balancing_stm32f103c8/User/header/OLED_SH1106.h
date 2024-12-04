#ifndef __OLED_SH1106_H__
#define __OLED_SH1106_H__

#include "stm32f10x.h"

#include "OLED_Font.h"

#include "Delay.h"
#include "UART.h"
#include "Head.h"

#define GPIO_Pin_OLED_SCLK                  GPIO_Pin_3      //GPIOB
#define GPIO_Pin_OLED_DC                    GPIO_Pin_4      //GPIOB
#define GPIO_Pin_OLED_SDAT                  GPIO_Pin_5      //GPIOB

#define GPIO_Pin_OLED_CS                    GPIO_Pin_15     //GPIOA
#define GPIO_Pin_OLED_RES                   GPIO_Pin_12     //GPIOA

#define OLED_CMD                            0
#define OLED_DAT                            1

#define OLED_FILL                           0xFF
#define OLED_UNFILL                         0x00

#define HIGH                                1
#define LOW                                 0

#define OLED_SCLK_HIGH                      GPIO_SetBits(GPIOB, GPIO_Pin_OLED_SCLK)
#define OLED_SCLK_LOW                       GPIO_ResetBits(GPIOB, GPIO_Pin_OLED_SCLK)
#define OLED_SDAT_HIGH                      GPIO_SetBits(GPIOB, GPIO_Pin_OLED_SDAT)
#define OLED_SDAT_LOW                       GPIO_ResetBits(GPIOB, GPIO_Pin_OLED_SDAT)
#define OLED_DC_HIGH                        GPIO_SetBits(GPIOB, GPIO_Pin_OLED_DC)
#define OLED_DC_LOW                         GPIO_ResetBits(GPIOB, GPIO_Pin_OLED_DC)
#define OLED_CS_HIGH                        GPIO_SetBits(GPIOA, GPIO_Pin_OLED_CS)
#define OLED_CS_LOW                         GPIO_ResetBits(GPIOA, GPIO_Pin_OLED_CS)
#define OLED_RES_HIGH                       GPIO_SetBits(GPIOA, GPIO_Pin_OLED_RES)
#define OLED_RES_LOW                        GPIO_ResetBits(GPIOA, GPIO_Pin_OLED_RES)

#define OLED_X                              128
#define OLED_Y                              64


extern unsigned char OLED_GRAM[128][8];

void OLED_Init(void);
void OLED_GPIO_Init(void);

void OLED_SCLK(unsigned char status);
void OLED_SDAT(unsigned char status);
void OLED_DC(unsigned char status);
void OLED_CSS(unsigned char status);
void OLED_RES(unsigned char status);

void OLED_Write_Cmd(unsigned char cmd);
void OLED_Write_Data(unsigned char data);
void OLED_Cursor_Operation(int offset);

void OLED_Reset(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);

void OLED_ClearDisplay(void);
void OLED_FillScreen(unsigned char dat);
void OLED_setFont(unsigned int font);

void OLED_GRAM_Clear(void);
void OLED_GRAM_Push(void);

void OLED_setCursor(unsigned char x, unsigned char y);
void OLED_resetCursor(void);
void OLED_space(void);
void OLED_error(void);
void OLED_newLine(void);

void OLED_Print_Letter_Caps(char c);
void OLED_Print_Letter_noCaps(char c);
void OLED_Print_Symbol1(char c);
void OLED_Print_Symbol2(char c);
void OLED_Print_Symbol3(char c);
void OLED_Print_Symbol4(char c);
void OLED_Print_Number(char c);

void OLED_drawPixel(unsigned char x, unsigned char y);


void OLED_Loading(uint8_t s);

#endif
