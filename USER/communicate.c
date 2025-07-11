#include "communicate.h"
#include "can.h"
#include "usart.h"
#include "usart3.h"
#include <string.h>

extern u16  cmd_lost_time;

extern u8 MBUS_ON_OFF;
extern ReturnedWheel ReturnedWheelData[9];
extern float liner_x_speed, anglespeed;
extern u16 Position1 ;
extern u16 Position2 ;
extern u16 Position3 ;
extern u16 Position4 ;
extern u16 Position5 ;
extern u16 Position6 ;
extern u16 Position7 ;
extern u16 Position8 ;

u8 can_send_flag;

COMMUNICATE* pSelect;

//ͨ�ų�ʼ������Ҫ�ڳ�ʼ��ʱ����
void CommunicateInit(COMMUNICATE* p)
{
    p->rx_wr_index = 0;
    p->rx_wr_size = 0;
    p->checksum = 0;
    // Device header
    memset(p->tx_buf, 0, sizeof(p->tx_buf));
    memset(p->rx_buf, 0, sizeof(p->rx_buf));
    //memset(p->tmp_buf, 0, sizeof(p->tmp_buf));
}

//У����У��
unsigned char sumCheck(COMMUNICATE* p)
{
    unsigned char i;
    p->checksum = 0;
    for(i = 0; i < p->rx_buf[2] + 3; i++)
        p->checksum ^= p->rx_buf[i];
    if(p->checksum == p->rx_buf[p->rx_wr_index - 1])
        return  1;
    else
        return  0;
}
u8 DecodeFrame1By1(unsigned char newdata, COMMUNICATE* p)
{
    if(p->rx_wr_index == 0)  // ??????????????
    {
        if(newdata == 0xAA)  // ???????? 0xAA
        {
            p->rx_buf[p->rx_wr_index++] = newdata;  // ?????
        }
        else
        {
            return 0;  // ???????,?? 0
        }
    }
    else
    {
        p->rx_buf[p->rx_wr_index++] = newdata;  // ????????

        if(p->rx_wr_index == 15)  // ????? 19 ???
        {
            if(sumCheck(p))  // ????
            {
                p->rx_wr_index = 0;  // ????,????
								gimbal_protocol(p);
                return 1;  // ?? 1 ????
            }
            else
            {
                // ????,?????????
                p->rx_wr_index = 0;
            }
        }
    }
    return 0;  // ??????? 19 ????????,?? 0
}
void ImformationCopy(COMMUNICATE* p)
{
    u8 i;
    p->tx_buf[0] = 0XAA;
    p->tx_buf[1] = 0X55;
    p->tx_buf[2] = 0X3e;
    //���ӱ�����λ��  ��λǰ����λ��
    memcpy(p->tx_buf + 3, & ReturnedWheelData[1].ReturnedWheelHallCnt, 2);
    memcpy(p->tx_buf + 5, & ReturnedWheelData[2].ReturnedWheelHallCnt, 2);
    memcpy(p->tx_buf + 7, & ReturnedWheelData[3].ReturnedWheelHallCnt, 2);
    memcpy(p->tx_buf + 9, & ReturnedWheelData[4].ReturnedWheelHallCnt, 2);
    memcpy(p->tx_buf + 11, & ReturnedWheelData[5].ReturnedWheelHallCnt, 2);
    memcpy(p->tx_buf + 13, & ReturnedWheelData[6].ReturnedWheelHallCnt, 2);
    //0     ����            8   �¶ȱ���
    //2     ��������        9   ����ƫ�ù���
    //3     ת�ѹ���        10  ��ѹƫ�ù���
    //5     ����������      11  ĸ�ߵ���ƫ�ù���
    //6     ��ѹ����        12  ��ת����
    //7     Ƿѹ����        13  UART����
    memcpy(p->tx_buf + 15, & ReturnedWheelData[1].ReturnedWheelErrorCode, 1);
    memcpy(p->tx_buf + 16, & ReturnedWheelData[2].ReturnedWheelErrorCode, 1);
    memcpy(p->tx_buf + 17, & ReturnedWheelData[3].ReturnedWheelErrorCode, 1);
    //ʵʱת��Ƕ�,���Ա�����ֵ ���512
    memcpy(p->tx_buf + 18, & Position1, 2);
    memcpy(p->tx_buf + 20, & Position2, 2);
    memcpy(p->tx_buf + 22, & Position3, 2);
    memcpy(p->tx_buf + 24, & Position4, 2);
    memcpy(p->tx_buf + 26, & Position5, 2);
    memcpy(p->tx_buf + 28, & Position6, 2);
    //����ת�� ��r/min��
    memcpy(p->tx_buf + 30, & ReturnedWheelData[1].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 32, & ReturnedWheelData[2].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 34, & ReturnedWheelData[3].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 36, & ReturnedWheelData[4].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 38, & ReturnedWheelData[5].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 40, & ReturnedWheelData[6].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 42, & liner_x_speed, 4);
    memcpy(p->tx_buf + 46, & anglespeed, 4);
    memcpy(p->tx_buf + 50, & cmd_lost_time, 2);
    p->tx_buf[52] = 0X0C;            //�汾��
    p->tx_buf[53] = MBUS_ON_OFF;     //ң����״̬

    p->tx_buf[61] = 0x00;
    uint8_t checnSum = 0;
    for(i = 0; i < 61; i++)                 //���У��
    {
        checnSum ^= p->tx_buf[i];
    }
    p->tx_buf[61] = checnSum;
}

void ImformationCopy1(COMMUNICATE* p)
{
    u8 i;
    p->tx_buf[0] = 0XAA;
    p->tx_buf[1] = 0X55;
    p->tx_buf[2] = 0X3e;
    //���ӱ�����λ��
    memcpy(p->tx_buf + 3, & ReturnedWheelData[1].ReturnedWheelHallCnt, 2);
    memcpy(p->tx_buf + 5, & ReturnedWheelData[2].ReturnedWheelHallCnt, 2);
    memcpy(p->tx_buf + 7, & ReturnedWheelData[3].ReturnedWheelHallCnt, 2);
    memcpy(p->tx_buf + 9, & ReturnedWheelData[4].ReturnedWheelHallCnt, 2);
    memcpy(p->tx_buf + 11, & ReturnedWheelData[5].ReturnedWheelHallCnt, 2);
    memcpy(p->tx_buf + 13, & ReturnedWheelData[6].ReturnedWheelHallCnt, 2);
    p->tx_buf[15] = MBUS_ON_OFF;     //ң����״̬

    p->tx_buf[23] = 0X00; //���У��
    uint8_t checnSum = 0;
    for(i = 0; i < 23; i++)
    {
        checnSum ^= p->tx_buf[i];
    }
    p->tx_buf[23] = checnSum;
}

void ImformationCopy2(COMMUNICATE* p)
{
    u8 i;
    memcpy(p->tx_buf + 0, & ReturnedWheelData[1].ReturnedWheelErrorCode, 1);
    memcpy(p->tx_buf + 1, & ReturnedWheelData[2].ReturnedWheelErrorCode, 1);
    memcpy(p->tx_buf + 2, & ReturnedWheelData[3].ReturnedWheelErrorCode, 1);

    //����ת�� ��r/min��
    memcpy(p->tx_buf + 3, & ReturnedWheelData[1].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 5, & ReturnedWheelData[2].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 7, & ReturnedWheelData[3].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 9, & ReturnedWheelData[4].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 11, & ReturnedWheelData[5].ReturnedWheelSpeedValue, 2);
    memcpy(p->tx_buf + 13, & ReturnedWheelData[6].ReturnedWheelSpeedValue, 2);
    //ʵʱת��Ƕ�,���Ա�����ֵ ���512
    memcpy(p->tx_buf + 15, & Position1, 2);
    memcpy(p->tx_buf + 17, & Position3, 2);
    memcpy(p->tx_buf + 19, & Position4, 2);
    memcpy(p->tx_buf + 21, & Position6, 2);

    p->tx_buf[23] = 0X00; //���У��
    uint8_t checnSum = 0;
    for(i = 0; i < 23; i++)
    {
        checnSum ^= p->tx_buf[i];
    }
    p->tx_buf[23] = checnSum;
}


//ͨ�ź�����ڣ���ѭ������
u8 recive_ok = 0;
void CommunicateEntry(void)
{
    u8 ch;
    //�������ݽ��ա������Ƿ�Ϸ�
    if(usart1GetByte( & ch))  //ch = usart1���½���8��������
    {
        if(DecodeFrame1By1(ch, & uart1))
        {
            //���ݺϷ�
            pSelect = & uart1;
            //if(uart1.rx_buf[5] == 0x00 || uart1.rx_buf[5] == 0x3f || uart1.rx_buf[5] == 0xff)
                cmd_lost_time = 0;
            recive_ok = 1;
        }
    }
    //CAN������
    if(can1GetByte( & ch))
    {
        if(DecodeFrame1By1(ch, & can1))
        {
            //���ݺϷ�
            pSelect = & can1;
            if(can1.rx_buf[5] == 0x00 || can1.rx_buf[5] == 0x3f || can1.rx_buf[5] == 0xff)
                cmd_lost_time = 0;
            recive_ok = 1;
        }
    }
}

void Sent_Date_PC(void)
{
    ImformationCopy( & uart1);//send
    USART1_Send(uart1.tx_buf, 1);

    
}

//void Get_PC_Date(void)
//{
//    if(recive_ok)
//    {
//        recive_ok = 0;
//        if(pSelect->rx_buf[2] == 0x0F)   //16*8bit���ٶȿ���ָ��
//        {
//            if(pSelect->rx_buf[3] == 0x12)
//            {
//                switch(pSelect->rx_buf[4])
//                {
//                case 0x00:
//                    break;  //registerconfig
//                case 0x01:
//                    memcpy( & PC_x_speed, pSelect->rx_buf + 6, 4);
//                    memcpy( & PC_y_speed, pSelect->rx_buf + 10, 4);
//                    memcpy( & PC_z_speed, pSelect->rx_buf + 14, 4);
//                    break;  //speedconfig
//                case 0x02:
//                    break;  //zhuanjumoshi
//                default :
//                    break;
//                }
//            }
//        }
//    }
//}
#include "gimbal_task.h"
#include "math.h"
extern Gimbal_State gimbal;
Gimbal_Master gimbal_master={
         .master_status = MASTER_NULL,
 };
_Bool master_receive_flag=0;
void gimbal_protocol(COMMUNICATE* p){
			uint8_t tmp;
		if(MBUS_ON_OFF==0x10){
			memcpy( & gimbal_master.master_status, p->rx_buf + 4, 1);
			memcpy( & gimbal_master.yaw, p->rx_buf + 5, 4);
			memcpy( & gimbal_master.pitch, p->rx_buf + 9, 4);
			memcpy( & tmp, p->rx_buf + 13, 1);
			if(tmp&0x80)gimbal_master.left_shoot=1;else gimbal_master.left_shoot=0;
			if(tmp&0x40)gimbal_master.right_shoot=1;else gimbal_master.right_shoot=0;
			gimbal.yaw=gimbal_master.yaw;
			gimbal.pitch=gimbal_master.pitch;
			if(gimbal_master.master_status==MASTER_VELOCITY){
			if(fabs(gimbal.yaw)<0.1)gimbal.yaw=0;
			if(gimbal.yaw>GIMBAL_YAW_MAX_RPM)gimbal.yaw=GIMBAL_YAW_MAX_RPM;
			if(gimbal.yaw<-GIMBAL_YAW_MAX_RPM)gimbal.yaw=-GIMBAL_YAW_MAX_RPM; 
			
			if(fabs(gimbal.pitch)<0.1)gimbal.pitch=0;
			if(gimbal.pitch>GIMBAL_PITCH_MAX_RPM)gimbal.pitch=GIMBAL_PITCH_MAX_RPM;
			if(gimbal.pitch<-GIMBAL_PITCH_MAX_RPM)gimbal.pitch=-GIMBAL_PITCH_MAX_RPM; 
			}
			
			master_receive_flag=1;
 }
}

