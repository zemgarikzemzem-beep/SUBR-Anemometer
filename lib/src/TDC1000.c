#include "TDC1000.h"
#include "spi.h"
#include "gpio.h"
#include "delay.h"

#define SPI_FILLER										0xFF

void TDC1000_SPIWrite(uint8_t addr, uint8_t data);
uint8_t TDC1000_SPIRead(uint8_t addr);
void TDC7200_SPIWrite(uint8_t addr, uint8_t data);


void TDC1000_Init(void){
	GPIOA->MODER&=~(0b11<<(11<<1));
	GPIOA->MODER|=(GPIO_MODE_OUTPUT<<(11<<1));
	GPIOA->MODER&=~(0b11<<(12<<1));
	GPIOA->MODER|=(GPIO_MODE_OUTPUT<<(12<<1));
	TDC1000_CSDIS;
	
	TDC1000_RES;
	delay_ms(10);
	TDC1000_START;
	delay_ms(10);
	
	TDC1000_SPIWrite(TDC1000_REG_ADR_CONFIG_0, (0x7<<TDC1000_REG_CONFIG_0_TX_FREQ_DIV_Pos)|(8<<TDC1000_REG_CONFIG_0_NUM_TX_Pos));
	
	TDC1000_SPIWrite(TDC1000_REG_ADR_CONFIG_1, TDC1000_REG_CONFIG1_DEFAULTS | (5<<TDC1000_REG_CONFIG_1_NUM_RX_Pos) | (0<<TDC1000_REG_CONFIG_1_NUM_AVG_Pos));
	
	TDC1000_SPIWrite(TDC1000_REG_ADR_CONFIG_2, TDC1000_REG_CONFIG2_DEFAULTS | (0<<TDC1000_REG_CONFIG_2_VCOM_SEL_Pos)\
													| (0b01<<TDC1000_REG_CONFIG_2_TOF_MEAS_MODE_Pos) | (1<<TDC1000_REG_CONFIG_2_DAMPING_Pos) | (0<<TDC1000_REG_CONFIG_2_MEAS_MODE_Pos));
	
	TDC1000_SPIWrite(TDC1000_REG_ADR_CONFIG_3, (0x5<<TDC1000_REG_CONFIG_3_ECHO_QUAL_THLD_Pos) | (0<<TDC1000_REG_CONFIG_3_BLANKING_Pos)\
													| (1<<TDC1000_REG_CONFIG_3_TEMP_MODE_Pos) | (1<<TDC1000_REG_CONFIG_3_TEMP_CLK_DIV_Pos) | (0<<TDC1000_REG_CONFIG_3_TEMP_RTD_SEL_Pos));
	
	TDC1000_SPIWrite(TDC1000_REG_ADR_CONFIG_4, TDC1000_REG_CONFIG4_DEFAULTS | (1<<TDC1000_REG_CONFIG_4_RECEIVE_MODE_Pos));
	
	TDC1000_SPIWrite(TDC1000_REG_ADR_TIMEOUT, TDC1000_REG_TIMEOUT_DEFAULTS | (0<<TDC1000_REG_TIMEOUT_FORCE_SHORT_TOF_Pos)\
													| (0<<TDC1000_REG_TIMEOUT_TOF_TIMEOUT_CTRL_Pos) | (0<<TDC1000_REG_TIMEOUT_ECHO_TIMEOUT_Pos));
	
	TDC1000_SPIWrite(TDC1000_REG_ADR_CLOCK_RATE, (4<<TDC1000_REG_AUTOZERO_PERIOD_Pos) | (0<<TDC1000_REG_CLOCKIN_DIV_Pos));


	uint16_t timreg=1000;
	TDC1000_SPIWrite(TDC1000_REG_ADR_TOF_0, ((timreg&0xFF)<<TDC1000_REG_TOF_0_TIMING_REG_Pos));
	TDC1000_SPIWrite(TDC1000_REG_ADR_TOF_1, (((timreg>>8)&0b11)<<TDC1000_REG_TOF_1_TIMING_REG_Pos) | (0x7<<TDC1000_REG_TOF_1_PGA_GAIN_Pos) | (0x1<<TDC1000_REG_TOF_1_LNA_FB_Pos));
}


//void TDC1000_SPIWrite(uint8_t addr, uint8_t data){
//	TDC1000_CSEN;
//	(SPI2->DR)=(((TDC1000_SPI_REG_WRITE | (addr&TDC1000_SPI_REG_ADDR_MASK))<<8)|data);
//	while((SPI2->SR&SPI_SR_BSY));
//	(uint8_t)(SPI2->DR);
//	TDC1000_CSDIS;
//}

//uint8_t TDC1000_SPIRead(uint8_t addr){
//	uint8_t data;
//	TDC1000_CSEN;
//	while((SPI2->SR&SPI_SR_BSY));
//	(SPI2->DR)=((TDC1000_SPI_REG_READ | (addr&TDC1000_SPI_REG_ADDR_MASK))<<8);
//	while(!(SPI2->SR&SPI_SR_RXNE));
//	data=(uint8_t)(SPI2->DR);
//	TDC1000_CSDIS;
//	return data;
//}

void TDC1000_SPIWrite(uint8_t addr, uint8_t data){
	TDC1000_CSEN;
	while(!(SPI2->SR&SPI_SR_TXE));
	*(__IO uint8_t*)&(SPI2->DR)=(((TDC1000_SPI_REG_WRITE | (addr&TDC1000_SPI_REG_ADDR_MASK))));
	while(!(SPI2->SR&SPI_SR_RXNE));
	(uint8_t)(SPI2->DR);
	while(!(SPI2->SR&SPI_SR_TXE));
	*(__IO uint8_t*)&(SPI2->DR)=data;
	while(!(SPI2->SR&SPI_SR_RXNE));
	(uint8_t)(SPI2->DR);
//	while((SPI2->SR&SPI_SR_BSY));
	TDC1000_CSDIS;
}

uint8_t TDC1000_SPIRead(uint8_t addr){
	uint8_t data=0;
	TDC1000_CSEN;
	while(!(SPI2->SR&SPI_SR_TXE));
	*(__IO uint8_t*)&(SPI2->DR)=((TDC1000_SPI_REG_READ | (addr&TDC1000_SPI_REG_ADDR_MASK)));
	while(!(SPI2->SR&SPI_SR_RXNE));
	(uint8_t)(SPI2->DR);
	while(!(SPI2->SR&SPI_SR_TXE));
	*(__IO uint8_t*)&(SPI2->DR)=SPI_FILLER;
	while(!(SPI2->SR&SPI_SR_RXNE));
	data=*(__IO uint8_t*)&(SPI2->DR);
	TDC1000_CSDIS;
	return data;
}


void TDC7200_Init(void){
	GPIOA->MODER&=~(0b11<<(9<<1));
	GPIOA->MODER|=(GPIO_MODE_OUTPUT<<(9<<1));
	
	GPIOB->MODER&=~(0b11<<(10<<1));
	GPIOB->MODER|=(GPIO_MODE_INPUT<<(10<<1));
	
	TDC7200_CSDIS;
	
	TDC7200_SPIWrite(0x01, 0x40);
	TDC7200_SPIWrite(0x03, 0x01);
//	TDC7200_SPIWrite(0x00, 0x03);
	
//	TDC7200_SPIWrite(TDC1000_REG_ADR_CONFIG_0, (0x7<<TDC1000_REG_CONFIG_0_TX_FREQ_DIV_Pos)|(0x5<<TDC1000_REG_CONFIG_0_NUM_TX_Pos));
}


//void TDC7200_SPIWrite(uint8_t addr, uint8_t data){
//	TDC7200_CSEN;
//	(SPI2->DR)=(((TDC7200_SPI_REG_WRITE | (addr&TDC7200_SPI_REG_ADDR_MASK))<<8)|data);
//	while((SPI2->SR&SPI_SR_BSY));
//	(uint8_t)(SPI2->DR);
//	TDC7200_CSDIS;
//}

//uint8_t TDC7200_SPIRead(uint8_t addr){
//	uint8_t data;
//	TDC7200_CSEN;
//	while((SPI2->SR&SPI_SR_BSY));
//	(SPI2->DR)=((TDC7200_SPI_REG_READ | (addr&TDC7200_SPI_REG_ADDR_MASK))<<8);
//	while(!(SPI2->SR&SPI_SR_RXNE));
//	data=(uint8_t)(SPI2->DR);
//	(uint8_t)(SPI2->DR);
//	TDC7200_CSDIS;
//	return data;
//}

void TDC7200_SPIWrite(uint8_t addr, uint8_t data){
	TDC7200_CSEN;
	while(!(SPI2->SR&SPI_SR_TXE));
	*(__IO uint8_t*)&(SPI2->DR)=(((TDC7200_SPI_REG_WRITE | (addr&TDC7200_SPI_REG_ADDR_MASK))));
	while(!(SPI2->SR&SPI_SR_RXNE));
	(uint8_t)(SPI2->DR);
	while(!(SPI2->SR&SPI_SR_TXE));
	*(__IO uint8_t*)&(SPI2->DR)=data;
	while(!(SPI2->SR&SPI_SR_RXNE));
	(uint8_t)(SPI2->DR);
//	(uint8_t)(SPI2->DR);
	TDC7200_CSDIS;
}

uint8_t TDC7200_SPIRead(uint8_t addr){
	uint8_t data=0;
	TDC7200_CSEN;
	while(!(SPI2->SR&SPI_SR_TXE));
	*(__IO uint8_t*)&(SPI2->DR)=((TDC7200_SPI_REG_READ | (addr&TDC7200_SPI_REG_ADDR_MASK)));
	while(!(SPI2->SR&SPI_SR_RXNE));
	(uint8_t)(SPI2->DR);
	while(!(SPI2->SR&SPI_SR_TXE));
	*(__IO uint8_t*)&(SPI2->DR)=SPI_FILLER;
	while(!(SPI2->SR&SPI_SR_RXNE));
	data=*(__IO uint8_t*)&(SPI2->DR);
	TDC7200_CSDIS;
	return data;
}

uint32_t TDC7200_SPIRead_Reg(uint8_t addr, uint8_t size){
	uint32_t data=0;
	
	delay_ms(1); // ??????????
	
	TDC7200_CSEN;
	while(!(SPI2->SR&SPI_SR_TXE));
	*(__IO uint8_t*)&(SPI2->DR)=((TDC7200_SPI_REG_READ | (addr&TDC7200_SPI_REG_ADDR_MASK)));
	while(!(SPI2->SR&SPI_SR_RXNE));
	(uint8_t)(SPI2->DR);
	
	while(size--){
		while(!(SPI2->SR&SPI_SR_TXE));
		*(__IO uint8_t*)&(SPI2->DR)=SPI_FILLER;
		while(!(SPI2->SR&SPI_SR_RXNE));
		data=(data<<8) | (*(__IO uint8_t*)&(SPI2->DR));
	}
	TDC7200_CSDIS;
//	delay_ms(1);
	return data;
}

//uint32_t TDC7200_SPIRead_Reg(uint8_t addr, uint8_t size){
//	uint32_t data;
//	
//	
//	TDC7200_CSEN;
//	while((SPI2->SR&SPI_SR_BSY));
//	(SPI2->DR)=((TDC7200_SPI_REG_READ | (addr&TDC7200_SPI_REG_ADDR_MASK))<<8); // 
//	while(!(SPI2->SR&SPI_SR_RXNE));
//	data=(uint16_t)(SPI2->DR);
////	TDC7200_CSDIS;
//	
////	SPI2->CR1&=~SPI_CR1_SPE;
////	SPI2->CR2&=~((0b1111<<SPI_CR2_DS_Pos));
////	SPI2->CR2|=((0b0111<<SPI_CR2_DS_Pos));
////	SPI2->CR1|=SPI_CR1_SPE;
//	
////	TDC7200_CSEN;*(__IO uint8_t*)&
//	while(!(SPI2->SR&SPI_SR_TXE));
//	(SPI2->DR)=0xFF;
//	while(!(SPI2->SR&SPI_SR_RXNE));
//	data=(data<<16)|((uint16_t)(SPI2->DR));
////	TDC7200_CSDIS;
////	TDC7200_CSEN;
////	while(!(SPI2->SR&SPI_SR_TXE));
////	*(__IO uint8_t*)&(SPI2->DR)=0xFF;
////	while(!(SPI2->SR&SPI_SR_RXNE));
////	data=(data<<8)|((SPI2->DR));
////	size/=2;
//	
////	while(--size){
////		while(!(SPI2->SR&SPI_SR_TXE));
////		(SPI2->DR)=0xFFFF;
//////	while((SPI2->SR&SPI_SR_BSY));
//////		delay_ms(100);
////		while(!(SPI2->SR&SPI_SR_RXNE));
////		data=(data<<8)|(uint8_t)(SPI2->DR);
////	}
////	(uint8_t)(SPI2->DR);
//	
//	TDC7200_CSDIS;
//	
////	SPI2->CR1&=~SPI_CR1_SPE;
////	SPI2->CR2&=~((0b1111<<SPI_CR2_DS_Pos));
////	SPI2->CR2|=((0b1111<<SPI_CR2_DS_Pos));
////	SPI2->CR1|=SPI_CR1_SPE;
//	
//	return data;
//}
