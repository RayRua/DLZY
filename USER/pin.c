#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

/**
 * @brief  初始化引脚
 * @param  GPIOx: GPIOA~GPIOI
 * @param  pin: GPIO_Pin_x
 * @param  mode: GPIO_Mode_IN 或 GPIO_Mode_OUT
 * @param  otype: GPIO_OType_PP 或 GPIO_OType_OD
 * @param  pupd: GPIO_PuPd_NOPULL, GPIO_PuPd_UP, GPIO_PuPd_DOWN
 * @retval 无
 */
void pin_init(GPIO_TypeDef* GPIOx, uint16_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = pin;
    GPIO_InitStruct.GPIO_Mode = mode;
    GPIO_InitStruct.GPIO_OType = otype;
    GPIO_InitStruct.GPIO_PuPd = pupd;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
 * @brief  读取引脚电平
 * @param  GPIOx: GPIOA~GPIOI
 * @param  pin: GPIO_Pin_x
 * @retval 0: 低电平，1: 高电平
 */
uint8_t pin_read(GPIO_TypeDef* GPIOx, uint16_t pin)
{
    return (GPIOx->IDR & pin) ? 1 : 0;
} 
