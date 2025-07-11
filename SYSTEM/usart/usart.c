//master DB1 usart1

#include "sys.h"
#include "usart.h"
#include "can.h"
#include "usart3.h"
#include "uart4.h"
#include "uart5.h"
#include "string.h"
#include "communicate.h"

unsigned char rx1buf[UART1_RXSIZE];
unsigned char rx1head;
unsigned char rx1tail;
unsigned char tx1buf[UART1_TXSIZE];
unsigned char tx1head;
unsigned char tx1tail;

COMMUNICATE uart1;

//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,需要添加ucos的头文件.
#if SYSTEM_SUPPORT_OS
#include "includes.h"                   //ucos 使用
#endif



//////////////////////////////////////////////////////////////////
//如果使用printf,需要选定use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//默认需要实现_sys_exit()函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//默认_sys_exit()函数需要返回值
void _sys_exit(int x)
{
    x = x;
}

//重定义fputc函数
int fputc(int ch, FILE* f)
{
    while((USART1->SR & 0X40) == 0);  //循环等待,直到发送完成

    USART1->DR = (u8) ch;
    return ch;
}

#endif

#if EN_USART1_RX   //使用串口1
//串口1接收中断处理函数
//注意,读取USARTx->SR寄存器会清除相应的标志位
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,大小为USART_REC_LEN字节.
//接收状态
//bit15   接收完成标志
//bit14   接收到的帧错误
//bit13~0 接收到的有效字节数目
u16 USART_RX_STA = 0;     //接收状态标志

//初始化IO 串口1
//bound:波特率




void USART1_SendOneByte(u8 dat)
{

    while((USART1->SR & 0X40) == 0) {};  //循环等待,直到发送完成

    USART1->DR = (u8) dat;

    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void USART1_Send(uint8_t* pdata, uint8_t Len)
{
    u8 i;

    for(i = 0; i < Len; i++)
    {
        USART1_SendOneByte(pdata[i]);
    }
}

u8 usart1GetByte(u8* pChar)
{
    unsigned char tmptail;

    if(rx1head != rx1tail)
    {
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); //禁止接收 读取数据
        tmptail = (rx1tail + 1) ;
        tmptail %= UART1_RXSIZE ;       /* calculate buffer index */
        rx1tail = tmptail;      /* store new index */
        * pChar = rx1buf[tmptail];
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
                          /* return data */
        return 1;
    }

    return 0;
}

u8 x1;
void USART1_IRQHandler(void)                    //串口1接收中断处理函数
{

    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        u8 tmphead;
        u8 data;
        data = USART_ReceiveData(USART1);               /* read the received data */
        tmphead = (rx1head + 1) ;                       /* calculate buffer index */
        tmphead %= UART1_RXSIZE;
        rx1head = tmphead;                      /* store new index */

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);

        rx1buf[tmphead] = data;
    }

    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)           // 接收完成
    {
        USART_ReceiveData(USART1);                                                              // 清除接收完成标志
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }

}

#endif




