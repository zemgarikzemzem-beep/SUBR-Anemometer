#ifndef __SPI_H
#define __SPI_H

#include "stm32g4xx.h"

void SPI1_Init(void);
void SPI1_Send_Byte(uint8_t b);
void SPI2_Init(void);
uint8_t SPI2_Send_Byte(uint8_t b);

#endif
