#ifndef TFT_H_
#define TFT_H_
#include "stm32g4xx.h"                  // Device header
#include "fonts.h"

#define TFT_RES GPIOB->BRR|=(1<<7)
#define TFT_START GPIOB->BSRR|=(1<<7)
#define TFT_DATA GPIOB->BSRR|=(1<<9)
#define TFT_COMMAND GPIOB->BRR|=(1<<9)
#define TFT_CSEN GPIOA->BRR|=(1<<4)
#define TFT_CSDIS GPIOA->BSRR|=(1<<4)

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09
#define ST7789_RDDPM   0x0A
#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13
#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E
#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define GRAY        0X8430
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define BROWN       0XBC40
#define BRRED       0XFC07
#define DARKBLUE    0X01CF
#define LIGHTBLUE   0X7D7C
#define GRAYBLUE    0X5458

#define LIGHTGREEN  0X841F
#define LGRAY       0XC618
#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12

void TFT_Init(void);
void TFT_Fill_Color(uint16_t color);
void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_Send_Char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t fontcolor,  uint16_t backcolor);
void TFT_Send_Str(uint16_t x, uint16_t y, char* str, uint8_t size, FontDef font, uint16_t fontcolor,  uint16_t backcolor);
void TFT_Rudik(void);
void TFT_Set_Area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

#endif /* TFT_H_ */
