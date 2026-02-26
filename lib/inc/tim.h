#ifndef TIM_H_
#define TIM_H_
#include "stm32g4xx.h"                  // Device header

void TIM1_Init(void);
void TIM2_Init(void);
void TIM7_Init(void);
void delay_ms(__IO uint32_t ms_num);
void delay_us(__IO uint32_t us_num);

#endif /* TIM_H_ */
