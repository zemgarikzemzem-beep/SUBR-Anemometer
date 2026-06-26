#include "delay.h"

void delay_ms(__IO uint32_t ms_num){
	ms_num*=SystemCoreClock/1000/9;
	while(--ms_num);
}

void delay_us(__IO uint32_t us_num){
	us_num*=SystemCoreClock/1000000/9;
	while(--us_num);
}
