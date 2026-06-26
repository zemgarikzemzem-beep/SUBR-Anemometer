#include "exti.h"
#include "gpio.h"
#include <stdlib.h>

void EXTI_Init(void){
	GPIOA->MODER&=~GPIO_MODER_MODE0_Msk;
	GPIOA->MODER|=(GPIO_MODE_INPUT<<(0<<1));
	GPIOA->OTYPER|=(GPIO_OUTPUT_PUSHPULL<<0);
	GPIOA->OSPEEDR|=(GPIO_SPEED_FREQ_LOW <<(0<<1));
	GPIOA->PUPDR|=(GPIO_PULL_DOWN<<(0<<1));
//	GPIOA->BRR|=(1<<0);
	
	EXTI->IMR1|=EXTI_IMR1_IM0;
	EXTI->RTSR1|=EXTI_RTSR1_RT0;
	EXTI->FTSR1|=EXTI_FTSR1_FT0;
	NVIC_EnableIRQ(EXTI0_IRQn);
//	NVIC_SetPriority(EXTI0_IRQn, 0);
}


#define FACTOR 10000  // Example value.
int32_t smooth(int32_t data, int32_t filterVal, int32_t smoothedVal)
{ 
//    if (filterVal > FACTOR)
//    {
//        filterVal = FACTOR - 100;
//    }
//    else if (filterVal <= 0)
//    {
//        filterVal = 0;
//    }
//	smoothedVal = (data * (FACTOR - 100) + smoothedVal * 100)/FACTOR;
	
		if(abs(smoothedVal-data)<=filterVal){
			smoothedVal = (data * (FACTOR - 5000) + smoothedVal * 5000)/FACTOR;
		}

	return smoothedVal;
}

uint8_t pulse_count=0;
uint32_t last_cnt, new_cnt;
extern uint8_t new_pulse_flag;
extern uint32_t phase_shift;

extern int temper;
int32_t phase_shift_smoothed=42870;

uint32_t a[100]={0,};
uint32_t tick=1;
uint8_t k=0;
uint32_t min=0xFFFFFFFF, max=0;
uint32_t acc=0;
int32_t length_mid=0;
uint8_t second_tick=0;
#define DIV_COEF		1


void EXTI0_IRQHandler(void){
	EXTI->PR1|=EXTI_PR1_PIF0;
//	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
	
	GPIOC->ODR^=(1<<6);
	new_cnt=TIM2->CNT;
//	if(new_cnt-last_cnt>=2300 && new_cnt-last_cnt<=2700){
//		if(++pulse_count==1 && new_pulse_flag){
//			pulse_count=0;
//			phase_shift = new_cnt;
//			new_pulse_flag=0;
////			NVIC_DisableIRQ(EXTI0_IRQn);
//		};
//	}
//	else pulse_count=0;
	
			if(new_pulse_flag && ++second_tick==1){
				phase_shift = new_cnt;
				new_pulse_flag=0;
				second_tick=0;
				
				acc+=phase_shift; // length=((length>phase_shift)?phase_shift:length);
				min=((min>phase_shift)?phase_shift:min);
				max=((max<phase_shift)?phase_shift:max);
				
				if(!--tick){
					tick=DIV_COEF; // 
					length_mid=phase_shift; // acc/DIV_COEF
					a[(k<100)?k++:(k=0)]=length_mid;
					acc=0; // 
					min=0xFFFFFFFF;
					max=0;
				}
				
				phase_shift_smoothed = smooth(phase_shift, 50+(temper-200)*8, phase_shift_smoothed); // phase_shift
				phase_shift = phase_shift_smoothed;
			}
		
	last_cnt=new_cnt;
}
