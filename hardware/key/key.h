#ifndef KEY_H__
#define KEY_H__

#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"

#define keyIn()	 GPIO_ReadInputDataBit ( GPIOA, GPIO_Pin_0)
#define helpKeyIn() GPIO_ReadInputDataBit ( GPIOD, GPIO_Pin_0)

typedef struct{
	bool isPress;
	uint8_t data;
} key_t;

void keyInit(void);
void keyProcess(void);



extern key_t key;

#endif

