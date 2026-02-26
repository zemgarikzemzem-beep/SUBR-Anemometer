#include "tft.h"
#include "spi.h"
#include "gpio.h"
#include "img.h"
#include "delay.h"

const uint8_t SHIFT_X = 0;
const uint8_t SHIFT_Y = 20;
const uint8_t DISP_WIDTH = 240;
const uint16_t DISP_HEIGHT = 280;

//__STATIC_INLINE void delay_us(__IO uint32_t micros)
//{
//	micros *=(SystemCoreClock / 1000000) / 9;
//	while (micros--);
//}

void TFT_Send_Command(uint8_t com){
	TFT_CSEN;
	TFT_COMMAND;
	SPI1_Send_Byte(com);
	TFT_CSDIS;
}

void TFT_Send_Byte(uint8_t data){
	TFT_CSEN;
	TFT_DATA;
	SPI1_Send_Byte(data);
	TFT_CSDIS;
}

void TFT_Send_Data(uint8_t* data, uint8_t size){
	TFT_CSEN;
	TFT_DATA;
	for(uint8_t i=0; i<size; ++i) SPI1_Send_Byte(data[i]);
	TFT_CSDIS;
}

void TFT_Init(void){
	GPIOB->MODER&=~0x000CC000; // 0x000F0000
	GPIOB->MODER|=((GPIO_MODE_OUTPUT<<(7<<1))|(GPIO_MODE_OUTPUT<<(9<<1)));
//	GPIOB->OTYPER|=((GPIO_OUTPUT_PUSHPULL<<8)|(GPIO_OUTPUT_PUSHPULL<<9));
//	GPIOB->OSPEEDR|=((GPIO_SPEED_FREQ_LOW <<(8<<1))|(GPIO_SPEED_FREQ_LOW <<(9<<1)));
	
	GPIOA->MODER&=~0x00000300;
	GPIOA->MODER|=(GPIO_MODE_OUTPUT<<(4<<1));
//	GPIOA->OTYPER|=((GPIO_OUTPUT_PUSHPULL<<4));
//	GPIOA->OSPEEDR|=((GPIO_SPEED_FREQ_LOW <<(4<<1)));
	
	TFT_CSEN;
	TFT_DATA;
	
	TFT_START;
	delay_us(100);
	
	TFT_RES;
	delay_us(10);
	TFT_START;
	delay_us(100);
	
	TFT_Send_Command(ST7789_SWRESET); // Software reset
	delay_us(150);
	
	TFT_Send_Command(ST7789_SLPOUT); // Out of sleep mode
	delay_us(10);
	
	TFT_Send_Command(ST7789_COLMOD); // Set color mode
	TFT_Send_Byte(0x55); // 16-bit color
	delay_us(10);
	
	TFT_Send_Command(ST7789_MADCTL); // Mem access ctrl (directions)
	TFT_Send_Byte(0x00); // Row/col addr, bottom-top refresh    3-й бит на "0", чтобы выводило пиксели с младшего бита
	/*
	TFT_Send_Command(ST7789_CASET); // Column addr set
	{uint8_t data[]={0,0,0,239};
	TFT_Send_Data(data, sizeof(data));}
	
	TFT_Send_Command(ST7789_RASET); // Row addr set
	{uint8_t data[]={0,0,279>>8,279&0xFF};
	TFT_Send_Data(data, sizeof(data));}
	*/
	TFT_Send_Command(ST7789_INVON); //  hack
	delay_us(10);
	
	TFT_Send_Command(ST7789_NORON); // Normal display on
	delay_us(10);
	
	TFT_Send_Command(ST7789_DISPON); // Main screen turn on
	delay_us(10);
	
}

void TFT_Set_Area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	
	TFT_CSEN;
	TFT_COMMAND;
	SPI1_Send_Byte(ST7789_CASET); // Column addr set
	TFT_DATA;
	SPI1_Send_Byte((SHIFT_X+x1)>>8);
	SPI1_Send_Byte((SHIFT_X+x1)&0xFF);
	SPI1_Send_Byte((SHIFT_X+x2)>>8);
	SPI1_Send_Byte((SHIFT_X+x2)&0xFF);
	TFT_COMMAND;
	SPI1_Send_Byte(ST7789_RASET); // Row addr set
	TFT_DATA;
	SPI1_Send_Byte((SHIFT_Y+y1)>>8);
	SPI1_Send_Byte((SHIFT_Y+y1)&0xFF);
	SPI1_Send_Byte((SHIFT_Y+y2)>>8);
	SPI1_Send_Byte((SHIFT_Y+y2)&0xFF);
	TFT_COMMAND;
	TFT_Send_Command(ST7789_RAMWR);
	TFT_CSDIS;
}



void TFT_Fill_Color(uint16_t color){
	TFT_Set_Area(0,0,DISP_WIDTH,DISP_HEIGHT);
	TFT_CSEN;
	TFT_DATA;
	for (uint32_t i = 0; i < DISP_WIDTH*DISP_HEIGHT; ++i){
		SPI1_Send_Byte(color>>8);
		SPI1_Send_Byte(color&0xFF);
				}
	TFT_CSDIS;				
}

void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color){
	TFT_Set_Area(x,y,x,y);
	uint8_t data[] = {color >> 8, color & 0xFF};
	TFT_Send_Data(data, sizeof(data));
}

void TFT_Send_Char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t fontcolor,  uint16_t backcolor){
	uint32_t b;
	
	uint8_t cf1=fontcolor >> 8;
	uint8_t cf2=fontcolor & 0xFF;
	uint8_t cb1=backcolor >> 8;
	uint8_t cb2=backcolor & 0xFF;
	
	
	TFT_Set_Area(x, y, x+font.width-1, y+font.height-1);
	
	TFT_CSEN;
	TFT_DATA;
	for(uint32_t i=0; i<font.height; ++i){
		if (ch>=192) b=font.data[font.height*(ch-96)+i];
		else if(ch==168) b=font.data[font.height*(160)+i];
		else if(ch==184) b=font.data[font.height*(161)+i];
		else b=font.data[font.height*(ch-32)+i];
		for(uint32_t j=0; j<font.width; ++j){
			if((b<<j)&0x8000){
				SPI1_Send_Byte(cf1);
				SPI1_Send_Byte(cf2);
			}
			else{
				SPI1_Send_Byte(cb1);
				SPI1_Send_Byte(cb2);
			}
		}
	}
	TFT_CSDIS;
}

void TFT_Send_Str(uint16_t x, uint16_t y, char* str, uint8_t size, FontDef font, uint16_t fontcolor,  uint16_t backcolor){
	uint16_t xn=x,yn=y;
	for(uint8_t i=0; i<size; ++i){
		TFT_Send_Char(xn,yn,str[i],font, fontcolor, backcolor);
		xn+=font.width;
		if(xn>DISP_WIDTH-font.width) {xn=x; yn+=font.height;}
	}

}
	
void TFT_Rudik(void){
	TFT_Set_Area(30,20,30+width-1,20+height-1);
	
	SPI1->CR1&=~SPI_CR1_SPE;
	SPI1->CR2=0x0000;
	SPI1->CR2|=((0b1111<<SPI_CR2_DS_Pos));
	SPI1->CR2|=SPI_CR2_TXDMAEN;
	SPI1->CR1|=SPI_CR1_SPE;
	
	TFT_CSEN;
	TFT_DATA;
	for (uint32_t i = 0; i < width*height; ++i){
		DMA1_Channel3->CCR=0;
		DMA1_Channel3->CPAR=(uint32_t)(&(SPI1->DR));
		DMA1_Channel3->CMAR=(uint32_t)(img+i);
		DMA1_Channel3->CNDTR=1;//width*height
		DMA1_Channel3->CCR|=(DMA_CCR_MINC|DMA_CCR_DIR|DMA_CCR_PSIZE_0|DMA_CCR_MSIZE_0);//
		DMA1_Channel3->CCR|=DMA_CCR_EN;
		//while(!(SPI1->SR&SPI_SR_TXE));
}
/*	
	for (uint32_t i = 0; i < width*height; ++i){
					while(!(SPI1->SR&SPI_SR_TXE));
					(SPI1->DR)=img[i];
				}
*/				
	TFT_CSDIS;
				
	SPI1->CR1&=~SPI_CR1_SPE;
	SPI1->CR2=0x0000;
	SPI1->CR2|=((0b0111<<SPI_CR2_DS_Pos));
	SPI1->CR2|=SPI_CR2_TXDMAEN;
	SPI1->CR1|=SPI_CR1_SPE;
}
