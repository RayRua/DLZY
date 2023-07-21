#include "sys.h"
#include "usart.h"	
#include "uart4.h"	
#include "uart5.h"	
#include "string.h"


extern u16 x_speed;
extern u16 y_speed;
extern u16 z_speed;
extern u8 PC_ReTime_flag;
extern u16 PC_ReTime_cnt;
extern u8 PC_ReTime_stop_flag;
extern u8 MBUS_ON_OFF;
extern u8  Batter_Voltage[2];			//电池电压
extern u8  Batter_Current[2];			//电池实时电流
extern u8  Batter_Capacity_Number;				//电池电量百分比
extern u8  Batter_State[2];				//电池保护状态

u8 PC_Sent_Date[62]={0};
extern u16 Position1,Position2,Position3,Position4,Position5,Position6,Position7,Position8;

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
 


//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	


uint8_t PC_Re_Date[19];//接受到这个值表示需要更新固件了
uint8_t PC_Re_Count;	 //接受计数
uint8_t PC_Re_FLAG;		 //接受完成标志位

float PC_x_speed;
float PC_y_speed;
float PC_z_speed;
//初始化IO 串口1 
//bound:波特率
void USART1_Init(u32 bound)
	{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	
	
/*************************************UART1***************************************************/
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

	
}
void USART1_SendOneByte(u8 dat)
{
    while((USART1->SR&0X40)==0) {}; //循环发送,直到发送完毕
    USART1->DR = (u8) dat;
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}
void Sent_Date_PC()
{
	u8 i;
	PC_Sent_Date[0]=0XAA;
	PC_Sent_Date[1]=0X55;
	PC_Sent_Date[2]=0X3e;
	
	PC_Sent_Date[3]=0X00; //1号轮码盘数低位
	PC_Sent_Date[4]=0X00; //1号轮码盘数高位
	
	PC_Sent_Date[5]=0X00; //2号轮码盘数低位
	PC_Sent_Date[6]=0X00; //2号轮码盘数高位
	
	PC_Sent_Date[7]=0X00; //3号轮码盘数低位
	PC_Sent_Date[8]=0X00; //3号轮码盘数高位
	
	PC_Sent_Date[9]=0X00; //4号轮码盘数高位
	PC_Sent_Date[10]=0X00;//4号轮码盘数高位
	
	PC_Sent_Date[11]=0X00;//5号轮码盘数高位
	PC_Sent_Date[12]=0X00;//5号轮码盘数高位
	
	PC_Sent_Date[13]=0X00;//6号轮码盘数高位
	PC_Sent_Date[14]=0X00;//6号轮码盘数高位
	
	PC_Sent_Date[15]=0X00;//--
	PC_Sent_Date[16]=0X00;//--
	
	
	PC_Sent_Date[17]=0X00;//--
	PC_Sent_Date[18]=0X00;//--
	
	PC_Sent_Date[19]=0X00;
	PC_Sent_Date[20]=0X00;
	
	PC_Sent_Date[21]=0X80;//二进制最高2位8轮:11 6轮：10 4轮：01   剩余6位为版本号
	
	PC_Sent_Date[22]=0;//通道5
	PC_Sent_Date[23]=0;//通道6
	
	PC_Sent_Date[24]=0; //0 	正常				8 	温度保护
											//2 	霍尔故障		9 	电流偏置故障
											//3 	转把故障		10 	电压偏置故障
											//5 	过电流保护	11 	母线电流偏置故障
											//6 	过压保护		12 	堵转保护
											//7 	欠压保护		13 	UART故障
	PC_Sent_Date[25]=0;
	PC_Sent_Date[26]=0;
	PC_Sent_Date[27]=0;
	PC_Sent_Date[28]=0;
	PC_Sent_Date[29]=0;
	PC_Sent_Date[30]=0;
	PC_Sent_Date[31]=0;
	
	
	PC_Sent_Date[32]=Position1&0x00ff;
	PC_Sent_Date[33]=Position1>>8;
	
	PC_Sent_Date[34]=Position2&0x00ff;
	PC_Sent_Date[35]=Position2>>8;
	
	PC_Sent_Date[36]=Position3&0x00ff;
	PC_Sent_Date[37]=Position3>>8;
	
	PC_Sent_Date[38]=Position4&0x00ff;
	PC_Sent_Date[39]=Position4>>8;
	
	PC_Sent_Date[40]=Position5&0x00ff;
	PC_Sent_Date[41]=Position5>>8;
	
	PC_Sent_Date[42]=Position6&0x00ff;
	PC_Sent_Date[43]=Position6>>8;
	
	PC_Sent_Date[44]=Position7&0x00ff;
	PC_Sent_Date[45]=Position7>>8;
	
	PC_Sent_Date[46]=Position8&0x00ff;
	PC_Sent_Date[47]=Position8>>8;
	
	PC_Sent_Date[48]=0X00;
	PC_Sent_Date[49]=0X00;
	
	PC_Sent_Date[50]=0X00;//转向闭环检测
	PC_Sent_Date[51]=0X00;//转向闭环检测
	
	PC_Sent_Date[52]=0X00;//防撞
	
	PC_Sent_Date[53]=MBUS_ON_OFF;//遥控器状态
	
    PC_Sent_Date[54]=Batter_Voltage[1];	//电池电压
	PC_Sent_Date[55]=Batter_Voltage[0];
	
	PC_Sent_Date[56]=Batter_Current[1];//电池实时电流
	
	PC_Sent_Date[57]=Batter_Current[0];
	
	PC_Sent_Date[58]=Batter_Capacity_Number;//电池容量百分比
	
	PC_Sent_Date[59]=Batter_State[1];  //电池保护状态
	PC_Sent_Date[60]=Batter_State[0];//电池保护状态
		
	PC_Sent_Date[61]=0XAA;   //异或校验
	
	//异或校验前清零
	PC_Sent_Date[61] = 0x00;	
	for(i = 0;i < 61; i++)					//异或校验
	{
		PC_Sent_Date[61] ^= PC_Sent_Date[i]; 
	}
	
	
	for(i=0;i<62;i++)
	{
		USART1_SendOneByte(PC_Sent_Date[i]);
	}
}
void Get_PC_Date(void)
{
		if((PC_Re_Date[0]==0xaa)&&(PC_Re_Date[1]==0x55)&&(PC_Re_Date[2]==0x0f))
		{
				PC_ReTime_flag=1;//PC发来的数据开始计数，当超过500毫秒没收到，则进行停止，数据清零
			  PC_ReTime_cnt=0;
				PC_ReTime_stop_flag=0;
				memcpy(&PC_x_speed,PC_Re_Date+6,4);
				memcpy(&PC_y_speed,PC_Re_Date+10,4);
				memcpy(&PC_z_speed,PC_Re_Date+14,4);
		}

}
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  			// 空闲中断
  {
  				PC_Re_Date[PC_Re_Count]=USART_ReceiveData(USART1);
          PC_Re_Count++;
          if(PC_Re_Date[0]==0xAA)
	  			{
							if(PC_Re_Count>18)
							{
									PC_Re_Count=0;
									PC_Re_FLAG=1;
							}
					}
					else
					{
							PC_Re_Count=0;
					}
					USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  			// 空闲中断
	{	
				USART_ReceiveData(USART1);																// 清除串口空闲中断
				USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
		
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
//		
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else USART_RX_STA|=0x8000;	//接收完成了 
//			}
//			else //还没收到0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}   		 
//  } 
}

#endif	

 



