#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h"

#define NOP         0xFF

void bsp_SPI2Init(void);
u8 SPI_NRF_RW(u8 dat);
u8 SPI_NRF_WriteReg(u8 reg,u8 dat);
u8 SPI_NRF_ReadReg(u8 reg);
#endif

