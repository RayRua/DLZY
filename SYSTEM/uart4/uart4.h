#ifndef __UART4_H
#define __UART4_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

void UART4_Init(u32 bound);
void UART4_SendOneByte(u8 dat);
void Sent_MagneticEncoder_Date(u8 ID);//获取每一路的编码器数据
void Get_MagneticEncoder_Date(void);
void Read_AS5600_State(void);
#endif

