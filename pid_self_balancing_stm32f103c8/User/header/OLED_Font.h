#ifndef __OLED_FONT_H__
#define __OLED_FONT_H__

#include <stdint.h>

#define FONT_8X8            1
#define FONT_16X16          2

extern const unsigned char Font_8x8_void[8];
extern const uint16_t Font_16x16_void[16];

extern const unsigned char Font_8x8_filled[8];
extern const uint16_t Font_16x16_filled[16];

extern const unsigned char Font_Symbol1_8x8[15][8];
extern const uint16_t Font_Symbol1_16x16[15][16];


extern const unsigned char Font_Number_8x8[10][8];
extern const uint16_t Font_Number_16x16[10][16];

extern const unsigned char Font_Symbol2_8x8[7][8];
extern const uint16_t Font_Symbol2_16x16[7][16];

extern const unsigned char Font_Letter_8x8_A_to_Z[26][8];
extern const uint16_t Font_Letter_16x16_A_to_Z[26][16];

extern const unsigned char Font_Symbol3_8x8[6][8];
extern const uint16_t Font_Symbol3_16x16[6][16];

extern const unsigned char Font_Letter_8x8_a_to_z[26][8];
extern const uint16_t Font_Letter_16x16_a_to_z[26][16];

extern const unsigned char Font_Symbol4_8x8[4][8];
extern const uint16_t Font_Symbol4_16x16[4][16];



#endif
