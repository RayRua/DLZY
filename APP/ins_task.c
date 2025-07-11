#include "ins_task.h"
#include "wit_c_sdk.h"
#include "usart2.h"
#include "cmsis_os.h"
#include "servo.h"
#include "motor_status.h"
#include "watchdog.h"

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80
static volatile char s_cDataUpdate = 0, s_cCmd = 0xff;
static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
static void Delayms(uint16_t ucMs);

double gimbal_angle[3];
typedef enum{
	ANGLE_ROLL=0,
	ANGLE_PITCH,
	ANGLE_YAW
}Angle;
void ins_init(void){
		WitInit(WIT_PROTOCOL_MODBUS, 0x50);
		WitSerialWriteRegister(Uart2_Send);//注册写回调函数    串口1接收数据调用 SensorUartSend函数
		WitRegisterCallBack(SensorDataUpdata);//注册获取传感器数据回调函数   串口2接收数据调用SensorDataUpdata函数
		WitDelayMsRegister(Delayms);//注册延时回调函数
};

static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)//传感器数据升级
{
	int i;
    for(i = 0; i < uiRegNum; i++)
    {
        switch(uiReg)//判断uiReg的数据是什么来进行选择对应的操作
        {           
            case Roll:
								
									gimbal_angle[ANGLE_ROLL]=sReg[uiReg] / 32768.0f * 180.0f;
									gimbal_angle[ANGLE_PITCH]=sReg[uiReg+1] / 32768.0f * 180.0f;
									break;
        }
		uiReg++;
    }
}
static void Delayms(uint16_t ucMs)//延时程序
{
	osDelay(ucMs);
}

void ins_task(void){
//	if(cmd_lost_imu_flag==1)
//	WitRedStart();
	WitRedRoll_Pitch();
	};
