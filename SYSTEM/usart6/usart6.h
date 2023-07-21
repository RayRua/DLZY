#ifndef __USART6_H
#define __USART6_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 


void USART6_Init(u32 bound);
void USART6_SendOneByte(u8 dat);
void Get_Batter_Date(void);
void Sent_Batter_code(void);
#endif



