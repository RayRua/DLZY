#include "sys.h"
#include "usart2.h"	



u8 SteeringMotor_Date[48]={0};//转向电机通信协议，控制板至驱动板驱动协议
u16 Position1_Data=2048;
u16 Position2_Data=2048;
u16 Position3_Data=2048;
u16 Position4_Data=2048;
u16 Position5_Data=2048;
u16 Position6_Data=2048;
u16 Position7_Data=2048;
u16 Position8_Data=2048;

u32 Check_Sum_Temp;
u8 Check_Sum_Set;

extern u16 Position1,Position2,Position3,Position4,Position5,Position6,Position7,Position8;
/**********************************************
串口配置函数
RS485通信控制所有舵机和电机
配置了串口2，PD5 PD6
115200,8b，1，N
接收中断使能
无发送
**********************************************/
void USART2_Init(u32 bound)
{
		GPIO_InitTypeDef gpio_initstructure;
		USART_InitTypeDef usart_initstructure;
		//USART_ClockInitTypeDef usart_clockinitstructure;	

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOD时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟
		USART_DeInit(USART2);  //复位串口2
			
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);	
		/* D5 USART2_Tx */   
		gpio_initstructure.GPIO_Pin = GPIO_Pin_5;
		gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
		gpio_initstructure.GPIO_OType = GPIO_OType_PP;
		gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
		gpio_initstructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &gpio_initstructure);
		
		/* D6 USART2_Rx  */
		gpio_initstructure.GPIO_Pin = GPIO_Pin_6;
		gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
		//gpio_initstructure.GPIO_OType = GPIO_OType_PP;
		gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
		gpio_initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &gpio_initstructure);	
  
		usart_initstructure.USART_BaudRate = bound;
		usart_initstructure.USART_WordLength = USART_WordLength_8b;
		usart_initstructure.USART_StopBits = USART_StopBits_1;
		usart_initstructure.USART_Parity = USART_Parity_No;
		usart_initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		usart_initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &usart_initstructure);
		USART_Cmd(USART2, ENABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
}

void USART2_SendOneByte(u8 dat)
{
    while((USART2->SR&0X40)==0) {}; //循环发送,直到发送完毕
    USART2->DR = (u8) dat;
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

void USART2_SendStr(u8 *pucStr, u8 ulNum) 
{   
		u8 i; 
		for(i = 0;i<ulNum;i++)
		{
			USART2_SendOneByte(*pucStr++);
		}    
} 

void Sent_SteeringMotor_Pozition(void)
{
		u8 Check_Count;
  SteeringMotor_Date[0]=0xff;//head0
  SteeringMotor_Date[1]=0xff;//head1
  SteeringMotor_Date[2]=0xfe;//ID
  SteeringMotor_Date[3]=0x2C;//同时控制6个舵机
  SteeringMotor_Date[4]=0x83;//指令
  SteeringMotor_Date[5]=0x2A;//写入数据首地址
  SteeringMotor_Date[6]=0x04;//写入的数据长度

  SteeringMotor_Date[7]=0x01;//写入第一个舵机的ID
  SteeringMotor_Date[8]=Position1_Data>>8;	        //目标位置高位
  SteeringMotor_Date[9]=Position1_Data&0x00ff;			//目标位置低位
  SteeringMotor_Date[10]=Position1>>8;	        //运行时间高位
  SteeringMotor_Date[11]=Position1&0x00ff;			//运行时间低位

  SteeringMotor_Date[12]=0x02;
  SteeringMotor_Date[13]=Position2_Data>>8;
  SteeringMotor_Date[14]=Position2_Data&0x00ff;
  SteeringMotor_Date[15]=Position2>>8;
  SteeringMotor_Date[16]=Position2&0x00ff;

  SteeringMotor_Date[17]=0x03;
  SteeringMotor_Date[18]=Position3_Data>>8;
  SteeringMotor_Date[19]=Position3_Data&0x00ff;
  SteeringMotor_Date[20]=Position3>>8;
  SteeringMotor_Date[21]=Position3&0x00ff;

  SteeringMotor_Date[22]=0x04;
  SteeringMotor_Date[23]=Position4_Data>>8;
	SteeringMotor_Date[24]=Position4_Data&0x00ff;
  SteeringMotor_Date[25]=Position4>>8;
  SteeringMotor_Date[26]=Position4&0x00ff;

  SteeringMotor_Date[27]=0x05;
  SteeringMotor_Date[28]=Position5_Data>>8;
  SteeringMotor_Date[29]=Position5_Data&0x00ff;
  SteeringMotor_Date[30]=Position5>>8;
  SteeringMotor_Date[31]=Position5&0x00ff;

  SteeringMotor_Date[32]=0x06;
  SteeringMotor_Date[33]=Position6_Data>>8;
  SteeringMotor_Date[34]=Position6_Data&0x00ff;
  SteeringMotor_Date[35]=Position6>>8;
  SteeringMotor_Date[36]=Position6&0x00ff;

  SteeringMotor_Date[37]=0x07;
  SteeringMotor_Date[38]=Position7_Data>>8;
  SteeringMotor_Date[39]=Position7_Data&0x00ff;
  SteeringMotor_Date[40]=Position7>>8;
  SteeringMotor_Date[41]=Position7&0x00ff;
  
  SteeringMotor_Date[42]=0x08;
  SteeringMotor_Date[43]=Position8_Data>>8;
  SteeringMotor_Date[44]=Position8_Data&0x00ff;
  SteeringMotor_Date[45]=Position8>>8;
  SteeringMotor_Date[46]=Position8&0x00ff;

  Check_Sum_Temp=0;
   for(Check_Count=2;Check_Count<=46;Check_Count++)
   {
     Check_Sum_Temp+=SteeringMotor_Date[Check_Count];
   }

  Check_Sum_Temp=~Check_Sum_Temp;

  Check_Sum_Set=(u8)Check_Sum_Temp;

  SteeringMotor_Date[47]=Check_Sum_Set;
	USART2_SendStr(SteeringMotor_Date,48);
}




void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  			// 空闲中断
    {
//				 Remote_Re_Date[Remote_Re_Count]=USART_ReceiveData(USART2);
//				 Remote_Re_Count++;
//				 if(Remote_Re_Date[0]==0x0F)
//				{
//						if(Remote_Re_Count>24)
//						{
//									Remote_Re_Count=0;
//									Remote_Re_FLAG=1;
//						}
//				}
//				else
//				{
//						Remote_Re_Count=0;
//				}
		    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART2,USART_IT_TXE)==SET)
  {
    USART_ClearITPendingBit(USART2,USART_IT_TXE);
  }
}



