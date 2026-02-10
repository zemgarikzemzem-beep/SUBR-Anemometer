#ifndef TIM_H_
#define TIM_H_
#include "stm32g4xx.h"                  // Device header

void TIM1_Init(void);
void TIM2_Init(void);
void TIM7_Init(void);
void delay_ms(uint16_t ms_num);
void delay_us(uint16_t us_num);

#endif /* TIM_H_ */
