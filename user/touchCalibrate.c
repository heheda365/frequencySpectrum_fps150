#include "iFlash.h"
#include "touch.h"
#include "GUI.h"
#include "WM.h"
#include "TEXT.h"
#include "BUTTON.h"
#include "SLIDER.h"

#include "touchCalibrate.h"
extern const GUI_FONT GUI_FontHZ_MicrosoftFont_13;

#define _ID  0x5555

#define Touch_IRQ PEN
TouchScreenParam iTouchScreenParam;
//
// 屏幕校准函数
// cmd = 0，判断校准，cmd = 1；强制校准
//
void touchCalibrate(u8 cmd){
	switch(cmd){
		case 0:
			iTouchScreenParam.id = FLASH_ReadHalfWord(Addr_Base);
			if(iTouchScreenParam.id == _ID ){                      //不需要校准
				iTouchScreenParam.AD_LEFT = FLASH_ReadHalfWord(Addr_Base+2);
				iTouchScreenParam.AD_RIGHT = FLASH_ReadHalfWord(Addr_Base+4);
				iTouchScreenParam.AD_TOP = FLASH_ReadHalfWord(Addr_Base+6);
				iTouchScreenParam.AD_BOTTOM = FLASH_ReadHalfWord(Addr_Base+8);
				// 用读到的参数设置GUI
				GUI_TOUCH_Calibrate(GUI_COORD_X, 0, 239, iTouchScreenParam.AD_LEFT, iTouchScreenParam.AD_RIGHT);
				GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, 319, iTouchScreenParam.AD_TOP, iTouchScreenParam.AD_BOTTOM);
			} else {
//				TouchCalibration();
			}
			break;
			
		case 1:
			break;
		
		default:
			break;
	}
}

void touchTest(void){
	GUI_CURSOR_Show();
  GUI_CURSOR_Select(&GUI_CursorCrossL);
  GUI_SetBkColor(GUI_WHITE);
  GUI_SetColor(GUI_BLACK);
  GUI_Clear();
  GUI_DispString("Measurement of\nA/D converter values");
  while (1) {
    GUI_PID_STATE TouchState;
    int xPhys, yPhys;
    GUI_TOUCH_GetState(&TouchState);  /* Get the touch position in pixel */
    xPhys = GUI_TOUCH_GetxPhys();     /* Get the A/D mesurement result in x */
    yPhys = GUI_TOUCH_GetyPhys();     /* Get the A/D mesurement result in y */
    /* Display the measurement result */
    GUI_SetColor(GUI_BLUE);
    GUI_DispStringAt("Analog input:\n", 0, 20);
    GUI_GotoY(GUI_GetDispPosY() + 2);
    GUI_DispString("x:");
    GUI_DispDec(xPhys, 4);
    GUI_DispString(", y:");
    GUI_DispDec(yPhys, 4);
    /* Display the according position */
    GUI_SetColor(GUI_RED);
    GUI_GotoY(GUI_GetDispPosY() + 4);
    GUI_DispString("\nPosition:\n");
    GUI_GotoY(GUI_GetDispPosY() + 2);
    GUI_DispString("x:");
    GUI_DispDec(TouchState.x,4);
    GUI_DispString(", y:");
    GUI_DispDec(TouchState.y,4);
    /* Wait a while */
    GUI_Delay(100);
  };
}

//void TouchCalibration(void)
//{
////	OS_CPU_SR  cpu_sr;
//	s16 buf[5];
//	s16 temp = 0;
//	Touch_Point iTouchPoint[4];
//	u8 i;
//	TEXT_Handle hTextelse;
////		if(Menu_ID == MainMenu_ID){
////			Out_MainMenu();                     //先退出主界面
////		} else {
////			for(i = 0;i < 6;i++){
////	      WM_Move(OtherMenu_pHead,40,0);    //先退出当前界面
////		  }
////		}
//		//校准部分
//		hTextelse = TEXT_Create(0,150,240, 20, 12, WM_CF_SHOW, "请准确点击屏幕左上角的小图标",TEXT_CF_HCENTER);
//		TEXT_SetFont(hTextelse,&GUI_FontHZ_MicrosoftFont_13);
//		TEXT_SetTextColor(hTextelse,GUI_WHITE);
//			
//		_DrawIcon(20,20);
//		while(Touch_IRQ){ //等待触摸
//			OSTimeDly(1); 
//		}
//		OS_ENTER_CRITICAL(); 
//		iTouchPoint[0].x = GUI_TOUCH_X_MeasureX();
//		iTouchPoint[0].y = GUI_TOUCH_X_MeasureY();
//		OS_EXIT_CRITICAL();
//		while(!Touch_IRQ){//等待释放
//			OSTimeDly(1);
//		}
//		_ClearScreen();
//		TEXT_SetText(hTextelse , "请准确点击屏幕右上角的小图标");
//		_DrawIcon(220,20);
//		while(Touch_IRQ){//等待触摸
//			OSTimeDly(1);
//		}
//		OS_ENTER_CRITICAL(); 
//		iTouchPoint[1].x = GUI_TOUCH_X_MeasureX();
//		iTouchPoint[1].y = GUI_TOUCH_X_MeasureY();
//		OS_EXIT_CRITICAL();
//		while(!Touch_IRQ){//等待释放
//      OSTimeDly(1);
//		}
//		_ClearScreen();
//		TEXT_SetText(hTextelse , "请准确点击屏幕左下角的小图标");
//		_DrawIcon(20,300);
//		while(Touch_IRQ){//等待触摸
//			OSTimeDly(1);
//		}
//		OS_ENTER_CRITICAL(); 
//		iTouchPoint[2].x = GUI_TOUCH_X_MeasureX();
//		iTouchPoint[2].y = GUI_TOUCH_X_MeasureY();
//		OS_EXIT_CRITICAL();
//		while(!Touch_IRQ){//等待释放
//			OSTimeDly(1);
//		}
//		_ClearScreen();
//		TEXT_SetText(hTextelse , "请准确点击屏幕右下角的小图标");
//		_DrawIcon(220,300);
//		while(Touch_IRQ){//等待触摸
//			OSTimeDly(1);
//		}
//		OS_ENTER_CRITICAL(); 
//		iTouchPoint[3].x = GUI_TOUCH_X_MeasureX();
//		iTouchPoint[3].y = GUI_TOUCH_X_MeasureY();
//		OS_EXIT_CRITICAL();
//		while(!Touch_IRQ){//等待释放
//			OSTimeDly(1);
//		}
//		WM_DeleteWindow(hTextelse);
//		_ClearScreen();
//		buf[0] = _ID;
//    buf[1] = (iTouchPoint[0].x	+ iTouchPoint[2].x ) / 2;  //left
//		buf[2] = (iTouchPoint[1].x	+ iTouchPoint[3].x ) / 2;  //right
//    buf[3] = (iTouchPoint[0].y	+ iTouchPoint[1].y ) / 2;  //top
//		buf[4] = (iTouchPoint[2].y	+ iTouchPoint[3].y ) / 2;  //bottom	
//		
//		temp = (buf[1] - buf[2])/10;
//		iTouchScreenParam.AD_LEFT = buf[1] + temp;
//		iTouchScreenParam.AD_RIGHT = buf[2] - temp;
//		temp = (buf[3] - buf[4])/15;
//		iTouchScreenParam.AD_TOP = (u16)(buf[3] + temp);
//		iTouchScreenParam.AD_BOTTOM = (u16)(buf[4] - temp);
//		
//		buf[1] = iTouchScreenParam.AD_LEFT;    //left
//		buf[2] = iTouchScreenParam.AD_RIGHT;   //right
//    buf[3] = iTouchScreenParam.AD_TOP;     //top
//		buf[4] = iTouchScreenParam.AD_BOTTOM;  //bottom	
//		GUI_TOUCH_GetxyMinMax();
//		FLASH_Write(Addr_Base,(u16 *)buf,5);                          //写入Flash
//		/*********************************************************************************************************/		
////		if(Menu_ID == MainMenu_ID){
////			Go_MainMenu();                        //进入主界面
////		} else {
////			for(i = 0;i < 6;i++){
////	      WM_Move(OtherMenu_pHead,-40,0);     //进入当前界面
////		  }
////		}		
//}

