#ifndef I2C_H_
#define I2C_H_
#include "stm32l4xx.h"                  // Device header

void I2C1_Init(void);
void LCD_WriteByteI2CLCD (uint8_t b);

#endif /* I2C_H_ */
