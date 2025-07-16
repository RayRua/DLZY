#include "bsp_init.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"
#include "uart5.h"
#include "usart6.h"
#include "led.h"
#include "can.h"
#include "communicate.h"
extern uint8_t MBus485_FLAG;
// 不带中断的USART1初始化函数
void USART1_Init_NoIRQ(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

// 不带中断的USART2初始化函数
// void USART2_Init_NoIRQ(u32 bound)
//{
//    GPIO_InitTypeDef gpio_initstructure;
//    USART_InitTypeDef usart_initstructure;

//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//    USART_DeInit(USART2);

//    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
//    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

//    gpio_initstructure.GPIO_Pin = GPIO_Pin_5;
//    gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
//    gpio_initstructure.GPIO_OType = GPIO_OType_PP;
//    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
//    gpio_initstructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_Init(GPIOD, &gpio_initstructure);

//    gpio_initstructure.GPIO_Pin = GPIO_Pin_6;
//    gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
//    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
//    gpio_initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOD, &gpio_initstructure);

//    usart_initstructure.USART_BaudRate = bound;
//    usart_initstructure.USART_WordLength = USART_WordLength_8b;
//    usart_initstructure.USART_StopBits = USART_StopBits_1;
//    usart_initstructure.USART_Parity = USART_Parity_No;
//    usart_initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    usart_initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    USART_Init(USART2, &usart_initstructure);
//    USART_Cmd(USART2, ENABLE);
//}

// 不带中断的USART3初始化函数
void USART3_Init_NoIRQ(u32 bound)
{
    GPIO_InitTypeDef gpio_initstructure;
    USART_InitTypeDef usart_initstructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    USART_DeInit(USART3);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

    gpio_initstructure.GPIO_Pin = GPIO_Pin_8;
    gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstructure.GPIO_OType = GPIO_OType_PP;
    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_initstructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &gpio_initstructure);

    gpio_initstructure.GPIO_Pin = GPIO_Pin_9;
    gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &gpio_initstructure);

    usart_initstructure.USART_BaudRate = bound;
    usart_initstructure.USART_WordLength = USART_WordLength_8b;
    usart_initstructure.USART_StopBits = USART_StopBits_1;
    usart_initstructure.USART_Parity = USART_Parity_No;
    usart_initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &usart_initstructure);
    USART_Cmd(USART3, ENABLE);
}

// 不带中断的UART4初始化函数
void UART4_Init_NoIRQ(u32 bound)
{
    GPIO_InitTypeDef gpio_initstructure;
    USART_InitTypeDef usart_initstructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    USART_DeInit(UART4);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

    gpio_initstructure.GPIO_Pin = GPIO_Pin_10;
    gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstructure.GPIO_OType = GPIO_OType_PP;
    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_initstructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gpio_initstructure);

    gpio_initstructure.GPIO_Pin = GPIO_Pin_11;
    gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &gpio_initstructure);

    usart_initstructure.USART_BaudRate = bound;
    usart_initstructure.USART_WordLength = USART_WordLength_8b;
    usart_initstructure.USART_StopBits = USART_StopBits_1;
    usart_initstructure.USART_Parity = USART_Parity_No;
    usart_initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART4, &usart_initstructure);
    USART_Cmd(UART4, ENABLE);
}

// 不带中断的UART5初始化函数
void UART5_Init_NoIRQ(void)
{
    GPIO_InitTypeDef gpio_initstructure;
    USART_InitTypeDef usart_initstructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 使能GPIOC时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // 使能GPIOD时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); // 使能UART5时钟
    USART_DeInit(UART5);                                  // 复位串口5

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

    /* C12 USART5_Tx */
    gpio_initstructure.GPIO_Pin = GPIO_Pin_12;
    gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstructure.GPIO_OType = GPIO_OType_PP;
    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_initstructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gpio_initstructure);

    /* D2 USART5_Rx  */
    gpio_initstructure.GPIO_Pin = GPIO_Pin_2;
    gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &gpio_initstructure);

    if (MBus485_FLAG == 0)
    {
        // 工业485遥控器读取指令 波特率：9600，8，N,1 站号：5
        usart_initstructure.USART_BaudRate = 9600;                                      // 波特率
        usart_initstructure.USART_WordLength = USART_WordLength_8b;                     // 8位数据位
        usart_initstructure.USART_StopBits = USART_StopBits_1;                          // 1个停止位
        usart_initstructure.USART_Parity = USART_Parity_No;                             // 无校验
        usart_initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
        usart_initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    }
    else
    {
        // 波特率100000  9 0 E 2  M.BUS协议
        usart_initstructure.USART_BaudRate = 100000;                                    // 波特率100000
        usart_initstructure.USART_WordLength = USART_WordLength_9b;                     // 9位数据位
        usart_initstructure.USART_StopBits = USART_StopBits_2;                          // 2个停止位
        usart_initstructure.USART_Parity = USART_Parity_Even;                           // 偶校验
        usart_initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
        usart_initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    }

    USART_Init(UART5, &usart_initstructure);
    USART_Cmd(UART5, ENABLE);
    // 注意：不配置中断，中断配置在bsp_enable_irq()中进行
}

// 不带中断的USART6初始化函数
void USART6_Init_NoIRQ(u32 bound)
{
    GPIO_InitTypeDef gpio_initstructure;
    USART_InitTypeDef usart_initstructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
    USART_DeInit(USART6);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

    gpio_initstructure.GPIO_Pin = GPIO_Pin_6;
    gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstructure.GPIO_OType = GPIO_OType_PP;
    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_initstructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gpio_initstructure);

    gpio_initstructure.GPIO_Pin = GPIO_Pin_7;
    gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &gpio_initstructure);

    usart_initstructure.USART_BaudRate = bound;
    usart_initstructure.USART_WordLength = USART_WordLength_8b;
    usart_initstructure.USART_StopBits = USART_StopBits_1;
    usart_initstructure.USART_Parity = USART_Parity_No;
    usart_initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART6, &usart_initstructure);
    USART_Cmd(USART6, ENABLE);
}

// 不带中断的CAN初始化函数
void CanConfiguartion_NoIRQ(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
    CAN_InitStructure.CAN_Prescaler = 6;
    CAN_Init(CAN1, &CAN_InitStructure);
}

// 外设初始化（不使能中断）
void bsp_init_no_irq()
{
    delay_init(168); // 初始化延时函数
    LED_Init();      // 初始化LED显示
    FLAG_Init();
    DigitalTube_LED_Init();
    USART1_Init_NoIRQ(115200); // 串口，波特率为115200，用于pc通信
    CanConfiguartion_NoIRQ();  // CAN口，发0x1315，收0x1316，用于pc通信
    //  USART2_Init_NoIRQ(115200);                                                      //串口中断，接收8路遥控器
    USART3_Init_NoIRQ(115200); // 串口中断，接收遥控器数据
    UART4_Init_NoIRQ(115200);  // 读取6路编码器数值，编码器数值
    UART5_Init_NoIRQ();        // 遥控接收初始化 12引脚M.BUS可遥控器 34引脚上电485工业遥控器
    USART6_Init_NoIRQ(115200);
    LED_Display(1, 1);
    LED0 = 0;
    LED1 = 1;
}

// 使能外设中断
void bsp_enable_irq()
{
    // 配置中断优先级（确保低于FreeRTOS可管理的最高优先级）
    NVIC_InitTypeDef NVIC_InitStructure;

    // USART1中断配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // USART2中断配置
    //    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    //    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    //    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    //    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //    NVIC_Init(&NVIC_InitStructure);

    // USART3中断配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // UART4中断配置
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // UART5中断配置
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // USART6中断配置
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // CAN中断配置
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能外设中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART6, USART_IT_IDLE, ENABLE); // 添加USART6 IDLE中断使能
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

// 保持原有函数名以兼容现有代码
void bsp_init()
{
    bsp_init_no_irq();
    bsp_enable_irq();
}
