#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include "stm32f4xx.h"

extern volatile uint32_t cmd_lost_time;
extern volatile uint8_t cmd_lost_flag;

extern volatile uint32_t cmd_lost_imu_time;
extern volatile uint8_t cmd_lost_imu_flag;

#define CMD_LOST_THRESHOLD 400
#define CMD_IMU_LOST_THRESHOLD 10000
void TIM3_Init(uint16_t arr, uint16_t psc);
void TIM3_IRQHandler(void);
void Watchdog_Init(void);
void Watchdog_Trigger(void);

#endif // __WATCHDOG_H__
