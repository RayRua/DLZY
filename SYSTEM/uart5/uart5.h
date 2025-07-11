#ifndef __UART5_H
#define __UART5_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

void UART5_Init(void);
void UART5_SendOneByte(u8 dat);
void Get_Remote_channel_Date(void);
void Sent_Modbus_RTU(void);
void gimbal_task(void);
#endif

