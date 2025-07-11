#ifndef __PIN_H
#define __PIN_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

void pin_init(GPIO_TypeDef* GPIOx, uint16_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd);
uint8_t pin_read(GPIO_TypeDef* GPIOx, uint16_t pin);

#endif // __PIN_H 
