#include "led.h"

u8 Seg_Display_Flag;//数码管显示切换标志位
u8 seg_date[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e}; //0-f

uint8_t SBUS_FLAG;
uint8_t MBus485_FLAG;
//初始化PA6和PA7为输出口.并使能这两个口的时钟
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOF时钟

    //GPIOA4,A5初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, & GPIO_InitStructure); //初始化
    GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5); //GPIOA4,A5设置高，灯灭
}

void FLAG_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //使能GPIOD时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_Init(GPIOD, & GPIO_InitStructure);

    SBUS_FLAG = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3);
    MBus485_FLAG = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4);
}

void DigitalTube_LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOD时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, & GPIO_InitStructure); //初始化
    GPIO_SetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
    GPIO_ResetBits(GPIOE, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);

}


void SMG_Output(u8 data)
{
    u8 i;

    for(i = 0; i < 8; i++)
    {
        CLK = 0;
        if(data & 0x80) //按位与，先发送高位
        {
            SER = 1;
        }
        else
        {
            SER = 0;
        }
        data <<= 1;

        CLK = 1; //产生一个上升沿
    }

    RCLK = 0;
    RCLK = 1;
}

void LED_Display(u32 seconds, u8 index)
{
    u8 sec[5] = {0};
    sec[4] = seconds / 1000;
    sec[3] = seconds / 100 % 10;
    sec[2] = seconds / 10 % 10;
    sec[1] = seconds % 10;



    GPIO_SetBits(GPIOE, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);
    SMG_Output(seg_date[sec[index]]);

    switch(index)
    {


    case 1:
        GPIO_ResetBits(GPIOE, GPIO_Pin_10);
        break;
    case 2:
        GPIO_ResetBits(GPIOE, GPIO_Pin_11);
        break;
    case 3:
        GPIO_ResetBits(GPIOE, GPIO_Pin_12);
        break;
    case 4:
        GPIO_ResetBits(GPIOE, GPIO_Pin_13);
        break;
    default:
        break;
    }
}

