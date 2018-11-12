#include "delay.h"
#include "key.h"

key_t key;
//
//
// 按键对应的相关端口初始化
//
void keyInit(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
  
  /*开启相应IO端口的时钟*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
                          
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	key.isPress = false;
	key.data = 0;
}

//
//
// 使用于10ms的循环中
//
void keyProcess(void){
	if(keyIn() == 0){
		delay_ms(10);
		if(keyIn() == 0){
			key.isPress = true;
			key.data = 1;
			while(!keyIn()){
				
			}
		}
	}
}

