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
uint8_t temper, hum;

void delay(__IO uint32_t tck){
	tck*=SystemCoreClock/10000;
	while(--tck);
}

extern uint32_t length_mid;
char tmp_str[20]={0,};

int main(void){
	Clock_Init();
	GPIO_Init();
	TIM7_Init();
	DHT22_Init();
	SPI1_Init();
	TFT_Init();
	TFT_Fill_Color(YELLOW);
	TIM2_Init();
	TIM1_Init();
	
	while(1){
		//SPI1_Send_Byte(0xAA);
//		GPIOC->ODR^=(1<<6);
		if(((data_th[0]+data_th[1]+data_th[2]+data_th[3])&0xFF)==data_th[4] && DHT22_GetData(data_th)){ // 
			hum=(((data_th[0])<<8)+data_th[1]) / 10; // (float)
			temper=(((data_th[2] & 0x3F)<<8)+data_th[3]) / 10; // (float)
		}
			sprintf(tmp_str, "%5d Â", length_mid); // (length_mid<=1572)?((1572-length_mid)/10+4):0
			TFT_Send_Str(50, 80, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
			
			sprintf(tmp_str, "%2dC   %2d%%", temper, hum);
			TFT_Send_Str(50, 200, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		delay_ms(2000);
	}
}

void HardFault_Handler(void){
	uint8_t i=1;
	while(i);
}