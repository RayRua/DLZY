#include "led.h" 


u8 Seg_Display_Flag;//数码管显示切换标志位
u8 seg_date[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};//0-9
//初始化PA6和PA7为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOF时钟

  //GPIOA4,A5初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);//GPIOA4,A5设置高，灯灭
}





//初始化PA6和PA7为输出口.并使能这两个口的时钟		    
//LED IO初始化
void DigitalTube_LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOD时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOD时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB时钟
  //GPIOD7初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOD,GPIO_Pin_7);//GPIOD7设置
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOB, GPIO_Pin_3| GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);//GPIOA4,A5设置高，灯灭

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_1);//GPIOD7设置
}

void Seg_Display(u8 x)
{
	u8 date_ge;
	u8 date_shi;
	date_ge=x%10;
	date_shi=x/10%10;
			switch(Seg_Display_Flag)
			{
				case 0:SEGS1=0;SEGS2=1;//打开第一个数码管
							 SEGA =(seg_date[date_ge]>>0)&0x01;			
							 SEGB =(seg_date[date_ge]>>1)&0x01;
							 SEGC =(seg_date[date_ge]>>2)&0x01;
							 SEGD =(seg_date[date_ge]>>3)&0x01;
							 SEGE =(seg_date[date_ge]>>4)&0x01;
							 SEGF =(seg_date[date_ge]>>5)&0x01;
							 SEGG =(seg_date[date_ge]>>6)&0x01;
							 SEGDP=(seg_date[date_ge]>>7)&0x01;
							break;
				case 1:SEGS1=1;SEGS2=0;//打开第一个数码管
							 SEGA =(seg_date[date_shi]>>0)&0x01;
							 SEGB =(seg_date[date_shi]>>1)&0x01;
							 SEGC =(seg_date[date_shi]>>2)&0x01;
							 SEGD =(seg_date[date_shi]>>3)&0x01;
							 SEGE =(seg_date[date_shi]>>4)&0x01;
							 SEGF =(seg_date[date_shi]>>5)&0x01;
							 SEGG =(seg_date[date_shi]>>6)&0x01;
							 SEGDP=(seg_date[date_shi]>>7)&0x01;
							break;
				default:break;
			}
}












////接受数据校对
/*usart1*/
//extern uint8_t PC_Re_Date[10];
//extern uint8_t PC_Re_Count;
//extern uint8_t PC_Re_FLAG;
//unsigned char sumCheck(u8 *date) 
//{
//  unsigned char i;
//  u8 checksum = 0;
//  for(i=3;i<11;i++)
//  {
//		checksum=checksum+date[i];
//  }
//	checksum=~checksum;
//  if(checksum== date[11])	  
//    return  1;
//  else 	
//    return  0;
//}

//const u8 version_number[]={"Light_Robot_V100"};//该系统的型号和版本号，待写入到STM32 FLASH的字符串数组：当改变升级该软件时，只要修改最后的数字部分进行升级即可
//#define version_number_LENTH sizeof(version_number)	 		  	//数组长度	
//#define SIZE version_number_LENTH/4+((version_number_LENTH%4)?1:0)

//#define FLASH_SAVE_ADDR  0X08040004 	//设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
//										//否则,写操作的时候,可能会导致擦除整个扇区,从而引起部分程序丢失.引起死机.

//void iap()
//{

//	uint8_t flash_first_write_falg=0;
//	if(flash_first_write_falg==0)
//	{

//		//	SCB->VTOR = FLASH_BASE | 0x10000;//设置偏移量
//			flash_first_write_falg=1;
//			STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)version_number,SIZE);//将版本号和型号写入FLASH中
//	}
//	if(PC_Re_FLAG==1)
//  {
//		 if((PC_Re_Date[0]==0xFF)&&(PC_Re_Date[1]==0xFF)&&(PC_Re_Date[2]==0xFE)
//			&&(PC_Re_Date[3]==0x55)&&(PC_Re_Date[4]==0x08)&&(PC_Re_Date[5]==0x00)
//			&&(PC_Re_Date[6]==0x00))
//			{
//					 SCB->AIRCR =0X05FA0000|(u32)0x04;//软件复位
//					 PC_Re_Date[0]=0x0A;
//					 printf("复位中...\r\n");	
//			}
//			PC_Re_FLAG=0;
//  }
//}




