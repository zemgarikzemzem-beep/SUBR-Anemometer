#include "stm32g4xx.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>

#include "rcc.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"
#include "tft.h"
#include "fonts.h"
#include "dht22.h"
#include "exti.h"
#include "delay.h"
#include "TDC1000.h"


//void delay(__IO uint32_t tck)
//{
//	__asm__ __volatile__(
//			"mov	r0,#tck\n"
//		"label:\n"
//			"subs	r0, r0, #1\n"
//			"bne	label"
//	);
//}

uint8_t data_th[5]={0,};
int temper, hum;

extern int32_t length_mid, phase_shift;
char tmp_str[20]={0,};

uint8_t answ=0;
uint32_t time1=0, clock1=0;

double TOF=0, normLSB=0, calCount=0;

void TOF_Calc(void){
	uint32_t CAL1, CAL2, CAL2_PER, CLOCK, TIME_1;
	CLOCK=10000000;
	CAL2_PER=10;
	CAL1=TDC7200_SPIRead_Reg(0x1B, 3);
	CAL2=TDC7200_SPIRead_Reg(0x1C, 3);
	TIME_1=TDC7200_SPIRead_Reg(0x10, 3);
	calCount=(double)(CAL2-CAL1)/(CAL2_PER-1);
	normLSB=(double)(1/(double)CLOCK)/calCount;
	TOF=TIME_1*normLSB*1000000000;
}

int main(void){
	Clock_Init();
	GPIO_Init();
	DHT22_Init();
	
	SPI1_Init();
	TFT_Init();
	
	SPI2_Init();
	MCO_Init();  // Ћибо TIM1, либо - это !!!
	TDC1000_Init();
	TDC7200_Init();
	
	TFT_Fill_Color(YELLOW);
	
//	TDC1000_SPIWrite(0x01, 0x45);
	answ=TDC1000_SPIRead(0x00);
	sprintf(tmp_str, "0x%X", answ);
	TFT_Send_Str(20, 90, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
	
//	time1=TDC7200_SPIRead_Reg(0x01, 4);
//			TDC1000_SPIWrite(TDC1000_REG_ADR_CONFIG_0, 0x45);
//	TIM3_Init();
//	TIM1_Init();
//	TIM2_Init();
//	EXTI_Init();
	
	while(1){
		
		TDC7200_SPIWrite(0x00, 0x03);
		
		while(TDC7200_INTBUP);
		
		if(TDC7200_INTBDW){
//			time1=TDC7200_SPIRead_Reg(0x1C, 3); // 
//			clock1=TDC7200_SPIRead_Reg(0x11, 3);
			TOF_Calc();
		}
		
		delay_ms(100);
		
		answ=TDC7200_SPIRead(0x02);
		if((answ&((1<<0)|(1<<1)|(1<<2))) == ((1<<0)|(0<<1)|(0<<2))){
			answ=TDC1000_SPIRead(TDC1000_REG_ADR_ERROR_FLAGS);
			sprintf(tmp_str, "0x%X", answ);
			TFT_Send_Str(20, 90, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		
			answ=TDC7200_SPIRead(0x02);
			sprintf(tmp_str, "0x%X", answ);
			TFT_Send_Str(20, 120, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		
			sprintf(tmp_str, "0x%X", time1);
			TFT_Send_Str(20, 150, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		
			sprintf(tmp_str, "%lf", TOF);
			TFT_Send_Str(20, 180, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		}
		else TFT_Fill_Color(YELLOW);
	
	
//		answ=TDC7200_SPIRead(0x1C);
//		sprintf(tmp_str, "0x%X", answ);
//		TFT_Send_Str(20, 210, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
	
//		answ=TDC7200_SPIRead(0x13);
//		sprintf(tmp_str, "0x%X", answ);
//		TFT_Send_Str(20, 180, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		//SPI1_Send_Byte(0xAA);
//		GPIOC->ODR^=(1<<6);
		
//		if(((data_th[0]+data_th[1]+data_th[2]+data_th[3])&0xFF)==data_th[4] && DHT22_GetData(data_th)){ // 
//			hum=(((data_th[0])<<8)+data_th[1]); // (float) / 10
//			temper=(((data_th[2] & 0x3F)<<8)+data_th[3]); // (float) / 10
//			
//		}
//		sprintf(tmp_str, "%2d.%dC   %2d.%d%%", temper/10, temper%10, hum/10, hum%10);
//		TFT_Send_Str(10, 200, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
//		
//		sprintf(tmp_str, "%5d м/с", (length_mid<=1820-(temper-220)*5)?((1820-(temper-220)*5-length_mid)/10+4):0); // phase_shift 40010 243
//		TFT_Send_Str(20, 80, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);


		
	}
}

void HardFault_Handler(void){
	uint8_t i=1;
	while(i);
}