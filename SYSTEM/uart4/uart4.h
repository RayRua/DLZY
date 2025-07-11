#ifndef __UART4_H
#define __UART4_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 


void UART4_Init(u32 bound);
void UART4_SendOneByte(u8 dat);
void Uart4_Send(uint8_t* data, uint32_t len);
#endif

