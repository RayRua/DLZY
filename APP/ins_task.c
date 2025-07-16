#include "ins_task.h"
#include "wit_c_sdk.h"
#include "usart2.h"
#include "cmsis_os.h"
#include "servo.h"
#include "motor_status.h"
#include "watchdog.h"

#define ACC_UPDATE 0x01
#define GYRO_UPDATE 0x02
#define ANGLE_UPDATE 0x04
#define MAG_UPDATE 0x08
#define READ_UPDATE 0x80
static volatile char s_cDataUpdate = 0, s_cCmd = 0xff;
static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
static void Delayms(uint16_t ucMs);

double gimbal_angle[3];
typedef enum
{
	ANGLE_ROLL = 0,
	ANGLE_PITCH,
	ANGLE_YAW
} Angle;
void ins_init(void)
{
	WitInit(WIT_PROTOCOL_MODBUS, 0x50);
	WitSerialWriteRegister(Uart2_Send);	   // ע��д�ص�����    ����1�������ݵ��� SensorUartSend����
	WitRegisterCallBack(SensorDataUpdata); // ע���ȡ���������ݻص�����   ����2�������ݵ���SensorDataUpdata����
	WitDelayMsRegister(Delayms);		   // ע����ʱ�ص�����
};

static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum) // ��������������
{
	int i;
	for (i = 0; i < uiRegNum; i++)
	{
		switch (uiReg) // �ж�uiReg��������ʲô������ѡ���Ӧ�Ĳ���
		{
		case Roll:

			gimbal_angle[ANGLE_ROLL] = sReg[uiReg] / 32768.0f * 180.0f;
			gimbal_angle[ANGLE_PITCH] = sReg[uiReg + 1] / 32768.0f * 180.0f;
			break;
		}
		uiReg++;
	}
}
static void Delayms(uint16_t ucMs) // ��ʱ����
{
	osDelay(ucMs);
}

void ins_task(void)
{
	//	if(cmd_lost_imu_flag==1)
	//	WitRedStart();
	WitRedRoll_Pitch();
};
