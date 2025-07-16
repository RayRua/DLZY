#include "bsp_init.h"

#include "delay.h"

#include "motor_config.h"
#include "ins_task.h"

#include "cmsis_os.h"
#include "init_task.h"
#include "watchdog.h"

#include "pin.h"

u16 Mbus_Time_OFF_cnt;
u16 Mbus_Time_OFF_flag;
extern u8 IRQ4_FLAG, F;
u8 uart4flag;
u16 PC_ReTime_cnt;
u8 PC_ReTime_stop_flag = 1;
u16 err;
u8 LED_1s_flag = 1;
u16 LED_time_1s_cnt = 0;
u16 err_time_1s_cnt = 0;
u32 Time_cnt = 0;
u16 err_LED = 0000;
u8 LED_index = 1;
volatile uint32_t TimingDelay = 0; // 全局计时变量，volatile 防止优化

// 全局标志，用于标记中断是否已使能
volatile u8 irq_enabled = 0;

/*重要信息：3个分别对应ID：9*/

void GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

int main(void)
{
	// 1. 首先配置中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置系统中断优先级分组为2

	// 2. 初始化基础外设（不使能中断）
	bsp_init_no_irq();

	// 3.
	motor_init();

	GPIO_init();
	// ins_init();

	Watchdog_Init();
	// 4. 配置外设中断（在启动内核前配置好）
	bsp_enable_irq();

	// 5. 创建任务
	init_task();
	delay_ms(10);

	// 初始化PD4和PD5为输入
	//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//		pin_init(GPIOD, GPIO_Pin_4, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL);
	//		pin_init(GPIOD, GPIO_Pin_5, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL);

	// 可在此处使用pd4_level和pd5_level变量

	// 6. 启动调度器（会自动开启全局中断）
	osKernelStart();

	while (1)
	{
	}
}
