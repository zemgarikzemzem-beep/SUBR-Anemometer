#include "tim.h"
#include "gpio.h"
#include "tft.h"
#include "dht22.h"

/*
Скорость звука в сухом воздухе определяется по формуле с0 = 20,1 sqrt(Т) м/с, 
а во влажном воздухе с0 = 20,1 sqrt(ТВ) м/с, где ТВ = так называемая акустическая виртуальная температура, 
которая определяется по формуле ТВ = Т (1+ 0,275 е/р) . 
При изменении температуры воздуха на 1° скорость звука изменяется на 0,61 м/с. 
Скорость звука зависит от величины отношения е/р (отношение влажности к давлению) , 
но эта зависимость мала, и, например, при упругости водяного пара менее 7мм пренебрежение ею дает ошибку в скорости звука, 
не превышающую 0,5 м/сек. 
При нормальном давлении и Т = 273° (0 °С) скорость звука в сухом воздухе равна 331 м/сек. 
Во влажном воздухе скорость звука может быть определена по формуле с = 331 + 0,6t + 0,07е. 
*/


void TIM1_Init(void){
	GPIOA->MODER&=~GPIO_MODER_MODE8_Msk;
	GPIOA->MODER|=(GPIO_MODE_ALTERNATE<<(8<<1));
	GPIOA->AFR[1]|=(0b0110<<GPIO_AFRH_AFSEL8_Pos);
	
	RCC->APB2ENR|=RCC_APB2ENR_TIM1EN;
//	NVIC_EnableIRQ(TIM1_CC_IRQn);
	TIM1->PSC=SystemCoreClock/80000000-1;   // Prescaler = (f(APB1) / f) - 1
	TIM1->ARR=2000-1;   // Period
	TIM1->CCR1=1000-1;
	TIM1->CR1=0;
	TIM1->CR2|=(0b10<<TIM_CR2_MMS_Pos); //  ?
	TIM1->SMCR|=((0b00010<<TIM_SMCR_TS_Pos) | (0b0101<<TIM_SMCR_SMS_Pos) | TIM_SMCR_MSM);
	TIM1->CCMR1|=(TIM_CCMR1_OC1PE | (0b0110<<TIM_CCMR1_OC1M_Pos));
	TIM1->BDTR|=(TIM_BDTR_MOE);
	TIM1->CCER|=(TIM_CCER_CC1E); //  | TIM_CCER_CC1P
//	TIM1->DIER|=TIM_DIER_UIE;
	TIM1->CR1|=TIM_CR1_CEN;
}

uint32_t phase1, phase_shift;

//void TIM1_CC_IRQHandler(void){
//		TIM1->SR&=~TIM_SR_UIF;
//		TIM1->SR&=~TIM_SR_CC2IF;
//		TIM1->SR&=~TIM_SR_CC3IF;
//		TIM1->SR&=~TIM_SR_CC4IF;
//	if(TIM1->SR&TIM_SR_CC1IF){
//		TIM1->SR&=~TIM_SR_CC1IF;
//		phase_shift=TIM2->CNT;
//		TIM1->CNT=0;
//////		GPIOC->BSRR|=(1<<6);
//	
//	}
//}
void TIM2_Init(void){
//	GPIOA->MODER&=~GPIO_MODER_MODE15_Msk;
//	GPIOA->MODER|=(GPIO_MODE_ALTERNATE<<(15<<1));
//	GPIOA->AFR[1]|=(0b0001<<GPIO_AFRH_AFSEL15_Pos);
	
	RCC->APB1ENR1|=RCC_APB1ENR1_TIM2EN;
	TIM2->PSC=SystemCoreClock/80000000-1;   // Prescaler = (f(APB1) / f) - 1
	TIM2->ARR=0xFFFFFFFF;   // Period 1000
	TIM2->CR1=0;
	TIM2->SMCR|=((0b0100<<TIM_SMCR_SMS_Pos) | (0b010<<TIM_SMCR_TS_Pos));
//	TIM2->CCMR1|=(0b0110<<TIM_CCMR1_OC1M_Pos | TIM_CCMR1_OC1PE);
	TIM2->CR1|=TIM_CR1_CEN;
}

void TIM3_Init(void){
	GPIOA->MODER&=~GPIO_MODER_MODE6_Msk;
	GPIOA->MODER|=(GPIO_MODE_ALTERNATE<<(6<<1));
	GPIOA->AFR[0]|=(0b0010<<GPIO_AFRL_AFSEL6_Pos);
	
	RCC->APB1ENR1|=RCC_APB1ENR1_TIM3EN;
	NVIC_EnableIRQ(TIM3_IRQn);
	TIM3->PSC=SystemCoreClock/10000-1;   // Prescaler = (f(APB1) / f) - 1
	TIM3->ARR=1000-1;   // Period 1000
	TIM3->CCR1=4;
	TIM3->CR1|=TIM_CR1_DIR;
	TIM3->CR2|=((0b0100<<TIM_CR2_MMS_Pos));
	TIM3->SMCR|=(TIM_SMCR_MSM);
	TIM3->CCMR1|=(TIM_CCMR1_OC1PE | (0b0110<<TIM_CCMR1_OC1M_Pos)); //
	
//	TIM3->CCER|=TIM_CCER_CC1P;
	
//	TIM3->BDTR|=(TIM_BDTR_MOE);
	
	TIM3->EGR|=TIM_EGR_CC1G;
	TIM3->SR&=~TIM_SR_CC1IF;
	
	TIM3->DIER=TIM_DIER_CC1IE;
	TIM3->CCER|=TIM_CCER_CC1E;
	TIM3->CR1|=TIM_CR1_CEN;
}


uint8_t new_pulse_flag=0;

void TIM3_IRQHandler(void){
//	TIM3->SR&=~TIM_SR_UIF;
//	TIM3->SR&=~TIM_SR_CC2IF;
//	TIM3->SR&=~TIM_SR_CC3IF;
//	TIM3->SR&=~TIM_SR_CC4IF;
	if(TIM3->SR&TIM_SR_CC1IF){
		TIM3->SR&=~TIM_SR_CC1IF;
		TIM1->CNT=0;
		TIM2->CNT=0;
		new_pulse_flag=1;
//		NVIC_EnableIRQ(EXTI0_IRQn);
	}
//	GPIOC->ODR^=(1<<6);
}
