#include "sys.h"
#include "delay.h"  
#include "usart.h" 
#include "usart2.h" 
#include "usart3.h" 
#include "uart4.h"	
#include "uart5.h" 
#include "usart6.h" 
#include "led.h"
#include "angle_speed.h" 
#include "usmart.h"
#include "stmflash.h"


u16 time_5ms_cnt;			           //5ms定时时间
u16 time_5ms_flag;		           //5ms定时时间标志位
u16 time_100ms_cnt;			         //100ms定时时间
u16 time_100ms_flag;		         //100ms定时时间标志位
u16 time_500ms_cnt;		           //500ms定时时间
u16 time_500ms_flag;	           //500ms定时时间标志位
extern u8 Seg_Display_Flag;      //数码管显示切换标志位
extern u8 MagneticEncoder_ID_Number;  //AS5600磁编码器ID寄存器

extern u8 AS5600_Time_Flag;//开始计时标志位
extern u16 AS5600_500ms_cnt;//调中指令超时计数器
extern u8 step; //上电执行步骤
extern u8 Mbus_Time_Count_Flag;//MUBS是否连接标志位
extern u16 Mbus_Time_500ms_cnt;//调中指令超时计数器



extern u8 PC_ReTime_flag;
u16 PC_ReTime_cnt;
u8 PC_ReTime_stop_flag=1;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置系统中断优先级分组2
	delay_init(168);      														//初始化延时函数
	
	USART1_Init(115200);															//主串口，波特率为115200
	USART2_Init(115200);															//控制所有舵机
	USART3_Init(115200);															//控制所有电机
	UART4_Init(115200);																//读取6路AS5600编码器的值
	UART5_Init(100000);																//遥控接收初始化；波特率100000  9 0 E 2  M.BUS协议
	USART6_Init(9600);																//获取电池的参数
	
	LED_Init();					  														//初始化LED显示
	DigitalTube_LED_Init();														 //初始化数码管显示
	LED0=0;
	LED1=1;
	delay_ms(1000);
	SysTick_Config(SystemCoreClock /1000);   					//配置时基准为1ms
  	Read_AS5600_State();															//配置和读取as5600中值和状态

	//memcpy(&x22,&yy1,4);
	while(1) 
	{	
		Get_Remote_channel_Date();											//获取当前遥控器的值;
		Get_MagneticEncoder_Date();                     //获取AS5600位置的值
		ALL_speed_OUT();																//电机速度和舵机角度值计算
		Get_Batter_Date();														  //获取电池数据
		Get_MotorSpeed_Re_Date();
		/*每500毫秒执行*/
		if(time_500ms_flag==1)													//每500毫秒执行一次
		{ 
				Sent_Batter_code();													//定时发送获取电池参数的指令
			  time_500ms_flag=0;
		}
		/*每100毫秒执行*/
		if(time_100ms_flag==1)													//每100毫秒执行一次
		{
			 Sent_MotorSpeed_code();	      							//发送8个电机值
			  Sent_SteeringMotor_Pozition();							//发送8个舵机值
			  time_100ms_flag=0;  
				LED0=!LED0;																	//提示系统正在运行	
				LED1=!LED1;																	//提示系统正在运行;
		}
		/*每5毫秒执行*/
		if(time_5ms_flag==1)
		{								
			if(Seg_Display_Flag==1)Seg_Display_Flag=0;else Seg_Display_Flag=1;	//数码管位切换	
			Sent_MagneticEncoder_Date(MagneticEncoder_ID_Number);								//获取每一路的编码器数据
			time_5ms_flag=0;
		}
	}	
}
/*时基中断*/
void SysTick_Handler(void)
{
	time_5ms_cnt++;
	time_100ms_cnt++;
  time_500ms_cnt++;
	
  if(time_5ms_cnt>=5)//5ms
  {
		time_5ms_flag=1;
    time_5ms_cnt=0;
		Seg_Display(33);//数码管显示	
		if(MagneticEncoder_ID_Number>=7)MagneticEncoder_ID_Number=1;else MagneticEncoder_ID_Number++;//AS5600磁编码器ID寄存器
  }
	if(time_100ms_cnt>=100)//100ms
  {
    time_100ms_cnt=0;
		time_100ms_flag=1;
  }
	
  if(time_500ms_cnt>=500)//500ms
  {
    time_500ms_cnt=0;
		time_500ms_flag=1;
  }
	
	if(PC_ReTime_flag==1)//PC收到数据开始计数,超时则停止
	{
			PC_ReTime_cnt++;
			if(PC_ReTime_cnt>500)
			{
					PC_ReTime_cnt=0;
					PC_ReTime_stop_flag=1;
				  PC_ReTime_flag=0;
			}
	
	}
	if(AS5600_Time_Flag==1)
	{
			AS5600_500ms_cnt++;
			if(AS5600_500ms_cnt>1000)
			{
					AS5600_500ms_cnt=0;
				  AS5600_Time_Flag=0;
				  step=1;//AS5600调中指令超时，进入读取调中参数的状态
			}
	}
	if(Mbus_Time_Count_Flag==1)
	{
			Mbus_Time_500ms_cnt++;
		  if(Mbus_Time_500ms_cnt>500)
			{
					Mbus_Time_500ms_cnt=0;
				  Mbus_Time_Count_Flag=0;
			}
	}

}
