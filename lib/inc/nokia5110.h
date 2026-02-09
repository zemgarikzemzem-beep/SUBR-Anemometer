#ifndef NOKIA5110_H_
#define NOKIA5110_H_
#include "stm32l4xx.h"                  // Device header

#define N_RES GPIOB->BSRR|=(1<<(8+16))
#define N_START GPIOB->BSRR|=(1<<8)
#define N_DATA GPIOB->BSRR|=(1<<9)
#define N_COMMAND GPIOB->BSRR|=(1<<(9+16))

void NOKIA_Init(void);
void NOKIA_Send_Char(uint8_t ch);
void NOKIA_Send_Str(char* str);
void NOKIA_Clean(void);
void NOKIA_Monkey(void);
void NOKIA_Send_Pos(uint8_t x, uint8_t y);

#endif /* NOKIA5110_H_ */
