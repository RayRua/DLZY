//gimbal J13 usart6

#include "sys.h"
#include "usart6.h"

#include "delay.h"
#include "Modbus_RTU_protocol.h"
#include "motor_config.h"

_Bool ProcessReceivedData(uint8_t* rx_buff,uint16_t cnt);
uint8_t usart6_rx_buf[RX_BUF_SIZE];
volatile uint16_t usart6_rx_cnt = 0;
void USART6_SendOneByte(u8 dat)
{
    while((USART6->SR & 0X40) == 0) {};  //循环发送,直到发送完毕
    USART6->DR = (u8) dat;
    while(USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
}
// RS485_Send函数添加重试机制
void RS485_Send(uint8_t* data, uint32_t len) {
    //GPIO_SetBits(RS485_DIR_GPIO, RS485_DIR_PIN);
    //delay_ms(1);  // 确保方向稳定

    for(uint16_t i = 0; i < len; i++) {
        USART6_SendOneByte( * data++);
    }
}
void USART6_IRQHandler(void) {
    if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
        if(usart6_rx_cnt < RX_BUF_SIZE) {
            usart6_rx_buf[usart6_rx_cnt++] = USART_ReceiveData(USART6);
						if(usart6_rx_cnt >= 64) usart6_rx_cnt = 0;  // 防止溢出
						USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        }
				
    }
		
		 if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
    {
       while(1){
					if(usart6_rx_cnt > 6) {
            if(ProcessReceivedData(usart6_rx_buf, usart6_rx_cnt)){usart6_rx_cnt = 0; break;}
						else{usart6_rx_cnt--;}
        }
				else break;
			}
					int clear=USART6->SR;
					clear=USART6->DR;	
	 }
}

_Bool ProcessReceivedData(uint8_t* rx_buff,uint16_t cnt){
		uint16_t  usTemp, i;            
		if(rx_buff[1] != 0x03)
		{
				cnt--;
				memcpy(rx_buff, &rx_buff[1], cnt);
				return 0;
		}

		if(CRC16_Check_Sum(rx_buff, rx_buff[2] + 5))
	 {
				cnt--;
				memcpy(rx_buff, &rx_buff[1], cnt);
				return 0;
		}
		usTemp = 1+1+1+rx_buff[2];//usTemp num of reg
		if(rx_buff[0]==GIMBAL_YAW)
		for(i = 0; i < usTemp; i++)
		{
				response_yaw[i] = rx_buff[i] ;
		}
		else if(rx_buff[0]==GIMBAL_PITCH)
			for(i = 0; i < usTemp; i++)
		{
				response_pitch[i] = rx_buff[i];
		}
		Modbus_Response_Register();
		return 1;
}



////////////  servo test
void Usart6_SendOneByte(u8 dat)
{
    while((USART6->SR & 0X40) == 0) {}; //????,??????
    USART6->DR = (u8) dat;
    while(USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
}

void Uart6SendStr(u8* pucStr, u8 ulNum)
{
    u8 i;
    for(i = 0; i < ulNum; i++)
    {
        Usart6_SendOneByte( * pucStr++);
    }
}
u8 USART_Set_NPosition[43] = {0xff, 0xff, 0xfe, 0x22, 0x83, 0x2a, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
u8 Clear_Duiji[8] = {0xff, 0xff, 0xfe, 0x04, 0x03, 0x28, 0x00};
void Clear_DuoJi(u8 ID_Data, u16 cmd);
void Set_N_DuoJi_Position(u16 Position_Data1, u16 Position_Time1, u16 Position_Data2, u16 Position_Time2, u16 Position_Data3, u16 Position_Time3, u16 Position_Data4, u16 Position_Time4, u16 Position_Data5, u16 Position_Time5, u16 Position_Data6, u16 Position_Time6, u16 Position_Data7, u16 Position_Time7)
{
    Clear_DuoJi(0xfe, 0x01);
    u8 Check_Count;
    u8 Check_Sum_Set1;
    u32 Check_Sum_Temp1;

    USART_Set_NPosition[0] = 0xff; //head0
    USART_Set_NPosition[1] = 0xff; //head1
    USART_Set_NPosition[2] = 0xfe; //ID
    USART_Set_NPosition[3] = 0x27; //????6???
    USART_Set_NPosition[4] = 0x83; //??
    USART_Set_NPosition[5] = 0x2A; //???????
    USART_Set_NPosition[6] = 0x04; //???????

    USART_Set_NPosition[7] = 0x01; //????????ID
    USART_Set_NPosition[8] = Position_Data1 >> 8; //??????
    USART_Set_NPosition[9] = Position_Data1 & 0x00ff; //??????
    USART_Set_NPosition[10] = Position_Time1 >> 8; //??????
    USART_Set_NPosition[11] = Position_Time1 & 0x00ff; //??????

    USART_Set_NPosition[12] = 0x02;
    USART_Set_NPosition[13] = Position_Data2 >> 8;
    USART_Set_NPosition[14] = Position_Data2 & 0x00ff;
    USART_Set_NPosition[15] = Position_Time2 >> 8;
    USART_Set_NPosition[16] = Position_Time2 & 0x00ff;

    USART_Set_NPosition[17] = 0x03;
    USART_Set_NPosition[18] = Position_Data3 >> 8;
    USART_Set_NPosition[19] = Position_Data3 & 0x00ff;
    USART_Set_NPosition[20] = Position_Time3 >> 8;
    USART_Set_NPosition[21] = Position_Time3 & 0x00ff;

    USART_Set_NPosition[22] = 0x04;
    USART_Set_NPosition[23] = Position_Data4 >> 8;
    USART_Set_NPosition[24] = Position_Data4 & 0x00ff;
    USART_Set_NPosition[25] = Position_Time4 >> 8;
    USART_Set_NPosition[26] = Position_Time4 & 0x00ff;

    USART_Set_NPosition[27] = 0x05;
    USART_Set_NPosition[28] = Position_Data5 >> 8;
    USART_Set_NPosition[29] = Position_Data5 & 0x00ff;
    USART_Set_NPosition[30] = Position_Time5 >> 8;
    USART_Set_NPosition[31] = Position_Time5 & 0x00ff;

    USART_Set_NPosition[32] = 0x06;
    USART_Set_NPosition[33] = Position_Data6 >> 8;
    USART_Set_NPosition[34] = Position_Data6 & 0x00ff;
    USART_Set_NPosition[35] = Position_Time6 >> 8;
    USART_Set_NPosition[36] = Position_Time6 & 0x00ff;

    USART_Set_NPosition[37] = 0x07;
    USART_Set_NPosition[38] = Position_Data7 >> 8;
    USART_Set_NPosition[39] = Position_Data7 & 0x00ff;
    USART_Set_NPosition[40] = Position_Time7 >> 8;
    USART_Set_NPosition[41] = Position_Time7 & 0x00ff;

    Check_Sum_Temp1 = 0;
    for(Check_Count = 2; Check_Count <= 41; Check_Count++)
    {
        Check_Sum_Temp1 += USART_Set_NPosition[Check_Count];
    }

    Check_Sum_Temp1 = ~Check_Sum_Temp1;
    Check_Sum_Set1 = (u8)Check_Sum_Temp1;
    USART_Set_NPosition[42] = Check_Sum_Set1;

    Uart6SendStr(USART_Set_NPosition, 43);
}

void Clear_DuoJi(u8 ID_Data, u16 cmd)
{
    u8 Check_Sum_ID;

    Clear_Duiji[0] = 0xff;
    Clear_Duiji[1] = 0xff;
    Clear_Duiji[2] = ID_Data;
    Clear_Duiji[3] = 0x04;
    Clear_Duiji[4] = 0x03;
    Clear_Duiji[5] = 0x28;
    Clear_Duiji[6] = cmd;

    Check_Sum_ID = ~(Clear_Duiji[2] + Clear_Duiji[3] + Clear_Duiji[4] + Clear_Duiji[5] + Clear_Duiji[6]);
    Clear_Duiji[7] = Check_Sum_ID;

    Uart6SendStr(Clear_Duiji, 8);
}





