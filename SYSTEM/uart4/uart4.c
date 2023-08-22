#include "sys.h"
#include "uart4.h"	
#include "uart5.h"	
#include "string.h"
#include "Bell.h"

u8  MagneticEncoder_Se_Date[14] ={0X5A,0X0E,0X01,0X01,0X00,0X00,0X00,0X00,0XA5,0X00,0X00,0X06,0X00,0X69};//主机发送该指令，等待编码器返回参数
u8  MagneticEncoder_Se_State[14]={0X5A,0X0E,0X01,0X01,0X00,0X00,0X00,0X00,0XA5,0X00,0X00,0X05,0X00,0X69};//主机发送该指令，等待编码器返回参数
u8  MagneticEncoder_Se_Code[14]={0X5A,0X0E,0X01,0X01,0X00,0X00,0X00,0X00,0XA5,0X00,0X00,0X04,0X00,0X69};//主机发送该指令，等待编码器返回参数
u8  MagneticEncoder_Re_Date[14];//AS5600数据接受缓存区
u8  MagneticEncoder_Re_Count;	  //AS5600数据数据接受计数
u8  MagneticEncoder_Re_FLAG;		//AS5600数据接受完成标志位

u16 Position1,Position2,Position3,Position4,Position5,Position6,Position7,Position8;

u8 step=0; //上电执行步骤
u8 AS5600_Time_Flag=0;//开始计时标志位
u16 AS5600_500ms_cnt=0;//调中指令超时计数器
u8 MagneticEncoder_ID_Number=1;  //AS5600磁编码器ID寄存器
extern u16 time_500ms_flag;	           //500ms定时时间标志位
extern u16 x_speed;
extern u16 y_speed;
extern u16 z_speed;
extern u16 k_number;
extern u8 Remote_Re_Date[25];

u8 As5600_MidValue1_State=0;
u8 As5600_MidValue2_State=0;

u8 As5600_MidValue3_State=0;
u8 As5600_MidValue4_State=0;
u8 As5600_MidValue5_State=0;

u8 As5600_MidValue6_State=0;
u8 As5600_MidValue7_State=0;
u8 As5600_MidValue8_State=0;
/*uart4*/
/**********************************************
串口配置函数   编码器协议
配置了串口4，PC10 PC11
波特率：115200,
数据位：8b，
停止位：1，
校验：N
接收中断使能*/
//**********************************************/

void UART4_Init(u32 bound)
{
  GPIO_InitTypeDef gpio_initstructure;
  USART_InitTypeDef usart_initstructure;
  //UART_ClockInitTypeDef usart_clockinitstructure;	

	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能UART5时钟
 

	USART_DeInit(UART4);  //复位串口4
  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);	
  /* C10 USART4_Tx */   
  gpio_initstructure.GPIO_Pin = GPIO_Pin_10;
  gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
  gpio_initstructure.GPIO_OType = GPIO_OType_PP;
  gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_initstructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &gpio_initstructure);
  
  /* C11 USART4_Rx  */
  gpio_initstructure.GPIO_Pin = GPIO_Pin_11;
  gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
  //gpio_initstructure.GPIO_OType = GPIO_OType_PP;
  gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &gpio_initstructure);

  usart_initstructure.USART_BaudRate = bound;									//波特率是115200
  usart_initstructure.USART_WordLength = USART_WordLength_8b;   //8位数据位
  usart_initstructure.USART_StopBits = USART_StopBits_1;				//1个停止位
  usart_initstructure.USART_Parity =USART_Parity_No;					//偶校验
  usart_initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件控制
  usart_initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART4, &usart_initstructure);
  USART_Cmd(UART4, ENABLE);
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);	
	
	
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口5中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}

void UART4_SendOneByte(u8 dat)
{
    while((UART4->SR&0X40)==0) {}; //循环发送,直到发送完毕
    UART4->DR = (u8) dat;
    while(USART_GetFlagStatus(UART4,USART_FLAG_TC) == RESET);
}

void Sent_MagneticEncoder_Date(u8 ID)//获取每一路的编码器数据
{
	u8 i;
	MagneticEncoder_Se_Date[7]=ID;
	MagneticEncoder_Se_Date[12]=0;
	for (i = 0; i < 12; i++)
  {
        MagneticEncoder_Se_Date[12] += MagneticEncoder_Se_Date[i];
  }
	for(i=0;i<14;i++)
	{
			UART4_SendOneByte(MagneticEncoder_Se_Date[i]);
	}
}

void Sent_MagneticEncoder_State(u8 ID)//获取每一路的是否已经调中值
{
	u8 i;
	MagneticEncoder_Se_State[7]=ID;
	MagneticEncoder_Se_State[12]=0;
	for (i = 0; i < 12; i++)
  {
        MagneticEncoder_Se_State[12] += MagneticEncoder_Se_State[i];
  }
	for(i=0;i<14;i++)
	{
			UART4_SendOneByte(MagneticEncoder_Se_State[i]);
	}
}

void Sent_MagneticEncoder_Code(u8 ID)//发送每一路调中值命令
{
	u8 i;
	MagneticEncoder_Se_Code[7]=ID;
	MagneticEncoder_Se_Code[12]=0;
	for (i = 0; i < 12; i++)
  {
        MagneticEncoder_Se_Code[12] += MagneticEncoder_Se_Code[i];
  }
	for(i=0;i<14;i++)
	{
			UART4_SendOneByte(MagneticEncoder_Se_Code[i]);
	}
}
	u8 id=1;
void Read_AS5600_State(void)
{
	while(1)
	{
		if(step==0)
		{
				Bell();
			  AS5600_Time_Flag=1;//留1秒的时间判断遥控器状态，进行调中，超时则跳出调中，进入询问调中参数状态
				Get_Remote_channel_Date();		//获取当前遥控器的值;
				if((x_speed<=210)&&(y_speed>=1790)&&(k_number>1790))//进入调中值状态
				{
					 AS5600_500ms_cnt=0;
//					 i++;if(i>=7)i=1;
					if(time_500ms_flag==1)
					{
					 switch(id)
					 {
							case 1:if(As5600_MidValue1_State==0)Sent_MagneticEncoder_Code(1);else	id =2;break;
							case 2:if(As5600_MidValue2_State==0)Sent_MagneticEncoder_Code(2);else id =3;break;	
							case 3:if(As5600_MidValue3_State==0)Sent_MagneticEncoder_Code(3);else id =4;break;
							case 4:if(As5600_MidValue4_State==0)Sent_MagneticEncoder_Code(4);else id =5;break;
							case 5:if(As5600_MidValue5_State==0)Sent_MagneticEncoder_Code(5);else id =6;break;
							case 6:if(As5600_MidValue6_State==0)Sent_MagneticEncoder_Code(6);else step=2;break;
						  default:break;
					 }
					 time_500ms_flag=0;
				 } 
				 if((As5600_MidValue1_State==1)&&(As5600_MidValue2_State==1)&&(As5600_MidValue3_State==1)
				  &&(As5600_MidValue4_State==1)&&(As5600_MidValue5_State==1)&&(As5600_MidValue6_State==1))
				 {
					 Bell();
					 while(1)
					 {
						step = 2;
						 if(Remote_Re_Date[23] == 0x10)
						 {
							break;
						 }
					 }	
				 }
					 if(MagneticEncoder_Re_FLAG==1)
					 {
						 if((MagneticEncoder_Re_Date[0]==0x5A)&&(MagneticEncoder_Re_Date[1]==0x0E)&&(MagneticEncoder_Re_Date[11]==0x04)&&(MagneticEncoder_Re_Date[13]==0x69))
						 {
								 switch(MagneticEncoder_Re_Date[7])
								 {
									 case 1:As5600_MidValue1_State=MagneticEncoder_Re_Date[3];break;
									 case 2:As5600_MidValue2_State=MagneticEncoder_Re_Date[3];break;
									 case 3:As5600_MidValue3_State=MagneticEncoder_Re_Date[3];break;
									 case 4:As5600_MidValue4_State=MagneticEncoder_Re_Date[3];break;
									 case 5:As5600_MidValue5_State=MagneticEncoder_Re_Date[3];break;
									 case 6:As5600_MidValue6_State=MagneticEncoder_Re_Date[3];break;
									 default:break;
								 }
									MagneticEncoder_Re_FLAG=0;
						 }
					 }
				}
		}
		else if(step==1)
		{
//			     i++;if(i>=7)i=1;
					 switch(MagneticEncoder_ID_Number)
					 {
						  case 1:	if(As5600_MidValue1_State==0)Sent_MagneticEncoder_State(1);break;
							case 2:	if(As5600_MidValue2_State==0)Sent_MagneticEncoder_State(2);break;
							case 3: if(As5600_MidValue3_State==0)Sent_MagneticEncoder_State(3);break;
							case 4: if(As5600_MidValue4_State==0)Sent_MagneticEncoder_State(4);break;
							case 5: if(As5600_MidValue5_State==0)Sent_MagneticEncoder_State(5);break;
							case 6: if(As5600_MidValue6_State==0)Sent_MagneticEncoder_State(6);break;
						  default:break;
					 }
					 if((As5600_MidValue1_State==1)&&(As5600_MidValue2_State==1)&&(As5600_MidValue3_State==1)
					  &&(As5600_MidValue4_State==1)&&(As5600_MidValue5_State==1)&&(As5600_MidValue6_State==1))
					 {
							step=2; //已经调中的机器，可直接进入运行状态
					 }
					 if(MagneticEncoder_Re_FLAG==1)
					 {
						 if((MagneticEncoder_Re_Date[0]==0x5A)&&(MagneticEncoder_Re_Date[1]==0x0E)&&(MagneticEncoder_Re_Date[11]==0x05)&&(MagneticEncoder_Re_Date[13]==0x69))
						 {
								 switch(MagneticEncoder_Re_Date[7])
								 {
									 case 1:As5600_MidValue1_State=MagneticEncoder_Re_Date[3];break;
									 case 2:As5600_MidValue2_State=MagneticEncoder_Re_Date[3];break;
									 case 3:As5600_MidValue3_State=MagneticEncoder_Re_Date[3];break;
									 case 4:As5600_MidValue4_State=MagneticEncoder_Re_Date[3];break;
									 case 5:As5600_MidValue5_State=MagneticEncoder_Re_Date[3];break;
									 case 6:As5600_MidValue6_State=MagneticEncoder_Re_Date[3];break;
									 default:break;
								 }
									MagneticEncoder_Re_FLAG=0;
						  }
					 }
		}
		else if(step==2)
		{
				Bell();
			  step=3;
				break; //进入运行模式
		}
	}
}



void UART4_IRQHandler(void)                	//串口4中断服务程序
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  			// 空闲中断
    {
				 MagneticEncoder_Re_Date[MagneticEncoder_Re_Count]=USART_ReceiveData(UART4);
				 MagneticEncoder_Re_Count++;
				 if(MagneticEncoder_Re_Date[0]==0x5a)
				 {
						if(MagneticEncoder_Re_Count>=14)
						{
									MagneticEncoder_Re_Count=0;
									MagneticEncoder_Re_FLAG=1;
						}
				}
				else
				{
						MagneticEncoder_Re_Count=0;
				}
		    USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
	if(USART_GetITStatus(UART4,USART_IT_TXE)==SET)
  {
    USART_ClearITPendingBit(UART4,USART_IT_TXE);
  }
}


///*获取AS5600实时数据*/
void Get_MagneticEncoder_Date(void)
{
		 if(MagneticEncoder_Re_FLAG==1)
		 {
			  if((MagneticEncoder_Re_Date[0]==0x5A)&&(MagneticEncoder_Re_Date[1]==0x0E)&&(MagneticEncoder_Re_Date[13]==0x69))
				{
					 switch(MagneticEncoder_Re_Date[7])	
					 {
						 case 1:Position1=MagneticEncoder_Re_Date[2]*256+MagneticEncoder_Re_Date[3];break;
						 case 2:Position2=MagneticEncoder_Re_Date[2]*256+MagneticEncoder_Re_Date[3];break;
						 case 3:Position3=MagneticEncoder_Re_Date[2]*256+MagneticEncoder_Re_Date[3];break;
						 case 4:Position4=MagneticEncoder_Re_Date[2]*256+MagneticEncoder_Re_Date[3];break;
						 case 5:Position5=MagneticEncoder_Re_Date[2]*256+MagneticEncoder_Re_Date[3];break;
						 case 6:Position6=MagneticEncoder_Re_Date[2]*256+MagneticEncoder_Re_Date[3];break;
						 default:break;
					 }
						MagneticEncoder_Re_FLAG=0;
				}
			}
}






