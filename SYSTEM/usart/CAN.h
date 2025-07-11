#ifndef __CAN_H_
#define __CAN_H_

#include "stm32f4xx.h"

#define CAN1_RXSIZE  256

void CanConfiguartion(void);
void CAN1_Send(uint8_t *pdata,uint8_t Len);
uint8_t can1GetByte(uint8_t* pChar);

#endif
