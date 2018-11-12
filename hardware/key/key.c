#include "delay.h"
#include "key.h"

key_t key;
//
//
// ������Ӧ����ض˿ڳ�ʼ��
//
void keyInit(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
  
  /*������ӦIO�˿ڵ�ʱ��*/
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
// ʹ����10ms��ѭ����
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

