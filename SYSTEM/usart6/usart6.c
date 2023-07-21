#include "sys.h"
#include "usart6.h"	
#include "usart.h"	
#include "string.h"
#include "delay.h"
/*uart6*/
u8  Batter_Re_Date[34];//电池接受缓存区
u8  Batter_Re_Count;	  //电池数据接受计数
u8  Batter_Re_FLAG;		//电池接受完成标志位

u8  Batter_Se_Date[7]={0XDD,0XA5,0X03,0X00,0XFF,0XFD,0X77};//主机发送该指令，等待电池返回参数

u8  Batter_Voltage[2];			//电池电压
u16 Batter_Voltage_Number;	//电池电压
u8  Batter_Current[2];			//电池实时电流
u16 Batter_Current_Number;	//电池实时电流
u8  Batter_Capacity_Number;				//电池电量百分比
u8  Batter_State[2];				//电池保护状态
/**********************************************
串口配置函数
配置了串口6，PC6、7
波特率：9600,
数据位：8b，
停止位：1，
校验：N
接收中断使能*/

//**********************************************/

void USART6_Init(u32 bound)
{
  GPIO_InitTypeDef gpio_initstructure;
  USART_InitTypeDef usart_initstructure;
  //UART_ClockInitTypeDef usart_clockinitstructure;	

	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE); //使能USART6时钟
 

	USART_DeInit(USART6);  //复位串口6
  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);	
  /* C12 USART5_Tx */   
  gpio_initstructure.GPIO_Pin = GPIO_Pin_6;
  gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
  gpio_initstructure.GPIO_OType = GPIO_OType_PP;
  gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_initstructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &gpio_initstructure);
  
  /* D2 USART5_Rx  */
  gpio_initstructure.GPIO_Pin = GPIO_Pin_7;
  gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
  //gpio_initstructure.GPIO_OType = GPIO_OType_PP;
  gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &gpio_initstructure);

  usart_initstructure.USART_BaudRate = bound;									  //波特率是
  usart_initstructure.USART_WordLength = USART_WordLength_8b;   //8位数据位
  usart_initstructure.USART_StopBits = USART_StopBits_1;				//1个停止位
  usart_initstructure.USART_Parity =USART_Parity_No;					  //无校验
  usart_initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件控制
  usart_initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &usart_initstructure);
  USART_Cmd(USART6, ENABLE);
  USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);	
	
	
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口6中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}

void Usart6_SendOneByte(u8 dat)
{
    while((USART6->SR&0X40)==0) {}; //循环发送,直到发送完毕
    USART6->DR = (u8) dat;
    while(USART_GetFlagStatus(USART6,USART_FLAG_TC) == RESET);
}


void USART6_IRQHandler(void)                	//串口6中断服务程序
{
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  			// 空闲中断
    {
				 Batter_Re_Date[Batter_Re_Count]=USART_ReceiveData(USART6);
				 Batter_Re_Count++;
				 if(Batter_Re_Date[0]==0xDD)
				{
						if(Batter_Re_Count>33)
						{
									Batter_Re_Count=0;
									Batter_Re_FLAG=1;  //电池数据一帧接受完成
						}
				}
				else
				{
						Batter_Re_Count=0;
				}
		    USART_ClearITPendingBit(USART6, USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART6,USART_IT_TXE)==SET)
  {
    USART_ClearITPendingBit(USART6,USART_IT_TXE);
  }
}

void Sent_Batter_code(void)
{
	u8 i;
	for(i=0;i<7;i++)
	{
			Usart6_SendOneByte(Batter_Se_Date[i]);
	}
}


///*获取电池实时数据*/
void Get_Batter_Date(void)
{

		 if(Batter_Re_FLAG==1)
		 {
			  if((Batter_Re_Date[0]==0xdd)&&(Batter_Re_Date[2]==0x00)&&(Batter_Re_Date[33]==0x77))
				{
					 /*获取电池实时电压*/
							Batter_Voltage[1]=Batter_Re_Date[4];
							Batter_Voltage[0]=Batter_Re_Date[5];
							Batter_Voltage_Number=Batter_Voltage[1]*256+Batter_Voltage[0];
					 /*获取电池实时电流*/
							Batter_Current[1]=Batter_Re_Date[6];
							Batter_Current[0]=Batter_Re_Date[7];
							Batter_Current_Number=Batter_Current[1]*256+Batter_Current[0];
					 /*获取电池剩余容量*/
							Batter_Capacity_Number=Batter_Re_Date[23];
					 /*获取电池各种保护状态*/
					    Batter_State[1]=Batter_Re_Date[20];
						  Batter_State[0]=Batter_Re_Date[21];
							Batter_Re_Date[0]=0x00;
							Batter_Re_FLAG=0;
				}
			}
}




