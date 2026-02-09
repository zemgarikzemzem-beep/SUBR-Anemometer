#ifndef USART_H_
#define USART_H_
//------------------------------------------------
#include "stm32l4xx.h"                 // Device header

void USART2_Init(void);
void USART2_Transmit(uint8_t* data, uint8_t len);
uint8_t USART2_Receive(void);
//------------------------------------------------
#endif /* USART_H_ */
