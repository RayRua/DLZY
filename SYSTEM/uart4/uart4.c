//J4 uart4

#include "sys.h"
#include "uart4.h"
#include "string.h"
#include "wit_c_sdk.h"


u8  MagneticEncoder_Se_Date[8] = {0X01, 0X03, 0X00, 0X00, 0X00, 0X01, 0X84, 0X0A}; //主机发送该指令，等待编码器单圈值：编码器当前角度=编码器单圈值*360/单圈分辨率。例如读取编码器单圈值为1000，单圈分辨率为1024(即 10bit，2^10=1024),编码器当前角度=1000*360/1024=351.5625°
u8  MagneticEncoder_Se_Code[8] = {0X01, 0X06, 0X00, 0X0E, 0X00, 0X01, 0X29, 0XC9}; //主机发送该指令，等待编码器返回参数；设定中点180°

u16 Position1 = 8192;
u16 Position2 = 8192;
u16 Position3 = 512;
u16 Position4 = 512;
u16 Position5 = 512;
u16 Position6 = 512;
u16 Position7 = 512;
u16 Position8 = 512;
u16 Steer_z = 512;
u8 MagneticEncoder_ID_Number;  //AS5600磁编码器ID寄存器
u8 step = 0; //上电执行步骤
u8 AS5600_Time_Flag = 0; //开始计时标志位
u16 AS5600_500ms_cnt = 0; //调中指令超时计数器

extern u16 time_100ms_flag;            //100ms定时时间标志位
extern u16 x_speed;
extern u16 y_speed;
extern u16 z_speed;
extern u16 k_number;
extern u8 Remote_Re_Date[25];

u8 ID_HEAD = 0;
u8 uart4_Buf[8];
u8 uart4_Count;
u8 rx4Re_Flag = 0;

u8 IRQ4_FLAG;
/*uart4*/
/**********************************************
串口配置函数   编码器协议
配置了串口4，PC10 PC11
波特率：115200,
数据位：8b，
停止位：1，
校验：N
接收中断使能*/
//**********************************************/



void UART4_SendOneByte(u8 dat)
{
    while((UART4->SR & 0X40) == 0) {};  //循环发送,直到发送完毕
    UART4->DR = (u8) dat;
    while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
}

void Uart4_Send(uint8_t* data, uint32_t len) {
  

    for(uint16_t i = 0; i < len; i++) {
        UART4_SendOneByte( * data++);
    }

}

void UART4_IRQHandler(void)
{
		unsigned char ucTemp;
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) {
            ucTemp = USART_ReceiveData(UART4);
						WitSerialDataIn(ucTemp);//接收到的数据传入WitSerialDataIn函数中
						USART_ClearITPendingBit(UART4, USART_IT_RXNE);
    }
		
}



