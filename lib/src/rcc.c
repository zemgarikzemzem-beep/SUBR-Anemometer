#include "rcc.h"
#include "gpio.h"

int Clock_Init(void){
	FLASH->ACR&=~FLASH_ACR_LATENCY_Msk;
	FLASH->ACR|=(4<<FLASH_ACR_LATENCY_Pos);
	
	RCC->CR|=RCC_CR_HSEON;
	while(!(RCC->CR&(RCC_CR_HSERDY)));
	
	RCC->PLLCFGR=(((2-1)<<RCC_PLLCFGR_PLLM_Pos) | (40<<RCC_PLLCFGR_PLLN_Pos) | RCC_PLLCFGR_PLLREN | RCC_PLLCFGR_PLLSRC_HSE); // Обязательно включить PLLR !!!
	RCC->CR|=RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));
	
//	RCC->CSR |= (RCC_CSR_LSION);               // For RTC enable
//	while (!(RCC->CSR & RCC_CSR_LSIRDY));
	
	RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN;
	RCC->APB1ENR1|=RCC_APB1ENR1_PWREN;
	
	RCC->CFGR&=~RCC_CFGR_SW_Msk;
	RCC->CFGR|=RCC_CFGR_SW_PLL;
	while((RCC->CFGR&RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);
	
//	RCC->CR&=~RCC_CR_HSION;  // Aey ?aaiou n iaiyou? - ia au?oaaou !!!
	return 0;
}

void MCO_Init(void){
	GPIOA->MODER&=~GPIO_MODER_MODE8_Msk;
	GPIOA->MODER|=(GPIO_MODE_ALTERNATE<<(8<<1));
	GPIOA->AFR[1]|=(0b0000<<GPIO_AFRH_AFSEL8_Pos);
	
	RCC->CFGR|=((0b0101<<RCC_CFGR_MCOSEL_Pos) | RCC_CFGR_MCOPRE_DIV8); // 
}
