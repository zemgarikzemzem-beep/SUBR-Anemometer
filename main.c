#include "stm32g4xx.h"

#include "rcc.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"
#include "tft.h"
#include "fonts.h"
#include "adc.h"


//void delay(__IO uint32_t tck)
//{
//	__asm__ __volatile__(
//			"mov	r0,#tck\n"
//		"label:\n"
//			"subs	r0, r0, #1\n"
//			"bne	label"
//	);
//}

void delay(__IO uint32_t tck){
	while(--tck);
}

int main(void){
	Clock_Init();
	GPIO_Init();
	SPI1_Init();
	TFT_Init();
	TFT_Fill_Color(YELLOW);
	TIM2_Init();
	TIM1_Init();
//	ADC_Init();
	
	while(1){
		//SPI1_Send_Byte(0xAA);
	}
}

void HardFault_Handler(void){
	uint8_t i=1;
	while(i);
}