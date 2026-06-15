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

double TOF=0, Temperature=0;

void TOF_Calc(void){
	uint32_t CAL1, CAL2, CAL2_PER, TIME_1, TIME_2, CLOCK_COUNT1;
	double normLSB=0, calCount=0, offset;
	uint32_t CLOCK=10000000;
	CAL2_PER=10;
	CAL1=TDC7200_SPIRead_Reg(0x1B, 3);
	CAL2=TDC7200_SPIRead_Reg(0x1C, 3);
	TIME_1=TDC7200_SPIRead_Reg(0x10, 3);
	CLOCK_COUNT1=TDC7200_SPIRead_Reg(0x11, 3);
	TIME_2=TDC7200_SPIRead_Reg(0x12, 3);
	calCount=(double)(CAL2-CAL1)/(CAL2_PER-1);
	normLSB=(1/(double)CLOCK)/calCount; // (double)
//	offset=1/(double)CLOCK - CAL1*normLSB;
	
	TOF=(TIME_1*normLSB + (double)(CLOCK_COUNT1)/CLOCK - (double)TIME_2*normLSB)*100000000; // 64998
}


	double TOF1, TOF2, TOF3, TOF4, TOF5;

void Temper_Calc(void){
	int CAL1, CAL2, CAL2_PER, TIME_1, TIME_2, TIME_3, TIME_4, TIME_5, TIME_6, CLOCK_COUNT1, CLOCK_COUNT2, CLOCK_COUNT3, CLOCK_COUNT4, CLOCK_COUNT5;
	double normLSB=0, calCount=0, offset, R_RTD1, t_REF=0, t_RTD1, t_RTD2, t_d1, t_d2;
	uint32_t CLOCK=10000000;
	double T_TEMP=1/(double)CLOCK, R_REF=1000;
	CAL2_PER=10;
	
	TIME_1=TDC7200_SPIRead_Reg(0x10, 3);
	CLOCK_COUNT1=TDC7200_SPIRead_Reg(0x11, 3);
	
	TIME_2=TDC7200_SPIRead_Reg(0x12, 3);
	CLOCK_COUNT2=TDC7200_SPIRead_Reg(0x13, 3);
	
	TIME_3=TDC7200_SPIRead_Reg(0x14, 3);
	CLOCK_COUNT3=TDC7200_SPIRead_Reg(0x15, 3);
	
	TIME_4=TDC7200_SPIRead_Reg(0x16, 3);
	CLOCK_COUNT4=TDC7200_SPIRead_Reg(0x17, 3);
	
	TIME_5=TDC7200_SPIRead_Reg(0x18, 3);
	CLOCK_COUNT5=TDC7200_SPIRead_Reg(0x19, 3);
	
	TIME_6=TDC7200_SPIRead_Reg(0x1A, 3);
	
	CAL1=TDC7200_SPIRead_Reg(0x1B, 3);
	CAL2=TDC7200_SPIRead_Reg(0x1C, 3);
	
	calCount=(double)(CAL2-CAL1)/(CAL2_PER-1);
	normLSB=T_TEMP/calCount; // (double)
//	offset=1/(double)CLOCK - CAL1*normLSB;
	
	TOF1=((TIME_1-TIME_2)*normLSB + (double)(CLOCK_COUNT1)*T_TEMP);//*1000000
	TOF2=((TIME_1-TIME_3)*normLSB + (double)(CLOCK_COUNT2)*T_TEMP);
	TOF3=((TIME_1-TIME_4)*normLSB + (double)(CLOCK_COUNT3)*T_TEMP);
	TOF4=((TIME_1-TIME_5)*normLSB + (double)(CLOCK_COUNT4)*T_TEMP);
	TOF5=((TIME_1-TIME_6)*normLSB + (double)(CLOCK_COUNT5)*T_TEMP);
	
//	t_RTD1=t_REF/2;
//	t_RTD2=0;
//	t_d1=(51*T_TEMP)+(t_RTD1*0.55);
//	t_d2=(51*T_TEMP)+(t_RTD2*0.55);
	
	t_REF=TOF1;
	t_RTD1=TOF5-TOF4;
	R_RTD1=R_REF*t_REF/t_RTD1;
	Temperature=(R_RTD1-R_REF)/(R_REF*0.00385); // 
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
	
		TDC1000_TEMP_MEASURE_MODE_ON;
		TDC7200_5_STOP;
	
	TFT_Fill_Color(YELLOW);
	
//	TDC1000_SPIWrite(0x01, 0x45);
//	answ=TDC1000_SPIRead(0x00);
//	sprintf(tmp_str, "0x%X", answ);
//	TFT_Send_Str(20, 90, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
	
//	time1=TDC7200_SPIRead_Reg(0x01, 4);
//			TDC1000_SPIWrite(TDC1000_REG_ADR_CONFIG_0, 0x45);
//	TIM3_Init();
//	TIM1_Init();
//	TIM2_Init();
//	EXTI_Init();
	
	while(1){
		
//		TDC1000_TEMP_MEASURE_MODE_ON;
//		TDC7200_5_STOP;
		
		
		TDC7200_SPIWrite(0x00, 0x83);     //  0x03
		
		while(TDC7200_INTBUP);
		
		if(TDC7200_INTBDW){
//			time1=TDC7200_SPIRead_Reg(0x1C, 3); // 
//			clock1=TDC7200_SPIRead_Reg(0x11, 3);
			
//			TOF_Calc();
			Temper_Calc();
		}
		
		delay_ms(100);
		
		answ=TDC7200_SPIRead(0x02);
		if((answ&((1<<0)|(1<<1)|(1<<2))) == ((1<<0)|(0<<1)|(0<<2))){
			
			
//			int TOF_corr=65080-(temper-222)*7;
//			sprintf(tmp_str, "%5d м/с", ((((int)TOF<=TOF_corr) && ((TOF_corr-(int)TOF)/8+4)<60))?((TOF_corr-(int)TOF)/8+4):0); //    64926
//			TFT_Send_Str(20, 80, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
			
			sprintf(tmp_str, "%7d.%dC", (uint32_t)Temperature, ((uint32_t)(Temperature*10)%10));
			TFT_Send_Str(20, 80, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
			sprintf(tmp_str, "0x%X", (uint32_t)TDC1000_SPIRead(0x07));
			TFT_Send_Str(20, 110, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
			sprintf(tmp_str, "%5d", (uint32_t)(TOF1*1000000));
			TFT_Send_Str(20, 140, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
			sprintf(tmp_str, "%5d", (uint32_t)((TOF5)*1000000));
			TFT_Send_Str(20, 170, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
			sprintf(tmp_str, "0x%X", TDC1000_SPIRead(0x01)); //%5d(uint32_t)(TOF5*1000000)
			TFT_Send_Str(20, 200, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
			
		}
		else TFT_Fill_Color(YELLOW);
	
		
		
//		if(((data_th[0]+data_th[1]+data_th[2]+data_th[3])&0xFF)==data_th[4] && DHT22_GetData(data_th)){ // 
//			hum=(((data_th[0])<<8)+data_th[1]); // (float) / 10
//			temper=(((data_th[2] & 0x3F)<<8)+data_th[3]); // (float) / 10
//			
//		}
//		sprintf(tmp_str, "%2d.%dC   %2d.%d%%", temper/10, temper%10, hum/10, hum%10);
//		TFT_Send_Str(10, 200, tmp_str, strlen(tmp_str), Font_16x26, RED, YELLOW);
		
		

//		TDC1000_TEMP_MEASURE_MODE_OFF;

		
	}
}

void HardFault_Handler(void){
	uint8_t i=1;
	while(i);
}