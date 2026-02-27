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
	TIM1->PSC=SystemCoreClock/100000000-1;   // Prescaler = (f(APB1) / f) - 1
	TIM1->ARR=5000-1;   // Period
	TIM1->CR1=0;
	TIM1->SMCR|=((0b00001<<TIM_SMCR_TS_Pos) | (0b0100<<TIM_SMCR_SMS_Pos));
	TIM1->CCMR1|=((0b01<<TIM_CCMR1_CC1S_Pos) | (0b10<<TIM_CCMR1_CC2S_Pos)); //  | (0b0000<<TIM_CCMR1_IC1F_Pos)
//	TIM1->CCER|=TIM_CCER_CC2P;
//	TIM1->DIER|=(TIM_DIER_CC1IE); //  | TIM_DIER_CC2IE
//	TIM1->EGR|=(TIM_EGR_CC1G); //  | TIM_EGR_CC2G
//	TIM1->SR&=~(TIM_SR_CC1IF); //  | TIM_SR_CC2IF
	TIM1->CCER|=(TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC2P); // 
//	TIM1->DIER|=TIM_DIER_UIE;
	TIM1->CR1|=TIM_CR1_CEN;
}

uint32_t phase1, phase_shift;
uint32_t tick=40000;
uint32_t length=0, length_mid=0;
uint32_t min=0xFFFFFFFF, max=0;
uint8_t k=0;
uint32_t a[100]={0,};

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
//		length+=phase_shift; // length=((length>phase_shift)?phase_shift:length);
//		min=((min>phase_shift)?phase_shift:min);
//		max=((max<phase_shift)?phase_shift:max);
//		
//		if(!--tick){
//			tick=40000; // 
//			phase1=TIM1->CNT;
//			length_mid=length/40000; // 
//			a[(k<100)?k++:(k=0)]=length_mid;
//			length=0; // 
//			min=0xFFFFFFFF;
//			max=0;
//		}
//	
//	}
//}
void TIM2_Init(void){
	GPIOA->MODER&=~GPIO_MODER_MODE15_Msk;
	GPIOA->MODER|=(GPIO_MODE_ALTERNATE<<(15<<1));
	GPIOA->AFR[1]|=(0b0001<<GPIO_AFRH_AFSEL15_Pos);
	
	RCC->APB1ENR1|=RCC_APB1ENR1_TIM2EN;
	TIM2->PSC=SystemCoreClock/100000000-1;   // Prescaler = (f(APB1) / f) - 1
	TIM2->ARR=2500-1;   // Period 1000
	TIM2->CR1=0;
	TIM2->CR2|=(0b0010<<TIM_CR2_MMS_Pos);
	TIM2->SMCR|=TIM_SMCR_MSM;
	TIM2->CCR1=1250; // 500
	TIM2->CCMR1|=(0b0110<<TIM_CCMR1_OC1M_Pos | TIM_CCMR1_OC1PE);
//	TIM2->BDTR|=(TIM_BDTR_MOE);
	TIM2->CCER|=(TIM_CCER_CC1E); //  | TIM_CCER_CC1P
	TIM2->CR1|=TIM_CR1_CEN;
}

void TIM7_Init(void){
	RCC->APB1ENR1|=RCC_APB1ENR1_TIM7EN;
	NVIC_EnableIRQ(TIM7_IRQn);
	TIM7->PSC=SystemCoreClock/100000000-1;   // Prescaler = (f(APB1) / f) - 1
	TIM7->ARR=2500;   // Period 1000
	TIM7->CR1=0;
	TIM7->EGR|=TIM_EGR_UG;
	TIM7->SR&=~TIM_SR_UIF;
	TIM7->DIER=TIM_DIER_UIE;
	TIM7->CR1|=TIM_CR1_CEN;
	RCC->APB1ENR1|=RCC_APB1ENR1_TIM7EN;
}

uint32_t ms=0, us=0;
uint8_t ms_flag=0, us_flag=0;

void TIM7_IRQHandler(void){
	TIM7->SR&=~TIM_SR_UIF;
////		GPIOC->BSRR|=(1<<6);
	length+=TIM1->CCR1; // length=((length>phase_shift)?phase_shift:length);
	phase1=TIM1->CCR2;
	min=((min>phase1)?phase1:min);
	max=((max<phase1)?phase1:max);
		
	if(!--tick){
		tick=40000; //
		phase_shift=length/40000; // 
		a[(k<100)?k++:(k=0)]=phase_shift;
		length=0; // 
		min=0xFFFFFFFF;
		max=0;
	}
}
