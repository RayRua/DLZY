#ifndef __USART2_H__
#define __USART2_H__

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

void USART2_Init(u32 bound);
void USART2_Init_NoIRQ(u32 bound);
//u8 usart2GetByte(u8* pChar);
void USART2_SendOneByte(u8 dat);
void USART2_SendStr(u8 *pucStr, u8 ulNum);

void Sent_SteeringMotor_Pozition(void);
void Uart2_Send(uint8_t* data, uint32_t len);
#endif

