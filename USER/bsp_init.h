#ifndef __BSP_INIT_H
#define __BSP_INIT_H

#include "stm32f4xx.h"

void bsp_init(void);
void bsp_init_no_irq(void);
void bsp_enable_irq(void);

#endif
