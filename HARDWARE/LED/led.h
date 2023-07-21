#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED端口定义
#define LED0 PAout(4)	// DS0
#define LED1 PAout(5)	// DS1	 

//数码管端口定义

#define SEGS1 PDout(7)	// DS0
#define SEGS2 PBout(3)	// DS1	 

#define SEGA  PBout(4)	// DS0
#define SEGB  PBout(5)	// DS1	
#define SEGC  PBout(6)	// DS0
#define SEGD  PBout(7)	// DS1	
#define SEGE  PBout(8)	// DS0
#define SEGF  PBout(9)	// DS1	
#define SEGG  PEout(0)	// DS0
#define SEGDP PEout(1)	// DS1	


void LED_Init(void); //初始化		 	
void DigitalTube_LED_Init(void);
void Seg_Display(u8 x);


#endif
