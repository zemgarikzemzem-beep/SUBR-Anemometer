#include "tim.h"
#include "gpio.h"
#include "tft.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>


void TIM2_Init(void){
	GPIOA->MODER&=~GPIO_MODER_MODE15_Msk;
	GPIOA->MODER|=(GPIO_MODE_ALTERNATE<<(15<<1));
	GPIOA->AFR[1]|=(0b0001<<GPIO_AFRH_AFSEL15_Pos);
	
	RCC->APB1ENR1|=RCC_APB1ENR1_TIM2EN;
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM2->PSC=SystemCoreClock/100000000-1;   // Prescaler = (f(APB1) / f) - 1
	TIM2->ARR=100000000-1;   // Period
	TIM2->CR1=0;
	TIM2->CCMR1|=((0b01<<TIM_CCMR1_CC1S_Pos) | (0b10<<TIM_CCMR1_CC2S_Pos));
	TIM2->CCER|=TIM_CCER_CC2P;
	TIM2->DIER|=(TIM_DIER_CC1IE | TIM_DIER_CC2IE);
	TIM2->EGR|=(TIM_EGR_CC1G | TIM_EGR_CC2G);
	TIM2->SR&=~(TIM_SR_CC1IF | TIM_SR_CC2IF);
	TIM2->CCER|=(TIM_CCER_CC1E | TIM_CCER_CC2E);
//	TIM2->DIER|=TIM_DIER_UIE;
	TIM2->CR1|=TIM_CR1_CEN;
}

uint32_t period, pulse_width;
uint16_t tick=40000;
int length=0;
uint8_t k=0;
uint32_t a[100]={0,};


void TIM2_IRQHandler(void){
	char tmp_str[20]={0,};
	
	if(TIM2->SR&TIM_SR_CC1IF){
		
		TIM2->SR&=~TIM_SR_CC1IF;
		TIM2->SR&=~TIM_SR_CC2IF;
		TIM2->CNT=0;
		period=TIM2->CCR1; // 
		pulse_width=TIM1->CNT; // TIM2->CCR2
//		length=((int)((float)pulse_width*50000/285)); // -171228-1403 + 175
//		if(length>=4386) length-=4386;
		length+=(pulse_width); // -9770
//		if(length<0) length=0;
//		a[k++]=(pulse_width); // /100 -9770
		
		if(!--tick){
			tick=40000; // 
			length/=40000;
			a[(k<100)?k++:(k=0)]=length;
//			k=0;
//		sprintf(tmp_str, "Период:%6d", period);
//		TFT_Send_Str(10, 50, tmp_str, strlen(tmp_str), Font_11x18, RED, YELLOW);
//		sprintf(tmp_str, "Ширина:%6d", pulse_width);
//		TFT_Send_Str(10, 80, tmp_str, strlen(tmp_str), Font_11x18, RED, YELLOW);
			
			sprintf(tmp_str, "%5d В", (length<=1588)?((1588-length)/10+4):0);
			TFT_Send_Str(50, 80, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
			length=0;
		}
	}
	
//	if(TIM2->SR&TIM_SR_CC2IF){
//		TIM2->SR&=~TIM_SR_CC2IF;
//		TIM2->CNT=0;
//		period=TIM2->CCR1;
//		pulse_width=TIM2->CCR2;
//	}
	
//	GPIOC->ODR^=(1<<6);
}

void TIM1_Init(void){
	GPIOA->MODER&=~GPIO_MODER_MODE8_Msk;
	GPIOA->MODER|=(GPIO_MODE_ALTERNATE<<(8<<1));
	GPIOA->AFR[1]|=(0b0110<<GPIO_AFRH_AFSEL8_Pos);
	
	RCC->APB2ENR|=RCC_APB2ENR_TIM1EN;
	TIM1->PSC=SystemCoreClock/100000000-1;   // Prescaler = (f(APB1) / f) - 1
	TIM1->ARR=2500-1;   // Period 1000
	TIM1->CR1=0;
	TIM1->CCR1=1250; // 500
	TIM1->CCMR1|=(0b0110<<TIM_CCMR1_OC1M_Pos | TIM_CCMR1_OC1PE);
	TIM1->BDTR|=(TIM_BDTR_MOE); // TIM_BDTR_BKP | TIM_BDTR_BK2P | 
//	TIM1->AF1|=(TIM1_AF1_BKINE);
//	TIM1->AF2|=(TIM1_AF2_BK2INE);
	TIM1->CCER|=TIM_CCER_CC1E;
	TIM1->CR1|=TIM_CR1_CEN;
}
