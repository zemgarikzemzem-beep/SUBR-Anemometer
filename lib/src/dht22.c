#include "dht22.h"
#include "gpio.h"
#include "delay.h"


void DHT22_Init(void){
	GPIOA->MODER&=~GPIO_MODER_MODE10_Msk;
	GPIOA->MODER|=(GPIO_MODE_OUTPUT<<(10<<1));
	GPIOA->OTYPER|=(GPIO_OUTPUT_OPENDRAIN<<10);
	GPIOA->OSPEEDR|=(GPIO_SPEED_FREQ_VERY_HIGH<<(10<<1));
	GPIOA->PUPDR|=(GPIO_PULL_NO<<(10<<1));
	
	delay_ms(2000);
	GPIOA->ODR|=GPIO_ODR_OD10;
}

uint8_t DHT22_GetData(uint8_t *data){
	uint8_t i, j = 0;
	GPIOA->ODR&=~GPIO_ODR_OD10;
	GPIOA->ODR|=GPIO_ODR_OD10;
	delay_ms(100);
	
	GPIOA->ODR &= ~GPIO_ODR_OD10;//низкий уровень
	delay_ms(18);
	GPIOA->ODR |= GPIO_ODR_OD10;//высокий уровень
	
//			while((GPIOA->IDR & GPIO_IDR_ID10));
//			while(!(GPIOA->IDR & GPIO_IDR_ID10));
//			while((GPIOA->IDR & GPIO_IDR_ID10));
	delay_us(39);//20-40 мкс
	if(GPIOA->IDR & GPIO_IDR_ID10) {
		return 0;
	}
	delay_us(80);
	if(!(GPIOA->IDR & GPIO_IDR_ID10)) {
		return 0;
	}
	delay_us(80);
	
	for (j=0; j<5; ++j)
	{
		data[j]=0;
		for(i=0; i<8; ++i){
			while(!(GPIOA->IDR & GPIO_IDR_ID10));
			delay_us(30);
			if(GPIOA->IDR & GPIO_IDR_ID10)
			{
				data[j]|=(1<<(7-i));
				while((GPIOA->IDR & GPIO_IDR_ID10));  // Обязательно подождать!!!
			}
		}
	}
	return 1;
}
