#ifndef _iFlash_h
#define _iFlash_h
#include "stm32f10x.h"

#define Addr_Base 0x08080000-2048  //Flash ���2K

u16 FLASH_ReadHalfWord(u32 addr);             //��ȡָ����ַ�İ�������
void FLASH_Write(u32 Addr,u16 *p,u16 Num);

#endif
