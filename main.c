#include "stm32g4xx.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>

#include "rcc.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"
#include "tft.h"
#include "fonts.h"
#include "dht22.h"
#include "exti.h"
#include "delay.h"
#include "TDC1000.h"


//void delay(__IO uint32_t tck)
//{
//	__asm__ __volatile__(
//			"mov	r0,#tck\n"
//		"label:\n"
//			"subs	r0, r0, #1\n"
//			"bne	label"
//	);
//}

uint8_t data_th[5]={0,};
int temper, hum;

extern int32_t length_mid, phase_shift;
char tmp_str[20]={0,};

int main(void){
	Clock_Init();
	GPIO_Init();
	DHT22_Init();
	
	SPI1_Init();
	TFT_Init();
	
	SPI2_Init();
	MCO_Init();  // Ћибо TIM1, либо - это !!!
	TDC1000_Init();
	TDC7200_Init();
	uint8_t ansv=0;
	
	TFT_Fill_Color(YELLOW);
	
//	TIM3_Init();
//	TIM1_Init();
//	TIM2_Init();
//	EXTI_Init();
	
	while(1){
		
		TDC7200_SPIWrite(0x00, 0x03);
	
		ansv=TDC1000_SPIRead(TDC1000_REG_ADR_ERROR_FLAGS);
		sprintf(tmp_str, "0x%X", ansv);
		TFT_Send_Str(20, 90, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
	
		ansv=TDC7200_SPIRead(0x02);
		sprintf(tmp_str, "0x%X", ansv);
		TFT_Send_Str(20, 120, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
	
		ansv=TDC7200_SPIRead(0x10);
		sprintf(tmp_str, "0x%X", ansv);
		TFT_Send_Str(20, 150, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
	
		ansv=TDC7200_SPIRead(0x11);
		sprintf(tmp_str, "0x%X", ansv);
		TFT_Send_Str(20, 180, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
	
//		ansv=TDC7200_SPIRead(0x13);
//		sprintf(tmp_str, "0x%X", ansv);
//		TFT_Send_Str(20, 180, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		//SPI1_Send_Byte(0xAA);
//		GPIOC->ODR^=(1<<6);
		
//		if(((data_th[0]+data_th[1]+data_th[2]+data_th[3])&0xFF)==data_th[4] && DHT22_GetData(data_th)){ // 
//			hum=(((data_th[0])<<8)+data_th[1]); // (float) / 10
//			temper=(((data_th[2] & 0x3F)<<8)+data_th[3]); // (float) / 10
//			
//		}
//		sprintf(tmp_str, "%2d.%dC   %2d.%d%%", temper/10, temper%10, hum/10, hum%10);
//		TFT_Send_Str(10, 200, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
//		
//		sprintf(tmp_str, "%5d м/с", (length_mid<=1820-(temper-220)*5)?((1820-(temper-220)*5-length_mid)/10+4):0); // phase_shift 40010 243
//		TFT_Send_Str(20, 80, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		delay_ms(200);

		
	}
}

void HardFault_Handler(void){
	uint8_t i=1;
	while(i);
}