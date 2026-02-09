#ifndef KEYN_H_
#define KEYN_H_
#include "stm32l4xx.h"                  // Device header
#include<stdbool.h>

#define COLS 2
#define ROWS 2
#define Pin_Set(GPIOp,n) GPIOp->BSRR|=(1<<n)
#define Pin_Reset(GPIOp,n) GPIOp->BSRR|=(1<<(n+16))

void KEYBOARD_Init(void);
bool IFPRESSED(void);
char KEYPRESSED(void);
void Del_Char(void);

#endif /* KEYN_H_ */
