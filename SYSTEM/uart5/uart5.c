#include "sys.h"
#include "uart5.h"	
#include "usart.h"	
#include "string.h"
/*uart5*/
u8 Remote_Re_Date[25];//遥控器接受缓存区
u8 Remote_Re_Count;	 //遥控器接受计数
u8 Remote_Re_FLAG;		 //遥控器接受完成标志位
u16 MBUS_CH[17];			//遥控器16个通道的实时数值，实际使用只用了6个通道
u16 x_speed=0;
u16 y_speed=0;
u16 z_speed=0;
u16 k_number=0;
u16 key5,key6;//遥控器的第5、6通道，
u8 MBUS_ON_OFF;//遥控器开关机状态
u8 Mbus_Time_Count_Flag=0;//MUBS是否连接标志位
u16 Mbus_Time_500ms_cnt=0;//调中指令超时计数器
//typedef struct
//{
//  u8 address;
//  u16 ech_data;
////  u16 ech_data_pre;
////  u16 delta;
//}MBUS_CH;
//typedef struct _sbusPacket
//{
// uint8_t   startbyte;
// uint32_t chan0 : 11;
// uint32_t chan1 : 11;
// uint32_t chan2 : 11;
// uint32_t chan3 : 11;
// uint32_t chan4 : 11;
// uint32_t chan5 : 11;
// uint32_t chan6 : 11;
// uint32_t chan7 : 11;
// uint32_t chan8 : 11;
// uint32_t chan9 : 11;
// uint32_t chan10 : 11;
// uint32_t chan11 : 11;
// uint32_t chan12 : 11;
// uint32_t chan13 : 11;
// uint32_t chan14 : 11;
// uint32_t chan15 : 11;
// uint8_t   flags;
// uint8_t   endbyte;
//} sbusPacket_t;

//sbusPacket_t sbus;
//memcpy((uint8_t *)&sbus,Remote_Re_Date,25);
/**********************************************
串口配置函数   遥控器M.BUS协议
配置了串口5，Pc12 Pd2
波特率：100000,
数据位：8b，
停止位：2，
校验：N
接收中断使能*/

//**********************************************/


	
	

void UART5_Init(u32 bound)
{
  GPIO_InitTypeDef gpio_initstructure;
  USART_InitTypeDef usart_initstructure;
  //UART_ClockInitTypeDef usart_clockinitstructure;	

	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOD时钟
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//使能UART5时钟
 

	USART_DeInit(UART5);  //复位串口3
  
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);	
  /* C12 USART5_Tx */   
  gpio_initstructure.GPIO_Pin = GPIO_Pin_12;
  gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
  gpio_initstructure.GPIO_OType = GPIO_OType_PP;
  gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_initstructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &gpio_initstructure);
  
  /* D2 USART5_Rx  */
  gpio_initstructure.GPIO_Pin = GPIO_Pin_2;
  gpio_initstructure.GPIO_Mode = GPIO_Mode_AF;
  //gpio_initstructure.GPIO_OType = GPIO_OType_PP;
  gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &gpio_initstructure);

  usart_initstructure.USART_BaudRate = bound;									//波特率是100000
  usart_initstructure.USART_WordLength = USART_WordLength_9b;   //9位数据位
  usart_initstructure.USART_StopBits = USART_StopBits_2;				//2个停止位
  usart_initstructure.USART_Parity =USART_Parity_Even;					//偶校验
  usart_initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件控制
  usart_initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART5, &usart_initstructure);
  USART_Cmd(UART5, ENABLE);
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	
	
	
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//串口5中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}

void UART5_SendOneByte(u8 dat)
{
    while((UART5->SR&0X40)==0) {}; //循环发送,直到发送完毕
    UART5->DR = (u8) dat;
    while(USART_GetFlagStatus(UART5,USART_FLAG_TC) == RESET);
}

void UART5_IRQHandler(void)                	//串口5中断服务程序
{
	Mbus_Time_Count_Flag=1;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  			// 空闲中断
    {
				 Remote_Re_Date[Remote_Re_Count]=USART_ReceiveData(UART5);
				 Remote_Re_Count++;
				 if(Remote_Re_Date[0]==0x0F)
				{
						if(Remote_Re_Count>24)
						{
									Remote_Re_Count=0;
									Remote_Re_FLAG=1;
						}
				}
				else
				{
						Remote_Re_Count=0;
				}
		    USART_ClearITPendingBit(UART5, USART_IT_RXNE);
	}
	if(USART_GetITStatus(UART5,USART_IT_TXE)==SET)
  {
    USART_ClearITPendingBit(UART5,USART_IT_TXE);
  }
}




/*通过M.BUS获取遥控器各通道实时数据*/
void Get_Remote_channel_Date(void)
{
	   if(Mbus_Time_Count_Flag==1)//表示已经接入MBUS,如果没有接入，就不进入中断。或者长时间没进入中断，表示断线。
		 {
			 if(Remote_Re_FLAG==1)//接收到完整的一帧数据
			 {
				 switch(Remote_Re_Date[23])//判断是否关机
				 {
					 /*开机状态*/
					 case 0x00: 
										if(Remote_Re_Date[0]==0x0f)//表示
										{
													MBUS_CH[1] =  ((int16_t)(Remote_Re_Date[1])>>0 |((int16_t)(Remote_Re_Date[2]) <<8))&0x07FF;
													MBUS_CH[2] =  ((int16_t)(Remote_Re_Date[2])>>3 |((int16_t)(Remote_Re_Date[3]) <<5))&0x07FF;
													MBUS_CH[3] =  ((int16_t)(Remote_Re_Date[3])>>6 |((int16_t)(Remote_Re_Date[4]) <<2)|(int16_t)(Remote_Re_Date[5]<<10))&0x07FF;
													MBUS_CH[4] =  ((int16_t)(Remote_Re_Date[5])>>1 |((int16_t)(Remote_Re_Date[6]) <<7))&0x07FF;
													MBUS_CH[5] =  ((int16_t)(Remote_Re_Date[6])>>4 |((int16_t)(Remote_Re_Date[7]) <<4))&0x07FF;
													MBUS_CH[6] =  ((int16_t)(Remote_Re_Date[7])>>7 |((int16_t)(Remote_Re_Date[8]) <<1)|(int16_t)(Remote_Re_Date[9]<<9))&0x07FF;
													MBUS_CH[7] =  ((int16_t)(Remote_Re_Date[9])>>2 |((int16_t)(Remote_Re_Date[10])<<6))&0x07FF;
													MBUS_CH[8] =  ((int16_t)(Remote_Re_Date[10])>>5|((int16_t)(Remote_Re_Date[11])<<3))&0x07FF;
													MBUS_CH[9] =  ((int16_t)(Remote_Re_Date[12])>>0|((int16_t)(Remote_Re_Date[13])<<8))&0x07FF;
													MBUS_CH[10]=  ((int16_t)(Remote_Re_Date[13])>>3|((int16_t)(Remote_Re_Date[14])<<5))&0x07FF;
													MBUS_CH[11]=  ((int16_t)(Remote_Re_Date[14])>>6|((int16_t)(Remote_Re_Date[15])<<2)|(int16_t)(Remote_Re_Date[16]<<10))&0x07FF;
													MBUS_CH[12]=  ((int16_t)(Remote_Re_Date[16])>>1|((int16_t)(Remote_Re_Date[17])<<7))&0x07FF;
													MBUS_CH[13]=  ((int16_t)(Remote_Re_Date[17])>>4|((int16_t)(Remote_Re_Date[18])<<4))&0x07FF;
													MBUS_CH[14]=  ((int16_t)(Remote_Re_Date[18])>>7|((int16_t)(Remote_Re_Date[19])<<1)|(int16_t)(Remote_Re_Date[20]<<9))&0x07FF;
													MBUS_CH[15]=  ((int16_t)(Remote_Re_Date[20])>>2|((int16_t)(Remote_Re_Date[21])<<6))&0x07FF;
													MBUS_CH[16]=  ((int16_t)(Remote_Re_Date[21])>>5|((int16_t)(Remote_Re_Date[22])<<3))&0x07FF;
													z_speed=MBUS_CH[1];
													x_speed=MBUS_CH[2];
													y_speed=MBUS_CH[4];
													k_number=MBUS_CH[3];
													
													key5=MBUS_CH[5];    //三个挡位，第五通道
													key6=MBUS_CH[6];    //两个挡位，第六通道
													
													MBUS_ON_OFF=Remote_Re_Date[23];
													
													Remote_Re_Date[0]=1;
													Remote_Re_FLAG=0;
													Remote_Re_Date[0]=0x00;Remote_Re_Date[1]=0x00;Remote_Re_Date[2]=0x00;Remote_Re_Date[3]=0x00;
										}
										else if((Remote_Re_Date[0]==0x00)&&(Remote_Re_Date[1]==0x00))
										{
													z_speed=1000;
													x_speed=1000;
													y_speed=1000;
													k_number=1000;//可以以此来判断遥控器是否关机	
										}
											break;
					 /*关机状态*/
					 case 0x10:     z_speed=1000;
													x_speed=1000;
													y_speed=1000;
													k_number=MBUS_CH[3];//因为K值不是自动归中，所以当遥控器关机时，
																							//不应该置为中值，应该是遥控器关机前的最后的状态值。
													
													key5=MBUS_CH[5];    //三个挡位，第五通道
													key6=MBUS_CH[6];    //两个挡位，第六通道
										
											break;
					 default:break;	 
						}
					} 
		 }
		 else	//被断开了
		 {
			  Remote_Re_Date[23]=0x10;//当线断开则强制关机模式
			 	MBUS_ON_OFF=Remote_Re_Date[23];
				z_speed=0;
				x_speed=0;
				y_speed=0;
				k_number=0;//可以以此来判断遥控器是否关机	
		 }
}

