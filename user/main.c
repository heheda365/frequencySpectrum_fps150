#include "delay.h"
#include "timer.h"
#include "voice.h"
#include "lcd_ex.h"
#include "LCD_Driver.h"
#include "key.h"

static uint64_t timeLast = 0;
uint16_t fps = 0;
uint16_t fpsCut = 0;
bool isStop = false;

int main(void){
	SystemInit();			//初始化RCC 设置系统主频为72MHZ
	delay_init();	     	//延时初始化 
	TIM3_Int_Init(99,7199);	//10Khz的计数频率，计数到100为10ms  
	LCD_Init();
	keyInit();
	signalGatherInit();
	displayFLineAndVWaveInit();
	
	while(1)
	{	
		if(!isStop){
			dsp();
			displayFLineAndVWave();
			fpsCut ++;
		}
		
		if(getTime10MsFlag()){
			keyProcess();
			
			if(key.isPress == true){
				key.isPress = false;
				isStop = !isStop;
			}
		}
		if(getTime() - timeLast > 100){		// 1s
			timeLast = getTime();
			fps = fpsCut;
			fpsCut = 0;
		}
	}
}






