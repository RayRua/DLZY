#include "init_task.h"

#include "cmsis_os.h"

// 1ms
#include "uart4.h"
#include "uart5.h"
#include "pin.h"

#include "detect_task.h"

#include "gimbal_task.h"
#include "motor_config.h"

#include "angle_speed.h"

#include "ins_task.h"
#include "wit_c_sdk.h"
// 100ms
#include "communicate.h"
#include "usart3.h"
#include "led.h"
// test
#include "usart2.h"
#include "uart4.h"

extern u8 MBUS_ON_OFF;               // 遥控器开关机状态
extern u8 Seg_Display_Flag;          // 数码管显示切换标志位
extern u8 MagneticEncoder_ID_Number; // AS5600磁编码器ID的存储

extern u8 AS5600_Time_Flag;     // 初始化时间标志位
extern u16 AS5600_500ms_cnt;    // 编码器指令超时计数
extern u8 step;                 // 上电执行步骤
extern u8 Mbus_Time_Count_Flag; // MUBS是否超时标志位
extern u16 Mbus_Time_500ms_cnt; // 编码器指令超时计数
extern u16 err_signal[30];

extern uint16_t count_gimbal;
// 外部声明中断使能标志
extern volatile u8 irq_enabled;

osThreadId insTaskHandle;
osThreadId gimbalTaskHandle;
osThreadId detectTaskHandle;
osThreadId TaskHandle_1;
osThreadId TaskHandle_100;

// void StartINSTASK(void const * argument);
void StartGimbalTask(void const *argument);
void StartDetectTask(void const *argument);
void Start_1ms_Task(void const *argument);
void Start_100ms_Task(void const *argument);

void init_task()
{

  // INS
  //  osThreadDef(ins_task, StartINSTASK, osPriorityNormal, 0, 1024);
  //  insTaskHandle = osThreadCreate(osThread(ins_task), NULL);
  //  if (insTaskHandle == NULL) {
  //    // 任务创建失败处理
  //    while(1); // 或者添加错误处理代码
  //  }

  // GIMBAL
  osThreadDef(gimbal_task, StartGimbalTask, osPriorityNormal, 0, 512);
  gimbalTaskHandle = osThreadCreate(osThread(gimbal_task), NULL);
  if (gimbalTaskHandle == NULL)
  {
    while (1)
      ;
  }

  // DETECT
  osThreadDef(detect_task, StartDetectTask, osPriorityNormal, 0, 256);
  detectTaskHandle = osThreadCreate(osThread(detect_task), NULL);
  if (detectTaskHandle == NULL)
  {
    while (1)
      ;
  }

  // CHASSIS
  osThreadDef(TaskHandle_1, Start_1ms_Task, osPriorityNormal, 0, 256);
  TaskHandle_1 = osThreadCreate(osThread(TaskHandle_1), NULL);
  if (TaskHandle_1 == NULL)
  {
    while (1)
      ;
  }

  // led_daemon
  osThreadDef(100ms_task, Start_100ms_Task, osPriorityNormal, 0, 128);
  TaskHandle_100 = osThreadCreate(osThread(100ms_task), NULL);
  if (TaskHandle_100 == NULL)
  {
    while (1)
      ;
  }
}

// void StartINSTASK(void const * argument)
//{
////	while(WitStartREFANGLECali()!=WIT_HAL_OK){flag_remote_shut=1;};
////	WitStopAccCali();
////	WitStopREFANGLECali();
////
//  /* USER CODE BEGIN 5 */
//  /* Infinite loop */
//  for(;;)
//  {
//		// 读取PD4和PD5电平
//
//    osDelay(1);
//  }
//  /* USER CODE END 5 */
//}

void StartGimbalTask(void const *argument)
{
  D_AI_Motor_Control_SHIFIT(GIMBAL_PITCH, D_AI_VELOCITY);
  DRV750_Motor_Control_SHIFIT(GIMBAL_YAW, DRV750_VELOCITY);
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for (;;)
  {
    gimbal_task();

    osDelay(1);
  }
  /* USER CODE END 5 */
}

void StartDetectTask(void const *argument)
{

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for (;;)
  {

    detect_task();
    osDelay(1);
  }
  /* USER CODE END 5 */
}

void Start_1ms_Task(void const *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for (;;)
  {
    //		if(step == 0) /*上电检测遥控器编码器数值进行初始化*/
    //        {
    //            Get_Remote_channel_Date();
    //            //setMagneticEncoder_Code();
    //
    //        }
    //        else
    //        {
    CommunicateEntry();        // 数据合法校验并选择通信模式 usart4   master 位号 DB1
    Get_Remote_channel_Date(); // 获取当前遥控器数值;
    //						Get_MagneticEncoder_Date();                     //获取AS5600位置的值
    // WheelReturnedDataHandle();											//获取轮子返回数据信息
    // ALL_speed_OUT();                                //输出速度和舵机角度值输出
    //}

    // UART4_SendOneByte(0x11);
    osDelay(1);
  }
  /* USER CODE END 5 */
}

void Start_100ms_Task(void const *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */

  for (;;)
  {

    // Sent_MotorSpeed_code();   //J10  //发送8路舵机数值 usart 3  位号J10
    Sent_Date_PC(); // test uart1 working normaly
    LED0 = !LED0;   // 显示系统运行状态
    // always lighting
    count_gimbal++;
    osDelay(100);
  }
  /* USER CODE END 5 */
}
