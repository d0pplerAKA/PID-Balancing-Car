#include "OLED_SH1106.h"

unsigned char OLED_GRAM[128][8];

unsigned char OLED_GRAM_COL[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

unsigned char   OLED_Cursor_X = 0;
unsigned char   OLED_Cursor_Y = 0;

unsigned int OLED_Font = FONT_8X8;

void OLED_Init(void)
{
    OLED_GPIO_Init();

    OLED_CSS(LOW);
    OLED_RES(HIGH);
    OLED_DC(HIGH);

    /* 复位 */
    OLED_Reset();
}

void OLED_GPIO_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_OLED_CS | GPIO_Pin_OLED_RES;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_OLED_DC | GPIO_Pin_OLED_SCLK | GPIO_Pin_OLED_SDAT;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void OLED_Reset(void)
{
    /* 复位 */
    OLED_RES(LOW);
    delay(100);
    OLED_RES(HIGH);
    delay(100);

	/* 开始写入初始化命令 */

    OLED_Display_Off();

    OLED_Write_Cmd(0x02);
    OLED_Write_Cmd(0x10);
    OLED_Write_Cmd(0x40);
    OLED_Write_Cmd(0xB0);
    OLED_Write_Cmd(0x81);
    OLED_Write_Cmd(0xCF);
    OLED_Write_Cmd(0xA1);
    OLED_Write_Cmd(0xC8);
    OLED_Write_Cmd(0xA7);
    OLED_Write_Cmd(0xA8);
    OLED_Write_Cmd(0x3F);
    OLED_Write_Cmd(0xD3);
    OLED_Write_Cmd(0x00);
    OLED_Write_Cmd(0xD5);
    OLED_Write_Cmd(0x80);
    OLED_Write_Cmd(0xD9);
    OLED_Write_Cmd(0x22);
    OLED_Write_Cmd(0xDA);
    OLED_Write_Cmd(0x12);
    OLED_Write_Cmd(0xDB);
    OLED_Write_Cmd(0x40);
    OLED_Write_Cmd(0xA4);
    OLED_Write_Cmd(0xA6);

    OLED_Display_On();

    OLED_ClearDisplay();
}

void OLED_ClearDisplay(void)
{
    OLED_GRAM_Clear();
    OLED_GRAM_Push();
}

void OLED_Display_On(void)
{
    OLED_Write_Cmd(0x8D);
    OLED_Write_Cmd(0x14);
    OLED_Write_Cmd(0xAF);
}

void OLED_Display_Off(void)
{
    OLED_Write_Cmd(0x8D);
    OLED_Write_Cmd(0x10);
    OLED_Write_Cmd(0xAE);
}


void OLED_GRAM_Clear(void)
{
    unsigned char i, j;

    OLED_resetCursor();

    for(i = 0; i < 128; i++)
        for(j = 0; j < 8; j++)
            OLED_GRAM[i][j] = 0x00;
}

void OLED_resetCursor(void)
{
    OLED_Cursor_X = 0;
    OLED_Cursor_Y = 0;
}

void OLED_GRAM_Push(void)
{
    unsigned char i, j;

    for(i = 0; i < 8; i++)
    {
        OLED_Write_Cmd(0xB0 + i);
        OLED_Write_Cmd(0x00 + 0x02);
        OLED_Write_Cmd(0x10);

        for(j = 0; j < 128; j++)
            OLED_Write_Data(OLED_GRAM[j][i]);
    }
}

void OLED_FillScreen(unsigned char dat)
{
    unsigned char i, j;

    for(i = 0; i < 8; i++)
    {
        OLED_Write_Cmd(0xB0 + i);
        OLED_Write_Cmd(0x00 + 0x02);
        OLED_Write_Cmd(0x10);

        for(j = 0; j < 128; j++)
            OLED_Write_Data(dat);
    }
}

void OLED_setCursor(unsigned char x, unsigned char y)
{
    OLED_Cursor_X = x;
    OLED_Cursor_Y = y;
}

void OLED_newLine(void)
{
    switch(OLED_Font)
    {
        case FONT_8X8:
            OLED_Cursor_X = 0;
            OLED_Cursor_Y += 8;
            break;
        case FONT_16X16:
            OLED_Cursor_X = 0;
            OLED_Cursor_Y += 16;
            break;
        
        default:
            __nop();
    }
}

void OLED_space(void)
{
    switch(OLED_Font)
    {
        case FONT_8X8:
            OLED_Cursor_X += 6;
            break;
        case FONT_16X16:
            OLED_Cursor_X += 8;
            break;
        
        default:
            OLED_Cursor_X += 6;
    }
}

void OLED_error(void)
{
    switch(OLED_Font)
    {
        case FONT_8X8:
        {
            for(uint8_t i = 0; i < 8; i++)
            {
                for(uint8_t j = 0; j < 8; j++)
                {                        
                    if((0xff >> (7 - j)) & 0x01)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(8);

            break;
        }
        case FONT_16X16:
        {
            for(uint8_t i = 0; i < 8; i++)
            {
                for(uint8_t j = 0; j < 8; j++)
                {                        
                    if((0xffff >> (7 - j)) & 0x0001)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(16);

            break;
        }

        default:
            __nop();
    }
}

void OLED_Print_Letter_Caps(char c)
{
    uint64_t letter;
    
    switch(OLED_Font)
    {
        case FONT_8X8:
        {
            for(uint8_t i = 0; i < 8; i++)
            {
                for(uint8_t j = 0; j < 8; j++)
                {
                    letter = Font_Letter_8x8_A_to_Z[c - 65][i];
                        
                    if((letter >> (7 - j)) & 0x01)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(8);

            break;
        }
        case FONT_16X16:
        {
            for(uint8_t i = 0; i < 16; i++)
            {
                for(uint8_t j = 0; j < 16; j++)
                {
                    letter = Font_Letter_16x16_A_to_Z[c - 65][i];
                        
                    if((letter >> (16 - j)) & 0x0001)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(14);

            break;
        }

        default:
            __nop();
    }
}

void OLED_Print_Letter_noCaps(char c)
{
    uint64_t letter;
    
    switch(OLED_Font)
    {
        case FONT_8X8:
        {
            for(uint8_t i = 0; i < 8; i++)
            {
                for(uint8_t j = 0; j < 8; j++)
                {
                    letter = Font_Letter_8x8_a_to_z[c - 97][i];
                        
                    if((letter >> (7 - j)) & 0x01)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(6);

            break;
        }
        case FONT_16X16:
        {
            for(uint8_t i = 0; i < 16; i++)
            {
                for(uint8_t j = 0; j < 16; j++)
                {
                    letter = Font_Letter_16x16_a_to_z[c - 97][i];
                        
                    if((letter >> (16 - j)) & 0x0001)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(10);

            break;
        }

        default:
            __nop();
    }
}

void OLED_Print_Symbol1(char c)
{
    uint64_t letter;
    
    switch(OLED_Font)
    {
        case FONT_8X8:
        {
            for(uint8_t i = 0; i < 8; i++)
            {
                for(uint8_t j = 0; j < 8; j++)
                {
                    letter = Font_Symbol1_8x8[c - 33][i];
                        
                    if((letter >> (7 - j)) & 0x01)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(7);

            break;
        }
        case FONT_16X16:
        {
            for(uint8_t i = 0; i < 16; i++)
            {
                for(uint8_t j = 0; j < 16; j++)
                {
                    letter = Font_Symbol1_16x16[c - 33][i];
                        
                    if((letter >> (16 - j)) & 0x0001)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(16);

            break;
        }

        default:
            __nop();
    }
}

void OLED_Print_Symbol2(char c)
{
    uint64_t letter;
    
    switch(OLED_Font)
    {
        case FONT_8X8:
        {
            for(uint8_t i = 0; i < 8; i++)
            {
                for(uint8_t j = 0; j < 8; j++)
                {
                    letter = Font_Symbol2_8x8[c - 58][i];
                        
                    if((letter >> (7 - j)) & 0x01)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(7);

            break;
        }
        case FONT_16X16:
        {
            for(uint8_t i = 0; i < 16; i++)
            {
                for(uint8_t j = 0; j < 16; j++)
                {
                    letter = Font_Symbol2_16x16[c - 58][i];
                        
                    if((letter >> (16 - j)) & 0x0001)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(16);

            break;
        }
        
        default:
            __nop();
    }
}

void OLED_Print_Symbol3(char c)
{
    uint64_t letter;
    
    switch(OLED_Font)
    {
        case FONT_8X8:
        {
            for(uint8_t i = 0; i < 8; i++)
            {
                for(uint8_t j = 0; j < 8; j++)
                {
                    letter = Font_Symbol3_8x8[c - 91][i];
                        
                    if((letter >> (7 - j)) & 0x01)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(7);

            break;
        }
        case FONT_16X16:
        {
            for(uint8_t i = 0; i < 16; i++)
            {
                for(uint8_t j = 0; j < 16; j++)
                {
                    letter = Font_Symbol3_16x16[c - 91][i];
                        
                    if((letter >> (16 - j)) & 0x0001)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(16);
            break;
        }

        default:
            __nop();
    }
}

void OLED_Print_Symbol4(char c)
{
    uint64_t letter;
    
    switch(OLED_Font)
    {
        case FONT_8X8:
        {
            for(uint8_t i = 0; i < 8; i++)
            {
                for(uint8_t j = 0; j < 8; j++)
                {
                    letter = Font_Symbol4_8x8[c - 123][i];
                        
                    if((letter >> (7 - j)) & 0x01)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(7);

            break;
        }
        case FONT_16X16:
        {
            for(uint8_t i = 0; i < 16; i++)
            {
                for(uint8_t j = 0; j < 16; j++)
                {
                    letter = Font_Symbol4_16x16[c - 123][i];
                        
                    if((letter >> (16 - j)) & 0x0001)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(16);

            break;
        }

        default:
            __nop();
    }
}

void OLED_Print_Number(char c)
{
    uint64_t letter;
    
    switch(OLED_Font)
    {
        case FONT_8X8:
        {
            for(uint8_t i = 0; i < 8; i++)
            {
                for(uint8_t j = 0; j < 8; j++)
                {
                    letter = Font_Number_8x8[c - 48][i];
                        
                    if((letter >> (7 - j)) & 0x01)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(8);

            break;
        }
        case FONT_16X16:
        {
            for(uint8_t i = 0; i < 16; i++)
            {
                for(uint8_t j = 0; j < 16; j++)
                {
                    letter = Font_Number_16x16[c - 48][i];
                        
                    if((letter >> (16 - j)) & 0x0001)
                        OLED_drawPixel(i + OLED_Cursor_X, j + OLED_Cursor_Y);
                }
            }

            OLED_Cursor_Operation(16);

            break;
        }

        default:
            __nop();
    }
}


void OLED_drawPixel(unsigned char x, unsigned char y)
{
    for(unsigned char i = 0; i < 8; i++)
    {
        if (y % 8 == i)
        {
            OLED_GRAM[x][y / 8] |= OLED_GRAM_COL[i];
            break;
        }
    }
}

void OLED_setFont(unsigned int font)
{
    switch(font)
    {
        case FONT_8X8:
            OLED_Font = FONT_8X8;
            break;
        case FONT_16X16:
            OLED_Font = FONT_16X16;
            break;
        
        default:
            OLED_Font = FONT_8X8;
    }
}

void OLED_SCLK(unsigned char status)
{
    if(status)
    {
        OLED_SCLK_HIGH;
    }
    else
    {
        OLED_SCLK_LOW;
    }
}

void OLED_SDAT(unsigned char status)
{
    if(status)
    {
        OLED_SDAT_HIGH;
    }
    else
    {
        OLED_SDAT_LOW;
    }
}

void OLED_DC(unsigned char status)
{
    if(status)
    {
        OLED_DC_HIGH;
    }
    else
    {
        OLED_DC_LOW;
    }
}

void OLED_CSS(unsigned char status)
{
    if(status)
    {
        OLED_CS_HIGH;
    }
    else
    {
        OLED_CS_LOW;
    }
}

void OLED_RES(unsigned char status)
{
    if(status)
    {
        OLED_RES_HIGH;
    }
    else
    {
        OLED_RES_LOW;
    }
}

void OLED_Cursor_Operation(int offset)
{
    if(OLED_Cursor_X >= OLED_X - offset)
    {
        if(OLED_Cursor_Y >= OLED_Y - offset)
        {
            OLED_Cursor_X = 0;
            OLED_Cursor_Y = 0;
        }

        OLED_Cursor_X = 0;
        OLED_Cursor_Y += offset;
    }
    else
    {
        OLED_Cursor_X += offset;
    } 
}

void OLED_Write_Cmd(unsigned char cmd)
{
    OLED_DC(LOW);

    for(unsigned char i = 0; i < 8; i++)
    {
        OLED_SCLK(LOW);
        OLED_SDAT((cmd & (0x80)));
        OLED_SCLK(HIGH);

        cmd <<= 1;
    }

    OLED_DC(HIGH);
}

void OLED_Write_Data(unsigned char data)
{
    OLED_DC(HIGH);

    for(unsigned char i = 0; i < 8; i++)
    {
        OLED_SCLK(LOW);
        OLED_SDAT((data & 0x80));
        OLED_SCLK(HIGH);

        data <<= 1;
    }

    OLED_DC(HIGH);
}

void OLED_Loading(uint8_t s)
{
    OLED_GRAM_Clear();
	for(uint8_t k = 0; k < s; k++)
	{
		uint8_t load = 19;
		OLED_GRAM_Clear();
		DEBUG_OUTPUT = DEBUG_ALL;
		printf("Features Loading.\n");
		printf("Usually takes 5(s).\n");
		OLED_GRAM_Push();

		for(uint8_t i = load - 2; i < 90 + load + 2; i++)
		{
			OLED_drawPixel(i, 46);
			OLED_drawPixel(i, 52);
		}

		for(uint8_t i = 46; i < 52; i++)
		{
			OLED_drawPixel(load - 2, i);
			OLED_drawPixel(90+load+2, i);
		}

		OLED_GRAM_Push();

		for(uint8_t i = 0; i < 90; i++)
		{
			load++;

			OLED_drawPixel(load, 48);
			OLED_drawPixel(load, 49);
			OLED_drawPixel(load, 50);

			OLED_GRAM_Push();
		}
	}

	delay(300);
	OLED_GRAM_Clear();
}

