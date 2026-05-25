#include "spi.h"
#include "gpio.h"

void SPI1_Init(void){
	RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;
//	RCC->AHB1ENR|=RCC_AHB1ENR_DMA1EN;
	
	GPIOA->MODER&=~0x0000CC00;
	GPIOA->MODER|=((GPIO_MODE_ALTERNATE<<(5<<1))|(GPIO_MODE_ALTERNATE<<(7<<1)));
	GPIOA->OTYPER|=((GPIO_OUTPUT_PUSHPULL<<5)|(GPIO_OUTPUT_PUSHPULL<<7));
	GPIOA->OSPEEDR|=((GPIO_SPEED_FREQ_VERY_HIGH <<(5<<1))|(GPIO_SPEED_FREQ_VERY_HIGH <<(7<<1)));
	GPIOA->AFR[0]|=0x50500000;
	
	SPI1->CR1=0;
	SPI1->CR1|=((0b001<<SPI_CR1_BR_Pos)|SPI_CR1_MSTR|SPI_CR1_SSM|SPI_CR1_SSI|SPI_CR1_BIDIMODE|SPI_CR1_BIDIOE);//|SPI_CR1_CPHA|SPI_CR1_CPOL
	SPI1->CR2=0x0000;
	SPI1->CR2|=((0b0111<<SPI_CR2_DS_Pos)); //|SPI_CR2_FRXTH|SPI_CR2_NSSP
	
//	SPI1->CR2|=SPI_CR2_TXDMAEN;
//	DMA1_CSELR->CSELR|=(0b0001<<DMA_CSELR_C3S_Pos);
	
	SPI1->CR1|=SPI_CR1_SPE;
}

inline void SPI1_Send_Byte(uint8_t b){
//	DMA1_Channel1->CCR=0;
//	DMA1_Channel1->CCR|=(DMA_CCR_MINC|DMA_CCR_DIR);
//	DMA1_Channel1->CPAR=(uint32_t)(&(SPI1->DR));
//	DMA1_Channel1->CMAR=(uint32_t)&b;
//	DMA1_Channel1->CNDTR=sizeof(b);
//	DMA1_Channel1->CCR|=DMA_CCR_EN;
//	while((SPI1->SR&SPI_SR_BSY));
	*(__IO uint8_t*)&(SPI1->DR)=b;   // фишка для записи одного байта в DR!!! || !(SPI1->SR&SPI_SR_TXE)
	while((SPI1->SR&SPI_SR_BSY));
}

//--------------------------------------------------------------------------------------------------------

void SPI2_Init(void){
	RCC->APB1ENR1|=RCC_APB1ENR1_SPI2EN;
//	RCC->AHB1ENR|=RCC_AHB1ENR_DMA1EN;
	
	GPIOB->MODER&=~((0b11<<(13<<1))|(0b11<<(14<<1))|(0b11<<(15<<1)));
	GPIOB->MODER|=((GPIO_MODE_ALTERNATE<<(13<<1))|(GPIO_MODE_ALTERNATE<<(14<<1))|(GPIO_MODE_ALTERNATE<<(15<<1)));
	GPIOB->OTYPER|=((GPIO_OUTPUT_PUSHPULL<<13)|(GPIO_OUTPUT_PUSHPULL<<14)|(GPIO_OUTPUT_PUSHPULL<<15));
	GPIOB->OSPEEDR|=((GPIO_SPEED_FREQ_VERY_HIGH <<(13<<1))|(GPIO_SPEED_FREQ_VERY_HIGH <<(14<<1))|(GPIO_SPEED_FREQ_VERY_HIGH <<(15<<1)));
	GPIOB->AFR[1]|=((5<<((13-8)<<2))|(5<<((14-8)<<2))|(5<<((15-8)<<2)));
	
	SPI2->CR1=0;
	SPI2->CR1|=((0b101<<SPI_CR1_BR_Pos)|SPI_CR1_MSTR|SPI_CR1_SSM|SPI_CR1_SSI);//|SPI_CR1_CPHA|SPI_CR1_CPOL|SPI_CR1_BIDIMODE|SPI_CR1_BIDIOE
	SPI2->CR2=0x0000;
	SPI2->CR2|=((0b0111<<SPI_CR2_DS_Pos)|SPI_CR2_NSSP|SPI_CR2_FRXTH); //
	
//	SPI2->CR2|=SPI_CR2_RXDMAEN;
//	DMA1_CSELR->CSELR|=(0b0001<<DMA_CSELR_C3S_Pos);
	
	SPI2->CR1|=SPI_CR1_SPE;
}

inline uint32_t SPI2_Send_Receive_Byte(uint8_t addr){
	uint8_t data[4]={0,};
	while(!(SPI2->SR&SPI_SR_TXE));
	SPI2->DR=(0x00|addr);
	while(!(SPI2->SR&SPI_SR_TXE));
	DMA1_Channel1->CCR=0;
	DMA1_Channel1->CCR|=(DMA_CCR_MINC); // |DMA_CCR_DIR
	DMA1_Channel1->CPAR=(uint32_t)(&(SPI2->DR));
	DMA1_Channel1->CMAR=(uint32_t)data;
	DMA1_Channel1->CNDTR=4;
	DMA1_Channel1->CCR|=DMA_CCR_EN;
	while((SPI2->SR&SPI_SR_BSY));
//	*(__IO uint8_t*)&(SPI2->DR)=b;   // фишка для записи одного байта в DR!!! || !(SPI2->SR&SPI_SR_TXE)
//	while((SPI2->SR&SPI_SR_BSY));
//	while(!(SPI2->SR&SPI_SR_RXNE));
	return (((data[0]<<24) | (data[1]<<16) | (data[2]<<8) | data[3]) & 0x3FFFFF);
}

