#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED端口定义
#define LED0 PAout(4)	// DS0
#define LED1 PAout(5)	// DS1	 

//数码管端口定义

#define SER PEout(7)	// DS0
#define RCLK PEout(8)	// DS1	 
#define CLK  PEout(9)	// DS0

#define A1  PBout(6)	// DS1	
#define A2  PBout(7)	// DS0
#define A3  PBout(8)	// DS1	
#define A4  PBout(9)	// DS0

void FLAG_Init(void);
void LED_Init(void); //初始化		 	
void DigitalTube_LED_Init(void);
void SMG_Output(u8 data);
void LED_Display(u32 seconds, u8 index);

#endif
