//remote CN1 uart5

#include "sys.h"
#include "uart5.h"
#include "usart.h"
#include "led.h"
#include "string.h"
#include "gimbal_task.h"
/*uart5*/
u8 Remote_Re_Date[25];//遥控器接受缓存区
u8 Remote_Re_Count;  //遥控器接受计数
u8 Remote_Re_FLAG;       //遥控器接受完成标志位
u16 MBUS_CH[17];            //遥控器16个通道的实时数值，实际使用只用了6个通道
u16 x_speed = 0;
u16 y_speed = 0;
u16 z_speed = 0;
u16 k_number = 0;
u16 key5, key6; //遥控器的第5、6通道，
u8 MBUS_ON_OFF=0x10;//遥控器开关机状态
uint8_t last_MBUS_ON_OFF=0;
u8 Mbus_Time_Count_Flag = 0; //MUBS是否连接标志位
u16 Mbus_Time_500ms_cnt = 0; //调中指令超时计数器

u8 commandBytes[8] = {0X05, 0X03, 0X00, 0X00, 0X00, 0X04, 0X45, 0X8D}; //主机发送该指令
u16 pitch = 0;
u16 yaw = 0;
int Communication_Status; //遥控开关机状态
int start_but;//启动按钮
int EMERGENCY_STOP = 0;//急停
int reset_but;//复位
int up_but;//上
int down_but;//下
int on1_but;//开关
int on2_but;//开关

extern uint8_t SBUS_FLAG;
extern uint8_t MBus485_FLAG;

 _Bool remote_first_init=1;//when open remote,and restart MCU the same with gimbal_task.h

/*****16位CRC检验表，低位在前，高位在后*******/
//////////////*高位表*///////////////////
static const u8 auchCRCHi[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, //
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40
};
//////////////*低位表*///////////////////
static const u8 auchCRCLo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
};
////////*16位CRC校验函数查表法*////////////////
static u16 crc16(u8* puchMsg, u8 usDataLen)
{
    u8 uchCRCHi = 0xFF ;
    u8 uchCRCLo = 0xFF ;
    u16 uIndex ;
    while(usDataLen--)
    {
        uIndex = uchCRCHi ^ * puchMsg++ ;
        uchCRCHi = uchCRCLo^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
    }
    return (((u8)(uchCRCLo) << 8) | uchCRCHi) ;
}

void UART5_SendOneByte(u8 dat)
{
    while((UART5->SR & 0X40) == 0) {};  //循环发送,直到发送完毕
    UART5->DR = (u8) dat;
    while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
}

void Sent_Modbus_RTU(void)//发送485遥控器读取指令 波特率：9600，8，N,1 从站号：5
{
    u8 i;
    for(i = 0; i < 8; i++)
    {
        UART5_SendOneByte(commandBytes[i]);
    }
}

void UART5_IRQHandler(void)                 //串口5中断服务程序
{

    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)            // 空闲中断
    {
        Mbus_Time_Count_Flag = 1;
        Remote_Re_Date[Remote_Re_Count] = USART_ReceiveData(UART5);
        Remote_Re_Count++;
        if(MBus485_FLAG == 0) {
            if(Remote_Re_Date[0] == 0x05)
            {
                if(Remote_Re_Count > 12)
                {
                    int receivedCRC = (Remote_Re_Date[12] << 8) | Remote_Re_Date[11];
                    int expectedCRC = crc16(Remote_Re_Date, 11);
                    if(expectedCRC == receivedCRC) {
                        Remote_Re_Count = 0;
                        Remote_Re_FLAG = 1;
                    } else {
                        Remote_Re_Count = 0;
                        Remote_Re_FLAG = 0;
                    }
                }
            }
            else
            {
                Remote_Re_Count = 0;
            }
        } 
				else {
            if(Remote_Re_Date[0] == 0x0F)
            {
                if(Remote_Re_Count > 24)
                {
                    Remote_Re_Count = 0;
                    Remote_Re_FLAG = 1;
                }
            }
            else
            {
                Remote_Re_Count = 0;
            }
        }
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }
    if(USART_GetITStatus(UART5, USART_IT_TXE) == SET)
    {
					Get_Remote_channel_Date();
        	int clear=USART6->SR;
							clear=USART6->DR;
    }
}

/*获取遥控器各通道实时数据*/
void Get_Remote_channel_Date(void)
{
    if(Mbus_Time_Count_Flag == 1)  //表示已经接入遥控器,如果没有接入，就不进入中断。或者长时间没进入中断，表示断线。
    {
        if(Remote_Re_FLAG == 1)  //接收到完整的一帧数据
        {
            if(MBus485_FLAG == 0) { //定制工业遥控器 Modbus-RTU:RS485
                start_but = (((int)(Remote_Re_Date[6])  >> 7)) & 0x01;
                Communication_Status = (((int)(Remote_Re_Date[4])  >> 7)) & 0x01;
                EMERGENCY_STOP = ((int)(Remote_Re_Date[3]) >> 0) & 0x01;
                reset_but = ((int)(Remote_Re_Date[3]) >> 1) & 0x01;
                up_but = ((int)(Remote_Re_Date[3]) >> 2) & 0x01;
                down_but = ((int)(Remote_Re_Date[3]) >> 3) & 0x01;
                on1_but = ((int)(Remote_Re_Date[3]) >> 4) & 0x01;
                on2_but = ((int)(Remote_Re_Date[3]) >> 5) & 0x01;

                if(Communication_Status) {
                    if(EMERGENCY_STOP) { //急停按下
                        MBUS_ON_OFF = 0x80;
                        x_speed = 128;
                        y_speed = 128;
                        z_speed = 128;
                        k_number = 0;
                    } else {
                        MBUS_ON_OFF = 0x00;
                        pitch = Remote_Re_Date[5];// * 7.8125;
                        yaw = Remote_Re_Date[8];// * 7.8125;
                        x_speed = Remote_Re_Date[7];// * 7.8125;
                        y_speed = yaw;
                        if(x_speed < 128) {
                            z_speed = (Remote_Re_Date[10]);// * 7.8125;
                        } else {
                            z_speed = (256 - Remote_Re_Date[10]);//* 7.8125;
                        }

                        k_number = 128;
                    }
                } else {
                    MBUS_ON_OFF = 0x10;
                    x_speed = 128;
                    y_speed = 128;
                    z_speed = 128;
                    k_number = 0;
                }
            } else { //S.BUS 6通道遥控器
                switch(Remote_Re_Date[23])  //判断是否关机
                {
                /*开机状态*/
                case 0x00:
                    if(Remote_Re_Date[0] == 0x0f)  //表示
                    {
                        MBUS_CH[1] = ((int16_t)(Remote_Re_Date[1]) >> 0 | ((int16_t)(Remote_Re_Date[2]) << 8)) & 0x07FF;
                        MBUS_CH[2] = ((int16_t)(Remote_Re_Date[2]) >> 3 | ((int16_t)(Remote_Re_Date[3]) << 5)) & 0x07FF;
                        MBUS_CH[3] = ((int16_t)(Remote_Re_Date[3]) >> 6 | ((int16_t)(Remote_Re_Date[4]) << 2) | (int16_t)(Remote_Re_Date[5] << 10)) & 0x07FF;
                        MBUS_CH[4] = ((int16_t)(Remote_Re_Date[5]) >> 1 | ((int16_t)(Remote_Re_Date[6]) << 7)) & 0x07FF;
                        MBUS_CH[5] = ((int16_t)(Remote_Re_Date[6]) >> 4 | ((int16_t)(Remote_Re_Date[7]) << 4)) & 0x07FF;
                        MBUS_CH[6] = ((int16_t)(Remote_Re_Date[7]) >> 7 | ((int16_t)(Remote_Re_Date[8]) << 1) | (int16_t)(Remote_Re_Date[9] << 9)) & 0x07FF;
                        MBUS_CH[7] = ((int16_t)(Remote_Re_Date[9]) >> 2 | ((int16_t)(Remote_Re_Date[10]) << 6)) & 0x07FF;
                        MBUS_CH[8] = ((int16_t)(Remote_Re_Date[10]) >> 5 | ((int16_t)(Remote_Re_Date[11]) << 3)) & 0x07FF;
                        MBUS_CH[9] = ((int16_t)(Remote_Re_Date[12]) >> 0 | ((int16_t)(Remote_Re_Date[13]) << 8)) & 0x07FF;
                        MBUS_CH[10] = ((int16_t)(Remote_Re_Date[13]) >> 3 | ((int16_t)(Remote_Re_Date[14]) << 5)) & 0x07FF;
                        MBUS_CH[11] = ((int16_t)(Remote_Re_Date[14]) >> 6 | ((int16_t)(Remote_Re_Date[15]) << 2) | (int16_t)(Remote_Re_Date[16] << 10)) & 0x07FF;
                        MBUS_CH[12] = ((int16_t)(Remote_Re_Date[16]) >> 1 | ((int16_t)(Remote_Re_Date[17]) << 7)) & 0x07FF;
                        MBUS_CH[13] = ((int16_t)(Remote_Re_Date[17]) >> 4 | ((int16_t)(Remote_Re_Date[18]) << 4)) & 0x07FF;
                        MBUS_CH[14] = ((int16_t)(Remote_Re_Date[18]) >> 7 | ((int16_t)(Remote_Re_Date[19]) << 1) | (int16_t)(Remote_Re_Date[20] << 9)) & 0x07FF;
                        MBUS_CH[15] = ((int16_t)(Remote_Re_Date[20]) >> 2 | ((int16_t)(Remote_Re_Date[21]) << 6)) & 0x07FF;
                        MBUS_CH[16] = ((int16_t)(Remote_Re_Date[21]) >> 5 | ((int16_t)(Remote_Re_Date[22]) << 3)) & 0x07FF;
                        z_speed = MBUS_CH[1];
                        x_speed = MBUS_CH[2];
                        y_speed = MBUS_CH[4];
                        k_number = MBUS_CH[3];

                        key5 = MBUS_CH[5];  //三个挡位，第五通道
                        key6 = MBUS_CH[6];  //两个挡位，第六通道

                        MBUS_ON_OFF = Remote_Re_Date[23];
                        Remote_Re_Date[0] = 1;
                        Remote_Re_FLAG = 0;
                        Remote_Re_Date[0] = 0x00;
                        Remote_Re_Date[1] = 0x00;
                        Remote_Re_Date[2] = 0x00;
                        Remote_Re_Date[3] = 0x00;
                    }
                    else if((Remote_Re_Date[0] == 0x00) && (Remote_Re_Date[1] == 0x00))
                    {
                        z_speed = 1000;
                        x_speed = 1000;
                        y_speed = 1000;
                        k_number = 0; //可以以此来判断遥控器是否关机
                    }
										if(remote_first_init&&last_MBUS_ON_OFF==0x00&&MBUS_ON_OFF==0x00){flag_remote_start=1;flag_remote_shut=1;remote_first_init=0;}
										if(last_MBUS_ON_OFF==0x10&&MBUS_ON_OFF==0x00){ 
										flag_remote_start=1;
										last_MBUS_ON_OFF=MBUS_ON_OFF;
								}
                    break;
                /*关机状态*/
                case 0x10:
								case 0xf4:
                    MBUS_ON_OFF = 0x10;
										if(last_MBUS_ON_OFF==0x00&&MBUS_ON_OFF==0x10){
										flag_remote_shut=1;
										last_MBUS_ON_OFF=MBUS_ON_OFF;
								}
                    z_speed = 0;
                    x_speed = 0;
                    y_speed = 0;
                    k_number = MBUS_CH[3]; //因为K值不是自动归中，所以当遥控器关机时，
                    //不应该置为中值，应该是遥控器关机前的最后的状态值。

                    key5 = MBUS_CH[5];  //三个挡位，第五通道
                    key6 = MBUS_CH[6];  //两个挡位，第六通道
                    break;
                default:
                    break;
                }
            }

        }
    }
    else   //被断开了
    {
        Remote_Re_Date[23] = 0x10; //当线断开则强制关机模式
        MBUS_ON_OFF = Remote_Re_Date[23];
        if(MBus485_FLAG == 0) {
            x_speed = 128;
            y_speed = 128;
            z_speed = 128;
        }
        else {
            x_speed = 0;
            y_speed = 0;
            z_speed = 0;
        }
        k_number = 0; //可以以此来判断遥控器是否关机
        EMERGENCY_STOP = 0;
    }
}
