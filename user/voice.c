#include "voice.h"
#include "ADC_DMA.h"
#include "stm32_dsp.h"
#include "table_fft.h"
#include "math.h"
#include "lcd_ex.h"
#include "LCD_Driver.h"

#define PI2  6.28318530717959
#define Fs   35714			// ����Ƶ��
#define F_LINE_COLOR	LIGHTGREEN
#define F_BACK_COLOR	0
#define V_WAVE_COLOR	GBLUE


#define DMA_BUF_SIZE	2048
uint16_t DMABuf[DMA_BUF_SIZE] = {0};
extern uint16_t TableFFT[];

#define NPT 1024            /* NPT = No of FFT point*/
int32_t  lBUFIN[NPT];         /* Complex input vector */
int32_t	 lBUFOUT[NPT];        /* Complex output vector */
uint16_t lBufMagArray[NPT/2];

/*
 * Ƶ����ʾ����
 */
void frequencyLinePlay(void) {
	int16_t hTemp = 0;
	for(int i=0; i<320; i++){
		hTemp = 240 - lBufMagArray[i + 1];		// �ӵڶ�����ʼ��ʾ
		if(hTemp < 0) hTemp = 0;
		POINT_COLOR = F_BACK_COLOR;
		LCD_drawVerLine(i, 0, hTemp - 1); 		// ����ɫ��
		POINT_COLOR = F_LINE_COLOR;
		LCD_drawVerLine(i, hTemp, 239);			// ǰ����
	}
}

/*
 * ������
 * high ���ߵĸ߶�
 * color ������ɫ
 */
void drawVerLine(uint16_t x, uint16_t y, uint16_t high, uint16_t color) {
	uint16_t high1 = high;
	LCD_SetCursor(y, x);       				//���ù��λ��
	LCD_WriteRAM_Prepare();     				//��ʼд��GRAM
	while (high1--) {
		LCD->LCD_RAM = color;
	}
}
//
// ��ָ����ɫ�ĵ�
//
void drawPoint(u16 x,u16 y,u16 color)
{
	LCD_SetCursor(y,x);//���ù��λ�� 
	LCD->LCD_RAM = color; 
}
//
// ��ʾ��ʼ��
//
 void displayFLineAndVWaveInit(void){
 	
 	LCD_Clear(F_BACK_COLOR);
 	LCD_Scan_Dir(U2D_L2R);					//ɨ�跽��
	LCD_SetCursor(0, 0);       				//���ù��λ��
	LCD_WriteRAM_Prepare();     			//��ʼд��GRAM
	
 }
/*
 * ��ʾ��ѹ���κ�Ƶ������
 */
 #define V_HIGHT	60				// ��ѹ���εĸ߶�
 #define V_DC		2000			// ��ѹ��ADCƽ��ֵ
 #define V_DIV		25				// ÿ�����صĵ�ѹ��С			 	
 void displayFLineAndVWave(void){
 	int16_t hTemp = 0;				// Ƶ��
 	int16_t vhTemp = 0;				// ��ѹ
 	// ������һ�ε�Ƶ�׺Ͳ���
 	static uint8_t hTempLast[320] = {0};
 	static uint8_t vhTempLast[320]	= {0};	

	for(int i=0; i<320; i++){
		hTemp = 239 - lBufMagArray[i + 1];	// �ӵڶ�����ʼ��ʾ
		vhTemp = V_HIGHT - (lBUFIN[2*i+1] + lBUFIN[2*i] - 2 * V_DC) / V_DIV / 2;
		if(hTemp < 0) hTemp = 0;			// �հ����߶�
		if(vhTemp < 0) vhTemp = 0;
		if(vhTemp > 239) vhTemp = 239;
			
		if(hTemp < vhTempLast[i]){			// ����������һ��V��
			drawPoint(i, vhTempLast[i], F_LINE_COLOR);
		} else {							// ������
			drawPoint(i, vhTempLast[i], F_BACK_COLOR);
		}	
		if(hTemp < hTempLast[i]){
				// ���߼ӳ�
				drawVerLine(i, hTemp, hTempLast[i] - hTemp, F_LINE_COLOR);
		} else if(hTemp > hTempLast[i]){
			drawVerLine(i, hTempLast[i], hTemp - hTempLast[i], F_BACK_COLOR);
		}
		// ����ѹ���ε�
		drawPoint(i, vhTemp, V_WAVE_COLOR);
		// ��¼����һ�ε�ֵ	
		vhTempLast[i] = vhTemp;
		hTempLast[i] = hTemp;
//		if(hTemp < vhTemp){
//			for(int i=0; i < hTemp; i++){
//				LCD->LCD_RAM=F_BACK_COLOR;
//			}
//			for(int i=0; i < vhTemp - hTemp; i++){
//				LCD->LCD_RAM=F_LINE_COLOR;
//			}
//			LCD->LCD_RAM=V_WAVE_COLOR;
//			for(int i=0; i< 239 - vhTemp; i++){
//				LCD->LCD_RAM=F_LINE_COLOR;
//			}
//		} else {
//			for (int i=0; i<vhTemp; i++) {
//				LCD->LCD_RAM=F_BACK_COLOR;
//			}
//			LCD->LCD_RAM=V_WAVE_COLOR;
//			for (int i=0; i<hTemp - vhTemp; i++) {
//				LCD->LCD_RAM=F_BACK_COLOR;
//			}
//			for (int i=0; i<239 - hTemp; i++) {
//				LCD->LCD_RAM=F_LINE_COLOR;
//			}
//		}
		
	}
 }
//
// �źŲɼ������ʼ��
//
void signalGatherInit(void){
	ADC1_DMAInit(DMABuf, DMA_BUF_SIZE);
}

uint16_t intSqrt(uint32_t a){
	uint32_t rem = 0;
	uint32_t root = 0;
	uint32_t divisor = 0;
	for(int i=0; i<16; i++){
		root <<= 1;
		rem = ((rem << 2) + (a >> 30));
		a <<= 2;
		divisor = (root<<1) + 1;
		if(divisor <= rem){
		rem -= divisor;
		root++;
	}
}
return (uint16_t)(root);
}
/******************************************************************
��������:GetPowerMag()
��������:�������г����ֵ
����˵��:
������ע:�Ƚ�lBufOutArray�ֽ��ʵ��(X)���鲿(Y)��Ȼ������ֵ(sqrt(X*X+Y*Y)
*******************************************************************/
void GetPowerMag(void)
{
//    int X,Y,Mag;
//	int j = 0;
//	int16_t * lBUFOUTTemp1 = (int16_t *)lBUFOUT;
//	int16_t * lBUFOUTTemp2 = (int16_t *)((uint32_t)lBUFOUT + 2);
//    for(int i=0; i<NPT; i+=2, j++)
//    {
//        X  = lBUFOUTTemp2[i];
//        Y  = lBUFOUTTemp1[i];
//        Mag = intSqrt(X * X + Y * Y);
//        lBufMagArray[j] = (unsigned long)(Mag * 2);
//    }

	int X,Y,Mag;
    unsigned short i;
    for(i=0; i<321; i++)
    {
        X  = (lBUFOUT[i] << 16) >> 16;
        Y  = (lBUFOUT[i] >> 16);
        Mag = intSqrt(X * X + Y * Y);
        lBufMagArray[i] = (unsigned long)(Mag * 2);
    }
}

//
// �źŶ�ȡ �任
//
#define WAIT_TIME 5
void dsp(void){
	uint16_t pointerCou = 0;
	uint16_t i = 0;
	pointerCou = DMA_GetCurrDataCounter(DMA1_Channel1);
	if(pointerCou > (NPT + WAIT_TIME)){
		for(i = 0; i < NPT; i ++){
			lBUFIN[i] = (int32_t)DMABuf[i + WAIT_TIME + DMA_BUF_SIZE - pointerCou];
		}
	} else {
		if(pointerCou > WAIT_TIME){
			for(i = 0; i < pointerCou - WAIT_TIME; i++){
				lBUFIN[i] = (int32_t)DMABuf[i + WAIT_TIME + DMA_BUF_SIZE - pointerCou];
			}
			for(int j=0; i < NPT + WAIT_TIME - pointerCou; i++, j++){
				lBUFIN[i] = (int32_t)DMABuf[j];
			}
		} else {
			for(i = WAIT_TIME; i < NPT + WAIT_TIME; i++){
				lBUFIN[i] = (int32_t)DMABuf[i];
			}
		}
	}
	cr4_fft_1024_stm32(lBUFOUT, lBUFIN, NPT);			// FFT
	GetPowerMag();										// ����Ƶ��
}


//
// ���ڲ���FFT
//
//
void InitBufInArray(void)
{
     u16 i=0;
  float fx;
  for(i=0;i<NPT;i++)
  {
    fx  = 400 * sin(PI2*i*53.0/Fs) + 211 * sin(PI2*i*914.0/Fs) + 200*sin(PI2*i*1541.0/Fs);
    lBUFIN[i] = ((s16)fx)<<16;
  }
}










