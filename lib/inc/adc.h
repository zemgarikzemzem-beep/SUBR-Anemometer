#ifndef ADC_H_
#define ADC_H_
#include "stm32g4xx.h"                  // Device header

void ADC_Init(void);
uint16_t ADC1_Result_DMA(void);

#endif /* ADC_H_ */
