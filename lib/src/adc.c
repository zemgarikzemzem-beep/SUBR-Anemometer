#include "adc.h"
#include "gpio.h"

extern void delay(__IO uint32_t tck);

void ADC_Init(void){
	RCC->AHB2ENR|=(RCC_AHB2ENR_ADC12EN);
	RCC->CCIPR|=(0b10<<RCC_CCIPR_ADC12SEL_Pos);
	
	GPIOA->MODER&=~0x00000003;           // (C0) Analog mode In
	GPIOA->MODER|=((GPIO_MODE_ANALOG<<(0<<1)));
//	GPIOA->ASCR|=GPIO_ASCR_ASC0;
	
//	ADC1->CR=0x00000000;
//	ADC1->CFGR=0x80000000;
	ADC1->SQR1|=ADC_SQR1_SQ1_0;
	ADC1->CR|=ADC_CR_ADVREGEN;
	//ADC1->CFGR|=(ADC_CFGR_CONT|ADC_CFGR_AUTDLY);   //continuous mode (disable DMA must)
	
	
  ADC1->CR |= ADC_CR_ADCAL;
  while (ADC1->CR & ADC_CR_ADCAL);
	
	ADC1->CR|=ADC_CR_ADEN;
	while (!(ADC1->ISR&ADC_ISR_ADRDY)){};
	
	
	//ADC1->IER|=ADC_IER_EOC;
	//NVIC_EnableIRQ(ADC1_IRQn);
}

	uint16_t adc;
uint16_t ADC1_Result_DMA(void){
	ADC1->CR |= ADC_CR_ADSTART;
	while(ADC1->ISR&ADC_ISR_EOC);
	DMA1_Channel1->CCR=0;   // !!!
	DMA1_Channel1->CPAR=(uint32_t)(&ADC1->DR);
	DMA1_Channel1->CMAR=(uint32_t)(&adc);
	DMA1_Channel1->CNDTR=sizeof(adc);
	DMA1_Channel1->CCR|=(DMA_CCR_MINC|DMA_CCR_MSIZE_0|DMA_CCR_PSIZE_0);
	DMA1_Channel1->CCR|=DMA_CCR_EN;
	ADC1->CR |= ADC_CR_ADSTP;
	return adc;
}

//void ADC1_IRQHandler(void){}
	//ADC1->IER|=ADC_IER_ADRDYIE;
