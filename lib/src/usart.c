#include "usart.h"
#include "gpio.h"

void USART2_Init(void){
	RCC->APB1ENR1|=RCC_APB1ENR1_USART2EN; // a9,10 af7
	
	GPIOA->MODER&=~((0x3<<(2<<1))|(0x3<<(3<<1)));// (A9,A10) Alternate push-pull high speed
	GPIOA->MODER|=(GPIO_MODE_ALTERNATE<<(2<<1))|(GPIO_MODE_ALTERNATE<<(3<<1));
	GPIOA->OTYPER|=((GPIO_OUTPUT_PUSHPULL<<2)|(GPIO_OUTPUT_PUSHPULL<<3));
	GPIOA->OSPEEDR|=((GPIO_SPEED_FREQ_VERY_HIGH<<(2<<1))|(GPIO_SPEED_FREQ_VERY_HIGH<<(3<<1)));
	GPIOA->PUPDR|=((GPIO_PULL_NO<<(2<<1))|(GPIO_PULL_NO<<(3<<1)));
	GPIOA->AFR[0]=0x00007700;
	
	if(USART2->CR1&USART_CR1_UE) USART2->CR1&=~USART_CR1_UE;
	
	USART2->CR1|=(USART_CR1_TE|USART_CR1_RE);
	USART2->BRR=(0x1A<<USART_BRR_DIV_MANTISSA_Pos)|(0x1<<USART_BRR_DIV_FRACTION_Pos);
	USART2->CR1|=USART_CR1_UE;
}

void USART2_Transmit(uint8_t* data, uint8_t len){
	uint8_t i = 0;
	
		while(i<len-1){
		USART2->TDR=data[i];
		while(!(USART2->ISR&USART_ISR_TXE));
		i++;
		}
		i=0;
	}
uint8_t data1;
uint8_t USART2_Receive(void){
	data1=USART2->RDR;
	while(USART2->ISR&USART_ISR_RXNE);
	return data1;
}
