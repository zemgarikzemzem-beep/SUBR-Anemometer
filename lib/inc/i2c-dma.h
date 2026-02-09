#ifndef I2CDMA_H_
#define I2CDMA_H_
#include "stm32l4xx.h"                  // Device header

void I2C1_DMA_Init(void);
void LCD_WriteByteI2CLCD (uint8_t b);

#endif /* I2CDMA_H_ */
