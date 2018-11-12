#ifndef VOICE_H__
#define VOICE_H__

#include "stm32f10x.h"

void signalGatherInit(void);
void dsp(void);
void frequencyLinePlay(void);
void InitBufInArray(void);
void displayFLineAndVWave(void);
 void displayFLineAndVWaveInit(void);
#endif

