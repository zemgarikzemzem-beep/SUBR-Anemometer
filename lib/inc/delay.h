#ifndef DELAY_H_
#define DELAY_H_
#include "stm32g4xx.h"                  // Device header

void delay_ms(__IO uint32_t ms_num);
void delay_us(__IO uint32_t us_num);

#endif /* DELAY_H_ */
