#include "stm32g4xx.h"

#include "rcc.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"
#include "tft.h"
#include "fonts.h"
#include "adc.h"
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

uint8_t data[5]={0,};
float temper;

void delay(__IO uint32_t tck){
	tck*=SystemCoreClock/10000;
	while(--tck);
}

int main(void){
	Clock_Init();
	GPIO_Init();
	SPI1_Init();
	TIM7_Init();
	DHT22_Init();
	TFT_Init();
	TFT_Fill_Color(YELLOW);
	TIM2_Init();
	TIM1_Init();
//	ADC_Init();
	
	while(1){
		//SPI1_Send_Byte(0xAA);
//		GPIOC->ODR^=(1<<6);
		if(data[0]+data[1]+data[2]+data[3]==data[4]){ // DHT22_GetData(data)
			temper=(float)((*(uint16_t*)(data+1)) & 0x3FFF) / 10;
		}
		delay_ms(1500);
	}
}

void HardFault_Handler(void){
	uint8_t i=1;
	while(i);
}