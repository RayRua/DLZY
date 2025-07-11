#include "can.h"
#include "string.h"
#include "communicate.h"

u16 can1buf[CAN1_RXSIZE];
u16 can1head;
u16 can1tail;
COMMUNICATE can1;

void CanConfiguartion(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_Init(GPIOD, & GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

    CAN_InitTypeDef CAN_InitStruct;
    CAN_InitStruct.CAN_TTCM = DISABLE;//非时间触发通信
    CAN_InitStruct.CAN_ABOM = DISABLE;//软件自动离线管理
    CAN_InitStruct.CAN_AWUM = DISABLE;//睡眠模式通过软件唤醒
    CAN_InitStruct.CAN_NART = ENABLE;//禁止报文自动传送
    CAN_InitStruct.CAN_RFLM = DISABLE;//报文不锁定，新的覆盖旧的
    CAN_InitStruct.CAN_TXFP = DISABLE;//优先级由报文表示决定
    CAN_InitStruct.CAN_Mode = CAN_Mode_Normal;//正常模式
    CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStruct.CAN_BS1 = CAN_BS1_7tq;
    CAN_InitStruct.CAN_BS2 = CAN_BS2_6tq;
//  CAN_InitStruct.CAN_Prescaler = 24;//168MHz 125Khz
//  CAN_InitStruct.CAN_Prescaler = 12;//168MHz 250KKhz
    CAN_InitStruct.CAN_Prescaler = 6;//168MHz 500Khz
//  CAN_InitStruct.CAN_Prescaler = 3;//168MHz 1Mhz

    CAN_FilterInitTypeDef CAN_FilterInitStruct;
    CAN_FilterInitStruct.CAN_FilterNumber = 0;//使用过滤器0
    CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStruct.CAN_FilterIdHigh = ((uint32_t)0x1315 << 3) >> 16;
    CAN_FilterInitStruct.CAN_FilterIdLow = (((uint32_t)0x1315 << 3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF;
    CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xFFFF;
    CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0xFFFF;
    CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    CAN_Init(CAN1, & CAN_InitStruct);
    CAN_FilterInit( & CAN_FilterInitStruct);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init( & NVIC_InitStruct);
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void CAN1_Send_Msg(uint8_t* pdata, uint8_t Len)
{
    CanTxMsg TxMessage;
    TxMessage.ExtId = 0x1316;//扩展ID
    TxMessage.IDE = CAN_Id_Extended;
    TxMessage.RTR = CAN_RTR_Data;
    TxMessage.DLC = Len;
    memcpy(TxMessage.Data, pdata, Len);
    while(1)
    {
        uint16_t i = 0;
        uint8_t Statue = CAN_TxStatus_NoMailBox;
        while(Statue == CAN_TxStatus_NoMailBox)
        {
            Statue = CAN_Transmit(CAN1, & TxMessage);
            i ++;
            if(i > 0xF)
                break;
        }
        if(Statue == CAN_TxStatus_NoMailBox)
            break;
        i = 0;
        while(CAN_TransmitStatus(CAN1, Statue) != CAN_TxStatus_Ok)
        {
            i ++;
            if(i > 0xF)
                break;
        }
        break;
    }
}

void CAN1_Send(uint8_t* pdata, uint8_t Len)
{
    for(uint8_t i = 0; i < Len >> 3; i ++)
    {
        CAN1_Send_Msg( & pdata[i << 3], 8);
    }
    if(Len & 0x07)
        CAN1_Send_Msg( & pdata[Len & 0xF8], Len & 0x07);
}

uint8_t can1GetByte(uint8_t* pChar)
{
    uint8_t tmptail;
    if(can1head != can1tail)
    {
        tmptail = can1tail + 1;
        tmptail %= CAN1_RXSIZE;
        can1tail = tmptail;
        tmptail = can1buf[tmptail];
        * pChar = tmptail;
        return 1;
    }
    return 0;
}

void CAN1_RX0_IRQHandler(void)
{
    //共有3种事件产生中断
    //1.接收到新消息 FMP0不是00
    //2.FIFO0满 RF0R的FULL置1
    //3.FIFO0上溢 RF0R的FOVR0置1
    CanRxMsg RxMessage;
    while(CAN_MessagePending(CAN1, CAN_FIFO0))
    {
        uint16_t tmphead;
        CAN_Receive(CAN1, CAN_FIFO0, & RxMessage);
        for(uint8_t i = 0; i < RxMessage.DLC; i ++)
        {
            tmphead = can1head + 1;
            tmphead %= CAN1_RXSIZE;
            can1head = tmphead;
            can1buf[can1head] = RxMessage.Data[i];
        }
    }
    //CAN_ClearITPendingBit();
}


