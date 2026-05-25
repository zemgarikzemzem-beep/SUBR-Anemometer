#ifndef __SPI_H
#define __SPI_H

#include "stm32g4xx.h"

void SPI1_Init(void);
void SPI1_Send_Byte(uint8_t b);
void SPI2_Init(void);
uint32_t SPI2_Send_Receive_Byte(uint8_t addr);

#endif
