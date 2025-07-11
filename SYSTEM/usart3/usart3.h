#ifndef __USART3_H__
#define __USART3_H__

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

void USART3_Init(u32 bound);
void USART3_SendOneByte(u8 dat);
void USART3_SendStr(u8 *pucStr, u8 ulNum);
void Sent_MotorSpeed_code(void);
void WheelReturnedDataHandle(void);

typedef struct
{
    int16_t ReturnedWheelSpeedValue;
    uint8_t ReturnedWheelErrorCode;
    uint16_t ReturnedWheelHallCnt;
} ReturnedWheel;


#endif

