#include "angle_speed.h" 
#include "usart.h" 
#include "usart2.h" 
#include "usart3.h" 
#include "uart5.h" 
#include "math.h"

/*******************************************************
**										    前                          **
**									O--29--|--29--O									  **
**												 |													**
**												66													**
**												 |												  **
**					O-----70-------|------70-----0						**
**												11													**
**-----------------------中*线------------------------**
**												 |													**
**												轴													**
**												线													**
**												 |													**
**												77													**
**												 |													**
**							O---51-----|-----51---O								**
********************************************************/

#define PI 3.1415926  //圆周率
#define Sa 0.29 			//两前轮到轴线的垂直距离 单位/米
#define Sb 0.70 			//中间轮到轴线的垂直距离 单位/米
#define Sc 0.51 			//两后轮到轴线的垂直距离 单位/米

#define Sda 0.77 			//前轮到中线的垂直距离 单位/米
#define Sdb 0.11 			//中轮到中线的垂直距离 单位/米
#define Sdc 0.77 			//后轮到中线的垂直距离 单位/米

#define Ra sqrtf((float)(Sda*Sda+Sa*Sa))//0.8225
#define Rb sqrtf((float)(Sdb*Sdb+Sb*Sb))//0.7085
#define Rc sqrtf((float)(Sdc*Sdc+Sc*Sc))//0.9235
	

#define Angle_to_NUM  651.89866  //4096÷360*180÷3.1415926

//#define Radian_to_NUM 11.377778  //4096÷360 //角度值 转化为 角度目标值



#define SteeringMotor_MidNum 2048               //转向器的中值
#define MBUS_MAX       800  										//遥控器数据（最大/最小）对中间值的最大差值
#define WHEEL_R        0.32  						        //轮子的直径是   0.32     米 
#define WHEEL_L        WHEEL_R*PI 			        //轮子的周长是   0.32*PI  米
#define SPEED_MAX      1.5 							        //最大速度是     1.5米/秒

#define SPEED_CNT_MAX  (SPEED_MAX/WHEEL_L)*60   //每分钟转的最大数值
#define SPEED_CNT      SPEED_CNT_MAX/MBUS_MAX;  //MBUS每1个单位对应的速度值的大小

//u16 SPEED_CNT;
float turn_R;

/*控制整机小车速度*/

u8 mm=0;
extern u16 x_speed;
extern u16 y_speed;
extern u16 z_speed;
extern u16 k_number;
extern u8 MBUS_ON_OFF; //遥控器开关机状态

float Dir_r=0; //转弯的斜率

int16_t d_x=0;//遥控器x轴的值
int16_t d_y=0;//遥控器y轴的值
int16_t d_z=0;//遥控器z轴的值

extern s16 motor1_speed;//1号电机速度
extern s16 motor2_speed;//2号电机速度
extern s16 motor3_speed;//3号电机速度
extern s16 motor4_speed;//4号电机速度
extern s16 motor5_speed;//5号电机速度
extern s16 motor6_speed;//6号电机速度
extern s16 motor7_speed;//7号电机速度
extern s16 motor8_speed;//8号电机速度


extern u16 Position1_Data;
extern u16 Position2_Data;
extern u16 Position3_Data;
extern u16 Position4_Data;
extern u16 Position5_Data;
extern u16 Position6_Data;

extern u8 PC_Re_FLAG;		 //接受完成标志位

extern float PC_x_speed;
extern float PC_y_speed;
extern float PC_z_speed;
extern u16 MBUS_CH[17];			//遥控器16个通道的实时数值，实际使用只用了6个通道
extern u16 key5,key6;//遥控器的第5、6通道，
u8 PC_ReTime_flag=0;
extern u8 PC_ReTime_stop_flag;
float speed_abs(float x)
{
	float y;
	y=x;
		if(y>0) y=y;
		else y=-y;
		return y;
}


/*遥控器值的大小转换成实际电机转速大小和舵机转向值的大小*/
void MBUS_TO_MotorSpeed(void)
{
	 
		if((x_speed==0)&&(y_speed==0)&&(z_speed==0)&&(k_number==0))//先开主机，后开遥控器，不可让机器行走。
		{
				motor1_speed=0;motor2_speed=0;motor3_speed=0;motor4_speed=0;motor5_speed=0;motor6_speed=0;
				Position1_Data=SteeringMotor_MidNum;
				Position2_Data=SteeringMotor_MidNum;
				Position3_Data=SteeringMotor_MidNum;
				Position4_Data=SteeringMotor_MidNum;
				Position5_Data=SteeringMotor_MidNum;
				Position6_Data=SteeringMotor_MidNum;
		}
		else if(speed_abs(y_speed-1000)>20)
		{
								d_y=y_speed-1000;
								motor1_speed =d_y*SPEED_CNT;
								motor2_speed =d_y*SPEED_CNT;
								motor3_speed =d_y*SPEED_CNT;
								motor4_speed =d_y*SPEED_CNT;
								motor5_speed =d_y*SPEED_CNT;
								motor6_speed =d_y*SPEED_CNT;
			
								Position1_Data=1024;
								Position2_Data=1024;
								Position3_Data=1024;
								Position4_Data=3072;
								Position5_Data=3072;
								Position6_Data=3072;
		} //直行
		else  if((speed_abs(y_speed-1000)<20))
		{
				if((speed_abs(x_speed-1000)>20)&&((speed_abs(z_speed-1000))<20))//x有速度  z为0
				{
								d_x=x_speed-1000;
								motor1_speed =-d_x*SPEED_CNT;
								motor2_speed =-d_x*SPEED_CNT;
								motor3_speed =-d_x*SPEED_CNT;
								motor4_speed =d_x*SPEED_CNT;
								motor5_speed =d_x*SPEED_CNT;
								motor6_speed =d_x*SPEED_CNT;
								
								Position1_Data=SteeringMotor_MidNum;
								Position2_Data=SteeringMotor_MidNum;
								Position3_Data=SteeringMotor_MidNum;
								Position4_Data=SteeringMotor_MidNum;
								Position5_Data=SteeringMotor_MidNum;
								Position6_Data=SteeringMotor_MidNum;
					}
				 //车体自传，无前进后退速度
					else if((speed_abs(x_speed-1000)<20)&&((speed_abs(z_speed-1000))>20))//x无速度，Z有速度
					{
							d_z=1000-z_speed;
						
							motor1_speed=(Ra/Rb)*d_z*SPEED_CNT;
							motor2_speed=d_z*SPEED_CNT;
							motor3_speed=(Rc/Rb)*d_z*SPEED_CNT;
						
							motor4_speed=(Ra/Rb)*d_z*SPEED_CNT;
							motor5_speed=d_z*SPEED_CNT;
							motor6_speed=(Rc/Rb)*d_z*SPEED_CNT;
						
						
							Position1_Data=1024+(u16)(atan2(Sa,Sda)*Angle_to_NUM);
							Position2_Data=1024+(u16)(atan2(Sb,Sdb)*Angle_to_NUM);
							Position3_Data=3072-(u16)(atan2(Sc,Sdc)*Angle_to_NUM);
							Position4_Data=3072-(u16)(atan2(Sa,Sda)*Angle_to_NUM);
							Position5_Data=3072-(u16)(atan2(Sb,Sdb)*Angle_to_NUM);
							Position6_Data=1024+(u16)(atan2(Sc,Sdc)*Angle_to_NUM);
					}
					else if((speed_abs(x_speed-1000)>20)&&((speed_abs(z_speed-1000))>20))//x有速度，Z有速度
					{
							/*
									遥控遥杆象限说明
											1、第一象限和第四象限：电机方向相反，舵机方向相同
											2、第二象限和第三象限：电机方向相反，舵机方向相同
											3、第一四象限和第二三象限舵机方向需要分别解算
											4、速度只跟半径相关
											5、速度方向跟遥杆的象限相关，跟其他没有任何关系	
							*/
								d_x=x_speed-1000;
								d_z=1000-z_speed;
								/*********************遥控器遥感的第一象限***********************************************/
								if((d_x>0)&&(d_z>0))
								{
									turn_R=(float)d_x/(float)d_z;//定义  X的分量除以Z的分量为半径
									
									motor1_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R-(float)Sa)*speed_abs(turn_R-(float)Sa)))))*SPEED_CNT;
									motor2_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R-(float)Sb)*speed_abs(turn_R-(float)Sb)))))*SPEED_CNT;
									motor3_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R-(float)Sc)*speed_abs(turn_R-(float)Sc)))))*SPEED_CNT;
									motor4_speed= (speed_abs((float)d_z)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R+(float)Sa)*speed_abs(turn_R+(float)Sa)))))*SPEED_CNT;
									motor5_speed= (speed_abs((float)d_z)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R+(float)Sb)*speed_abs(turn_R+(float)Sb)))))*SPEED_CNT;
									motor6_speed= (speed_abs((float)d_z)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R+(float)Sc)*speed_abs(turn_R+(float)Sc)))))*SPEED_CNT;
									
															
									if(turn_R>(float)Sb)	//当前行驶的轨迹在车体之外    b1>c1>a1
									{
											Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R-(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
										
											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
									}
									if((turn_R<=(float)Sb)&&(turn_R>(float)Sc))	//当前行驶的圆心在车体内，介于 后轮对中线的垂点 和 中轮对中线的垂点 之间
									{  
											Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position3_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
										
											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
									}
									if(turn_R<=(float)Sc)//&&(turn_R>(float)Sa))	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和 后轮对中线的垂点 之间
									{  
											Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position3_Data=     (uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
									
											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
									}
//									if(turn_R<=(float)Sa)	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和中点 之间
//									{  
//											Position1_Data=4096-(uint16_t)(atan2f((float)Sda,(float)((float)Sa)-turn_R)*(float)Angle_to_NUM);
//											Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
//											Position3_Data=     (uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
//										
//											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
//											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
//											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
//									}
									if(Position1_Data>=3072)Position1_Data=3072;
									if(Position2_Data>=3072)Position2_Data=3072;
									if(Position3_Data<=1024)Position3_Data=1024;
									if(Position4_Data>=3072)Position4_Data=3072;
									if(Position5_Data>=3072)Position5_Data=3072;
									if(Position6_Data<=1024)Position6_Data=1024;
								}
								/*********************遥控器遥感的第四象限***********************************************/
								if((d_x<0)&&(d_z>0))
								{
									turn_R=-(float)d_x/(float)d_z;//定义  X的分量除以Z的分量为半径
									
									motor1_speed=(speed_abs((float)d_z)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R-(float)Sa)*speed_abs(turn_R-(float)Sa)))))*SPEED_CNT;
									motor2_speed=(speed_abs((float)d_z)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R-(float)Sb)*speed_abs(turn_R-(float)Sb)))))*SPEED_CNT;
									motor3_speed=(speed_abs((float)d_z)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R-(float)Sc)*speed_abs(turn_R-(float)Sc)))))*SPEED_CNT;
									motor4_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R+(float)Sa)*speed_abs(turn_R+(float)Sa)))))*SPEED_CNT;
									motor5_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R+(float)Sb)*speed_abs(turn_R+(float)Sb)))))*SPEED_CNT;
									motor6_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R+(float)Sc)*speed_abs(turn_R+(float)Sc)))))*SPEED_CNT;
									

									if(turn_R>(float)Sb)	//当前行驶的轨迹在车体之外    b1>c1>a1
									{
											Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R-(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
									
											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
									}
									if((turn_R<=(float)Sb)&&(turn_R>(float)Sc))	//当前行驶的圆心在车体内，介于 后轮对中线的垂点 和 中轮对中线的垂点 之间
									{  
											Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position3_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
										
											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
									}
									if(turn_R<=(float)Sc)//&&(turn_R>(float)Sa))	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和 后轮对中线的垂点 之间
									{  
											Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position3_Data=     (uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
									
											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
									}
									if(Position1_Data>=3072)Position1_Data=3072;
									if(Position2_Data>=3072)Position2_Data=3072;
									if(Position3_Data<=1024)Position3_Data=1024;
									if(Position4_Data>=3072)Position4_Data=3072;
									if(Position5_Data>=3072)Position5_Data=3072;
									if(Position6_Data<=1024)Position6_Data=1024;
//									if(turn_R<=(float)Sa)	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和中点 之间
//									{  
//											Position1_Data=4096-(uint16_t)(atan2f((float)Sda,(float)((float)Sa)-turn_R)*(float)Angle_to_NUM);
//											Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
//											Position3_Data=     (uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
//									
//											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
//											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
//											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
//									}
								}
								/*********************遥控器遥感的第二象限***********************************************/
								if((d_x>0)&&(d_z<0))
								{
									 turn_R=-(float)d_x/(float)d_z;//定义  X的分量除以Z的分量为半径
									
									motor1_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R+(float)Sa)*speed_abs(turn_R+(float)Sa)))))*SPEED_CNT;
									motor2_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R+(float)Sb)*speed_abs(turn_R+(float)Sb)))))*SPEED_CNT;
									motor3_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R+(float)Sc)*speed_abs(turn_R+(float)Sc)))))*SPEED_CNT;
									motor4_speed=(speed_abs((float)d_z)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R-(float)Sa)*speed_abs(turn_R-(float)Sa)))))*SPEED_CNT;
									motor5_speed=(speed_abs((float)d_z)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R-(float)Sb)*speed_abs(turn_R-(float)Sb)))))*SPEED_CNT;
									motor6_speed=(speed_abs((float)d_z)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R-(float)Sc)*speed_abs(turn_R-(float)Sc)))))*SPEED_CNT;

									
									
									if(turn_R>(float)Sb)	//当前行驶的轨迹在车体之外    b1>c1>a1
									{
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
										
											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R-(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
									}
									if((turn_R<(float)Sb)&&(turn_R>(float)Sc))	//当前行驶的圆心在车体内，介于 后轮对中线的垂点 和 中轮对中线的垂点 之间
									{  
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
											
										
											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=			(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position6_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
									}
									if(turn_R<(float)Sc)//&&(turn_R>(float)Sa))	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和 后轮对中线的垂点 之间
									{  
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);

											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=			(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position6_Data=4096-(uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
									}
//									if(turn_R<(float)Sa)	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和中点 之间
//									{  
//											
//											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
//											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
//											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
//									
//											Position4_Data=     (uint16_t)(atan2f((float)Sda,(float)((float)Sa)-turn_R)*(float)Angle_to_NUM);
//											Position5_Data=     (uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
//											Position6_Data=4096-(uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
//									}
								  if(Position1_Data<=1024)Position1_Data=1024;
									if(Position2_Data>=3072)Position2_Data=3072;
									if(Position3_Data>=3072)Position3_Data=3072;
									if(Position4_Data<=1024)Position4_Data=1024;
									if(Position5_Data<=1024)Position5_Data=1024;
									if(Position6_Data>=3072)Position6_Data=3072;
								}
								/*********************遥控器遥感的第三象限***********************************************/
								if((d_x<0)&&(d_z<0))
								{

									 turn_R=(float)d_x/(float)d_z;//定义  X的分量除以Z的分量为半径
									
									motor1_speed=(speed_abs((float)d_z)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R+(float)Sa)*speed_abs(turn_R+(float)Sa)))))*SPEED_CNT;
									motor2_speed=(speed_abs((float)d_z)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R+(float)Sb)*speed_abs(turn_R+(float)Sb)))))*SPEED_CNT;
									motor3_speed=(speed_abs((float)d_z)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R+(float)Sc)*speed_abs(turn_R+(float)Sc)))))*SPEED_CNT;
									motor4_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R-(float)Sa)*speed_abs(turn_R-(float)Sa)))))*SPEED_CNT;
									motor5_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R-(float)Sb)*speed_abs(turn_R-(float)Sb)))))*SPEED_CNT;
									motor6_speed=-(speed_abs((float)d_z)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R-(float)Sc)*speed_abs(turn_R-(float)Sc)))))*SPEED_CNT;
									
									if(turn_R>(float)Sb)	//当前行驶的轨迹在车体之外    b1>c1>a1
									{
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
										
											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R-(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
									}
									if((turn_R<(float)Sb)&&(turn_R>(float)Sc))	//当前行驶的圆心在车体内，介于 后轮对中线的垂点 和 中轮对中线的垂点 之间
									{  
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
										 
										
											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=			(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position6_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
									}
									if(turn_R<(float)Sc)//&&(turn_R>(float)Sa))	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和 后轮对中线的垂点 之间
									{  
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=			(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position6_Data=4096-(uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
									}
//									if(turn_R<(float)Sa)	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和中点 之间
//									{  
//											
//											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
//											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
//											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
//										
//											Position4_Data=     (uint16_t)(atan2f((float)Sda,(float)((float)Sa)-turn_R)*(float)Angle_to_NUM);
//											Position5_Data=     (uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
//											Position6_Data=4096-(uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
//									}
								
								  if(Position1_Data<=1024)Position1_Data=1024;
									if(Position2_Data>=3072)Position2_Data=3072;
									if(Position3_Data>=3072)Position3_Data=3072;
									if(Position4_Data<=1024)Position4_Data=1024;
									if(Position5_Data<=1024)Position5_Data=1024;
									if(Position6_Data>=3072)Position6_Data=3072;
								}
						}
						else if((speed_abs(x_speed-1000)<=20)&&((speed_abs(z_speed-1000))<=20))//遥感位置归零
						{
								motor1_speed=0;motor2_speed=0;motor3_speed=0;motor4_speed=0;motor5_speed=0;motor6_speed=0;
								Position1_Data=SteeringMotor_MidNum;
								Position2_Data=SteeringMotor_MidNum;
								Position3_Data=SteeringMotor_MidNum;
								Position4_Data=SteeringMotor_MidNum;
								Position5_Data=SteeringMotor_MidNum;
								Position6_Data=SteeringMotor_MidNum;
						}
						else
						{	
								motor1_speed=0;motor2_speed=0;motor3_speed=0;motor4_speed=0;motor5_speed=0;motor6_speed=0;
						}
				}
				
}

void PC_TO_MotorSpeed(void)
{
	  if((PC_x_speed==0)&&(PC_y_speed==0)&&(PC_z_speed==0))
		{
				motor1_speed=0;motor2_speed=0;motor3_speed=0;motor4_speed=0;motor5_speed=0;motor6_speed=0;
				Position1_Data=SteeringMotor_MidNum;
				Position2_Data=SteeringMotor_MidNum;
				Position3_Data=SteeringMotor_MidNum;
				Position4_Data=SteeringMotor_MidNum;
				Position5_Data=SteeringMotor_MidNum;
				Position6_Data=SteeringMotor_MidNum;
		}
		/*X轴直行*/
		else if((PC_x_speed!=0)&&(PC_y_speed==0)&&(PC_z_speed==0))//车体速度
		{
				motor1_speed =-PC_x_speed*300;
				motor2_speed =-PC_x_speed*300;
				motor3_speed =-PC_x_speed*300;
				motor4_speed =PC_x_speed*300;
				motor5_speed =PC_x_speed*300;
				motor6_speed =PC_x_speed*300;
			 
			  Position1_Data=SteeringMotor_MidNum;
				Position2_Data=SteeringMotor_MidNum;
				Position3_Data=SteeringMotor_MidNum;
				Position4_Data=SteeringMotor_MidNum;
				Position5_Data=SteeringMotor_MidNum;
				Position6_Data=SteeringMotor_MidNum;
		}
		/*Z轴自转*/
		else if((PC_x_speed==0)&&(PC_y_speed==0)&&(PC_z_speed!=0))//
		{
	
				motor1_speed=(Ra/Rb)*PC_z_speed*300;
				motor2_speed=PC_z_speed*300;
				motor3_speed=(Rc/Rb)*PC_z_speed*300;
			
				motor4_speed=(Rc/Rb)*PC_z_speed*300;
				motor5_speed=PC_z_speed*300;
				motor6_speed=(Rc/Rb)*PC_z_speed*300;
		
				Position1_Data=1024+(u16)(atan2(Sa,Sda)*Angle_to_NUM);
				Position2_Data=1024+(u16)(atan2(Sb,Sdb)*Angle_to_NUM);
				Position3_Data=3072-(u16)(atan2(Sc,Sdc)*Angle_to_NUM);
				Position4_Data=3072-(u16)(atan2(Sa,Sda)*Angle_to_NUM);
				Position5_Data=3072-(u16)(atan2(Sb,Sdb)*Angle_to_NUM);
				Position6_Data=1024+(u16)(atan2(Sc,Sdc)*Angle_to_NUM);
				
				
		}
		//x、Z不为0，Y为0
		else if((PC_x_speed!=0)&&(PC_y_speed==0)&&(PC_z_speed!=0))//车体速度
		{
			
			if((PC_x_speed>0)&&(PC_z_speed>0))
			{
				turn_R=(float)PC_x_speed/(float)PC_z_speed;//定义  X的分量除以Z的分量为半径
				motor1_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R-(float)Sa)*speed_abs(turn_R-(float)Sa)))))*300;
				motor2_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R-(float)Sb)*speed_abs(turn_R-(float)Sb)))))*300;
				motor3_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R-(float)Sc)*speed_abs(turn_R-(float)Sc)))))*300;
				motor4_speed= (speed_abs((float)PC_z_speed)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R+(float)Sa)*speed_abs(turn_R+(float)Sa)))))*300;
				motor5_speed= (speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R+(float)Sb)*speed_abs(turn_R+(float)Sb)))))*300;
				motor6_speed= (speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R+(float)Sc)*speed_abs(turn_R+(float)Sc)))))*300;
				
										
				if(turn_R>(float)Sb)	//当前行驶的轨迹在车体之外    b1>c1>a1
				{
						Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
						Position2_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R-(float)Sb))*(float)Angle_to_NUM);
						Position3_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
					
						Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
						Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
						Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
				}
				if((turn_R<=(float)Sb)&&(turn_R>(float)Sc))	//当前行驶的圆心在车体内，介于 后轮对中线的垂点 和 中轮对中线的垂点 之间
				{  
						Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
						Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
						Position3_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
					
						Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
						Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
						Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
				}
				if(turn_R<=(float)Sc)//&&(turn_R>(float)Sa))	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和 后轮对中线的垂点 之间
				{  
						Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
						Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
						Position3_Data=     (uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
				
						Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
						Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
						Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
				}
//									if(turn_R<=(float)Sa)	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和中点 之间
//									{  
//											Position1_Data=4096-(uint16_t)(atan2f((float)Sda,(float)((float)Sa)-turn_R)*(float)Angle_to_NUM);
//											Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
//											Position3_Data=     (uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
//										
//											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
//											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
//											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
//									}
				if(Position1_Data>=3072)Position1_Data=3072;
				if(Position2_Data>=3072)Position2_Data=3072;
				if(Position3_Data<=1024)Position3_Data=1024;
				if(Position4_Data>=3072)Position4_Data=3072;
				if(Position5_Data>=3072)Position5_Data=3072;
				if(Position6_Data<=1024)Position6_Data=1024;
			}
			/*********************遥控器遥感的第四象限***********************************************/
								if((PC_x_speed<0)&&(PC_z_speed>0))
								{
									turn_R=-(float)PC_x_speed/(float)PC_z_speed;//定义  X的分量除以Z的分量为半径
									
									motor1_speed=(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R-(float)Sa)*speed_abs(turn_R-(float)Sa)))))*300;
									motor2_speed=(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R-(float)Sb)*speed_abs(turn_R-(float)Sb)))))*300;
									motor3_speed=(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R-(float)Sc)*speed_abs(turn_R-(float)Sc)))))*300;
									motor4_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R+(float)Sa)*speed_abs(turn_R+(float)Sa)))))*300;
									motor5_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R+(float)Sb)*speed_abs(turn_R+(float)Sb)))))*300;
									motor6_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R+(float)Sc)*speed_abs(turn_R+(float)Sc)))))*300;
									

									if(turn_R>(float)Sb)	//当前行驶的轨迹在车体之外    b1>c1>a1
									{
											Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R-(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
									
											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
									}
									if((turn_R<=(float)Sb)&&(turn_R>(float)Sc))	//当前行驶的圆心在车体内，介于 后轮对中线的垂点 和 中轮对中线的垂点 之间
									{  
											Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position3_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
										
											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
									}
									if(turn_R<=(float)Sc)//&&(turn_R>(float)Sa))	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和 后轮对中线的垂点 之间
									{  
											Position1_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position3_Data=     (uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
									
											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
									}
									if(Position1_Data>=3072)Position1_Data=3072;
									if(Position2_Data>=3072)Position2_Data=3072;
									if(Position3_Data<=1024)Position3_Data=1024;
									if(Position4_Data>=3072)Position4_Data=3072;
									if(Position5_Data>=3072)Position5_Data=3072;
									if(Position6_Data<=1024)Position6_Data=1024;
//									if(turn_R<=(float)Sa)	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和中点 之间
//									{  
//											Position1_Data=4096-(uint16_t)(atan2f((float)Sda,(float)((float)Sa)-turn_R)*(float)Angle_to_NUM);
//											Position2_Data=4096-(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
//											Position3_Data=     (uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
//									
//											Position4_Data=2048+(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
//											Position5_Data=2048+(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
//											Position6_Data=2048-(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
//									}
								}
								/*********************遥控器遥感的第二象限***********************************************/
								if((PC_x_speed>0)&&(PC_z_speed<0))
								{
									 turn_R=-(float)PC_x_speed/(float)PC_z_speed;//定义  X的分量除以Z的分量为半径
									
									motor1_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R+(float)Sa)*speed_abs(turn_R+(float)Sa)))))*300;
									motor2_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R+(float)Sb)*speed_abs(turn_R+(float)Sb)))))*300;
									motor3_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R+(float)Sc)*speed_abs(turn_R+(float)Sc)))))*300;
									motor4_speed=(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R-(float)Sa)*speed_abs(turn_R-(float)Sa)))))*300;
									motor5_speed=(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R-(float)Sb)*speed_abs(turn_R-(float)Sb)))))*300;
									motor6_speed=(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R-(float)Sc)*speed_abs(turn_R-(float)Sc)))))*300;

									
									
									if(turn_R>(float)Sb)	//当前行驶的轨迹在车体之外    b1>c1>a1
									{
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
										
											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R-(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
									}
									if((turn_R<(float)Sb)&&(turn_R>(float)Sc))	//当前行驶的圆心在车体内，介于 后轮对中线的垂点 和 中轮对中线的垂点 之间
									{  
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
											
										
											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=			(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position6_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
									}
									if(turn_R<(float)Sc)//&&(turn_R>(float)Sa))	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和 后轮对中线的垂点 之间
									{  
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);

											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=			(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position6_Data=4096-(uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
									}
//									if(turn_R<(float)Sa)	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和中点 之间
//									{  
//											
//											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
//											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
//											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
//									
//											Position4_Data=     (uint16_t)(atan2f((float)Sda,(float)((float)Sa)-turn_R)*(float)Angle_to_NUM);
//											Position5_Data=     (uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
//											Position6_Data=4096-(uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
//									}
								  if(Position1_Data<=1024)Position1_Data=1024;
									if(Position2_Data>=3072)Position2_Data=3072;
									if(Position3_Data>=3072)Position3_Data=3072;
									if(Position4_Data<=1024)Position4_Data=1024;
									if(Position5_Data<=1024)Position5_Data=1024;
									if(Position6_Data>=3072)Position6_Data=3072;
								}
								/*********************遥控器遥感的第三象限***********************************************/
								if((PC_x_speed<0)&&(PC_z_speed<0))
								{

									 turn_R=(float)PC_x_speed/(float)PC_z_speed;//定义  X的分量除以Z的分量为半径
									
									motor1_speed=(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R+(float)Sa)*speed_abs(turn_R+(float)Sa)))))*300;
									motor2_speed=(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R+(float)Sb)*speed_abs(turn_R+(float)Sb)))))*300;
									motor3_speed=(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R+(float)Sc)*speed_abs(turn_R+(float)Sc)))))*300;
									motor4_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sda)*(Sda)+speed_abs(turn_R-(float)Sa)*speed_abs(turn_R-(float)Sa)))))*300;
									motor5_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdb)*(Sdb)+speed_abs(turn_R-(float)Sb)*speed_abs(turn_R-(float)Sb)))))*300;
									motor6_speed=-(speed_abs((float)PC_z_speed)*(sqrtf((float)((Sdc)*(Sdc)+speed_abs(turn_R-(float)Sc)*speed_abs(turn_R-(float)Sc)))))*300;
									
									if(turn_R>(float)Sb)	//当前行驶的轨迹在车体之外    b1>c1>a1
									{
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
										
											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R-(float)Sb))*(float)Angle_to_NUM);
											Position6_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
									}
									if((turn_R<(float)Sb)&&(turn_R>(float)Sc))	//当前行驶的圆心在车体内，介于 后轮对中线的垂点 和 中轮对中线的垂点 之间
									{  
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
										 
										
											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=			(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position6_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R-(float)Sc))*(float)Angle_to_NUM);
									}
									if(turn_R<(float)Sc)//&&(turn_R>(float)Sa))	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和 后轮对中线的垂点 之间
									{  
											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
											Position4_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R-(float)Sa))*(float)Angle_to_NUM);
											Position5_Data=			(uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
											Position6_Data=4096-(uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
									}
//									if(turn_R<(float)Sa)	//当前行驶的圆心在车体内，介于 前轮对中线的垂点 和中点 之间
//									{  
//											
//											Position1_Data=2048-(uint16_t)(atan2f((float)Sda,(float)(turn_R+(float)Sa))*(float)Angle_to_NUM);
//											Position2_Data=2048-(uint16_t)(atan2f((float)Sdb,(float)(turn_R+(float)Sb))*(float)Angle_to_NUM);
//											Position3_Data=2048+(uint16_t)(atan2f((float)Sdc,(float)(turn_R+(float)Sc))*(float)Angle_to_NUM);
//										
//											Position4_Data=     (uint16_t)(atan2f((float)Sda,(float)((float)Sa)-turn_R)*(float)Angle_to_NUM);
//											Position5_Data=     (uint16_t)(atan2f((float)Sdb,(float)((float)Sb)-turn_R)*(float)Angle_to_NUM);
//											Position6_Data=4096-(uint16_t)(atan2f((float)Sdc,(float)((float)Sc)-turn_R)*(float)Angle_to_NUM);
//									}
							
								  if(Position1_Data<=1024)Position1_Data=1024;
									if(Position2_Data>=3072)Position2_Data=3072;
									if(Position3_Data>=3072)Position3_Data=3072;
									if(Position4_Data<=1024)Position4_Data=1024;
									if(Position5_Data<=1024)Position5_Data=1024;
									if(Position6_Data>=3072)Position6_Data=3072;	
								}
		}
		else if((PC_x_speed==0)&&(PC_y_speed!=0)&&(PC_z_speed==0))
		{
				motor1_speed =PC_y_speed*300;
				motor2_speed =PC_y_speed*300;
				motor3_speed =PC_y_speed*300;
				motor4_speed =PC_y_speed*300;
				motor5_speed =PC_y_speed*300;
				motor6_speed =PC_y_speed*300;

				Position1_Data=1024;
				Position2_Data=1024;
				Position3_Data=1024;
				Position4_Data=3072;
				Position5_Data=3072;
				Position6_Data=3072;
		}

}

/*所有电机速度和舵机角度值计算*/
void ALL_speed_OUT(void)
{
	  switch(MBUS_ON_OFF)
		{
			  case 0x00:/*开机状态*/  
					          PC_x_speed=0;
							  PC_y_speed=0;
				              PC_z_speed=0;
							  MBUS_TO_MotorSpeed();
								break;
				case 0x10:/*遥控器关机状态*/
									x_speed=1000;
									y_speed=1000;
									z_speed=1000;
				          k_number=MBUS_CH[3];//因为K值不是自动归中，所以当遥控器关机时，
																			//不应该置为中值，应该是遥控器关机前的最后的状态值。
									key5=MBUS_CH[5];    //三个挡位，第五通道
									key6=MBUS_CH[6];    //两个挡位，第六通道
								if(PC_Re_FLAG==1)//当有上位机数据时，则进行上位机操作。
								{
									  mm++;
										Get_PC_Date();//获得3个轴的数据
										PC_TO_MotorSpeed();
										PC_Re_FLAG=0;
								}
								else
								{
									if(PC_ReTime_stop_flag==1)//很久不发数据则置为0
									{
										motor1_speed=0;motor2_speed=0;motor3_speed=0;motor4_speed=0;motor5_speed=0;motor6_speed=0;
										Position1_Data=SteeringMotor_MidNum;
										Position2_Data=SteeringMotor_MidNum;
										Position3_Data=SteeringMotor_MidNum;
										Position4_Data=SteeringMotor_MidNum;
										Position5_Data=SteeringMotor_MidNum;
										Position6_Data=SteeringMotor_MidNum;
									}
								}
								break;
				default:motor1_speed=0;motor2_speed=0;motor3_speed=0;motor4_speed=0;motor5_speed=0;motor6_speed=0;
								Position1_Data=SteeringMotor_MidNum;
								Position2_Data=SteeringMotor_MidNum;
								Position3_Data=SteeringMotor_MidNum;
								Position4_Data=SteeringMotor_MidNum;
								Position5_Data=SteeringMotor_MidNum;
								Position6_Data=SteeringMotor_MidNum;
								break;
		
		}
}



