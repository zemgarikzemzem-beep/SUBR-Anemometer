#include "keyboard.h"
#include "gpio.h"
char key[2][2]={{'a','b'},{'c','d'}};
uint8_t colpins[COLS]={7,8};
uint8_t rowpins[ROWS]={5,6};
void KEYBOARD_Init(void){
	GPIOC->MODER&=~(0xFF<<(5<<1));
	GPIOC->MODER|=((GPIO_MODE_OUTPUT<<(5<<1))|(GPIO_MODE_OUTPUT<<(6<<1))|(GPIO_MODE_INPUT<<(7<<1))|(GPIO_MODE_INPUT<<(8<<1)));
	GPIOC->PUPDR&=~(0xFF<<(5<<1));
	GPIOC->PUPDR|=((GPIO_PULL_DOWN<<(7<<1))|(GPIO_PULL_DOWN<<(8<<1)));
	for(uint8_t k=0; k<ROWS; ++k) GPIOC->BSRR|=(1<<(5+k+16));
	for(uint8_t k=0; k<ROWS; ++k) GPIOC->BSRR|=(1<<(5+k));
}

bool IFPRESSED(void){
	return (GPIOC->IDR&(0X3<<7));
}

uint8_t keyi, keyj;
char KEYPRESSED(void){
	for(uint8_t i=0; i<ROWS; ++i){
		for(uint8_t k=0; k<ROWS; ++k) GPIOC->BSRR|=(1<<(5+k+16));
		GPIOC->BSRR|=(1<<(5+i));
		for(uint8_t j=0; j<COLS; ++j) if(GPIOC->IDR&(0X3<<(7+j))) {keyi=i; keyj=j;}
	}
	
	for(uint8_t k=0; k<ROWS; ++k) GPIOC->BSRR|=(1<<(5+k+16));
	for(uint8_t k=0; k<ROWS; ++k) GPIOC->BSRR|=(1<<(5+k));
	return key[keyi][keyj];
}
