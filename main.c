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
#include "delay.h"


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
uint16_t temper, hum;

extern uint32_t length_mid, phase_shift;
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
			hum=(((data_th[0])<<8)+data_th[1]); // (float) / 10
			temper=(((data_th[2] & 0x3F)<<8)+data_th[3]); // (float) / 10
			
			sprintf(tmp_str, "%2d.%dC   %2d.%d%%", temper/10, temper%10, hum/10, hum%10);
			TFT_Send_Str(10, 200, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		}
			sprintf(tmp_str, "%5d Â", (phase_shift<=1170)?((1170-phase_shift)/10+4):0); // phase_shift
			TFT_Send_Str(50, 80, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		delay_ms(2000);
	}
}

void HardFault_Handler(void){
	uint8_t i=1;
	while(i);
}