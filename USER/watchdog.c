#include "stm32f4xx.h"
#include "watchdog.h"

volatile uint32_t cmd_lost_time = CMD_LOST_THRESHOLD;      // 单位: ms
volatile uint8_t cmd_lost_flag = 1;       // 0=正常，1=超时

volatile uint32_t cmd_lost_imu_time = CMD_LOST_THRESHOLD;      // 单位: ms
volatile uint8_t cmd_lost_imu_flag = 1;       // 0=正常，1=超时


// 定时器初始化（TIM3，10ms中断）
void TIM3_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;            // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;         // 预分频系数
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE);
}

// TIM3中断服务函数
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        cmd_lost_time += 10; // 每10ms递增
				
        if (cmd_lost_time > CMD_LOST_THRESHOLD)
        {
            cmd_lost_flag = 1; // 超时，置位
						cmd_lost_time = CMD_LOST_THRESHOLD ;
        }
        else
        {
            cmd_lost_flag = 0; // 未超时，清零
        }
				
//				cmd_lost_imu_time +=10;
//				
//				if (cmd_lost_imu_time > CMD_IMU_LOST_THRESHOLD)
//        {
//            cmd_lost_imu_flag = 1; // 超时，置位
//						cmd_lost_imu_time = CMD_IMU_LOST_THRESHOLD ;
//        }
//        else
//        {
//            cmd_lost_imu_flag = 0; // 未超时，清零
//        }
    }
}

// 软件看门狗初始化
void Watchdog_Init(void)
{
    // 10ms定时：APB1=42MHz, 预分频4199, 自动重装99
    TIM3_Init(99, 4199);
}

//// 软件看门狗触发处理（可自定义）
//void Watchdog_Trigger(void)
//{
//    // 例如：软件复位
//    // NVIC_SystemReset();
//    // 或者点亮LED报警
//    // GPIO_SetBits(GPIOx, GPIO_Pin_x);
//} 
