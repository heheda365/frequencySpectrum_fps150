#include "iFlash.h"


/********************************************************************************************************** 
** Function name:      FLASH_ReadHalfWord
** Descriptions:       读取指定地址的半字数据
** input parameters:   u32 addr：要读的地址
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
u16 FLASH_ReadHalfWord(u32 addr)
{
	return *(vu16*)addr; 
}

/********************************************************************************************************** 
** Function name:      FLASH_Write
** Descriptions:       Flash写
** input parameters:   u32 Addr：起始地址
                       u16 *p ： 要写的内容（半字为单位）
                       u16 Num： 要写的半字数目 
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
void FLASH_Write(u32 Addr,u16 *p,u16 Num)
{	
	u16 i;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(Addr);
	for(i = 0;i < Num;i++){
		FLASH_ProgramHalfWord(Addr,*(p + i));
	  Addr += 2;
	} 
	FLASH_Lock();
}
