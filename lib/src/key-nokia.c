#include "key-nokia.h"
#include "gpio.h"
#include "nokia5110.h"

char key[2][2]={{'a','b'},{'c','d'}};
uint8_t colpins[COLS]={7,8};
uint8_t rowpins[ROWS]={5,6};
static uint16_t count=0;
void KEYBOARD_Init(void){
	GPIOC->MODER&=~(0xF<<(5<<1));
	GPIOC->MODER|=((GPIO_MODE_OUTPUT<<(5<<1))|(GPIO_MODE_OUTPUT<<(6<<1)));
	GPIOC->PUPDR&=~(0xF<<(5<<1));
	GPIOA->MODER&=~(0xF<<(0<<1));
	GPIOA->MODER|=((GPIO_MODE_INPUT<<(0<<1))|(GPIO_MODE_INPUT<<(1<<1)));
	GPIOA->PUPDR|=((GPIO_PULL_DOWN<<(0<<1))|(GPIO_PULL_DOWN<<(1<<1)));
	for(uint8_t k=0; k<ROWS; ++k) GPIOC->BRR|=(1<<(5+k));
	for(uint8_t k=0; k<ROWS; ++k) GPIOC->BSRR|=(1<<(5+k));
	
}

/*	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0]|=(SYSCFG_EXTICR1_EXTI0_PA|SYSCFG_EXTICR1_EXTI1_PA);
	EXTI->RTSR1|=(EXTI_RTSR1_RT0|EXTI_RTSR1_RT1);
	EXTI->IMR1|=(EXTI_IMR1_IM0|EXTI_IMR1_IM1);
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
uint8_t keyi, keyj;
void EXTI9_5_IRQHandler(void){
		for(uint8_t i=0; i<ROWS; ++i){
		for(uint8_t k=0; k<ROWS; ++k) GPIOC->BSRR|=(1<<(5+k+16));
		GPIOC->BSRR|=(1<<(5+i));
		for(uint8_t j=0; j<COLS; ++j) if(GPIOC->IDR&(0X3<<(7+j))) {keyi=i; keyj=j;}
	}
	
	for(uint8_t k=0; k<ROWS; ++k) GPIOC->BSRR|=(1<<(5+k+16));
	for(uint8_t k=0; k<ROWS; ++k) GPIOC->BSRR|=(1<<(5+k));
}

void EXTI1_IRQHandler(void){
	EXTI->PR1|=EXTI_PR1_PIF1;
}
*/
bool IFPRESSED(void){
	//count+=5;
	return (GPIOA->IDR&(0X3<<0));
}

uint8_t keyi, keyj;
char KEYPRESSED(void){
	for(uint8_t i=0; i<ROWS; ++i){
		for(uint8_t k=0; k<ROWS; ++k) GPIOC->BRR|=(1<<(5+k));
		GPIOC->BSRR|=(1<<(5+i));
		for(uint8_t j=0; j<COLS; ++j) if(GPIOA->IDR&(0X3<<(0+j))) {keyi=i; keyj=j;}
	}
	
	for(uint8_t k=0; k<ROWS; ++k) GPIOC->BRR|=(1<<(5+k));
	for(uint8_t k=0; k<ROWS; ++k) GPIOC->BSRR|=(1<<(5+k));
	return key[keyi][keyj];
}

void Del_Char(void){
	NOKIA_SetPos(0+count-5,0);
	NOKIA_Send_Char (' ');
	NOKIA_SetPos(0+count-5,0);
}
