#ifndef DHT22_H
#define DHT22_H

#include "stm32g4xx.h"

void DHT22_Init(void);
uint8_t DHT22_GetData(uint8_t *data);

#endif
