#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"



void TIM3_Int_Init(u16 arr,u16 psc);
u8 getTime10MsFlag(void);
u8 getTime100MsFlag(void);
uint64_t getTime(void);
 
#endif
