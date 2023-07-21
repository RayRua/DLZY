#ifndef __angle_speed_H__
#define __angle_speed_H__
#include "sys.h"
#include "STM32F4xx.h"	
//#include "stm32f4xx_conf.h"


void All_speed_Control(void);//控制车的速度

void SpeedsumCheck(unsigned char *p);
void ALL_speed_OUT(void);
void MBUS_TO_MotorSpeed(void);
#endif



