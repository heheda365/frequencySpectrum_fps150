#ifndef __TOUCHCALIBRATE_H
#define __TOUCHCALIBRATE_H

#include "stm32f10x.h"

typedef struct {
	int id;
	int AD_LEFT;
  int AD_BOTTOM;
  int AD_TOP;
  int AD_RIGHT;
}TouchScreenParam;   //´¥ÃþÆÁ²ÎÊý

typedef struct {
	int x;
	int y;
} Touch_Point;

void touchTest(void);
void touchCalibrate(u8 cmd);



#endif

