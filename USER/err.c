//#include "err.h"
//#include "angle_speed.h"
//#include "usart.h"
//#include "usart2.h"
//#include "usart3.h"
//#include "uart5.h"
//#include "math.h"
//#include "led.h"

//extern u16 x_speed;
//extern u16 y_speed;
//extern u16 z_speed;
//extern u16 Position1, Position2, Position3, Position4, Position5, Position6, Position7, Position8;

//extern u16 Position1_Data;
//extern u16 Position2_Data;
//extern u16 Position3_Data;
//extern u16 Position4_Data;
//extern u16 Position5_Data;
//extern u16 Position6_Data;

//extern s16 motor1_speed;//1号电机速度
//extern s16 motor2_speed;//2号电机速度
//extern s16 motor3_speed;//3号电机速度
//extern s16 motor4_speed;//4号电机速度
//extern s16 motor5_speed;//5号电机速度
//extern s16 motor6_speed;//6号电机速度
//extern s16 motor7_speed;//7号电机速度
//extern s16 motor8_speed;//8号电机速度

//float  Position1_gap;
//float  Position2_gap;
//float  Position3_gap;
//float  Position4_gap;
//float  Position5_gap;
//float  Position6_gap;
//extern u16 err_LED;
//extern u16 err;
//u16 err_group[30];
//u16 err_signal[30];


//u16 time_err_cnt1 = 0;
//u16 time_err_cnt2 = 0;
//u16 time_err_cnt3 = 0;
//u16 time_err_cnt4 = 0;
//u16 time_err_cnt5 = 0;
//u16 time_err_cnt6 = 0;
//u16 err_flag;

//u8 p, F;
//extern u16 time_500ms_cnt;			           //5ms定时时间
//extern u16 time_500ms_flag;		           //5ms定时时间标志位

//float Position_abs(float x)
//{
//    int y;
//    y = x;
//    if(y > 0) y = y;
//    else y = -y;
//    return y;
//}



//void Faultsum(void)
//{
//    u8 i, n,
//    p = 0;
//    for(i = 1; i <= 30; i++)
//    {
//        if(err_group[i] >= 1)
//        {
//            n++;
//            err_signal[n] = i;
//            p++;
//        }
//    }
//    F = p;
//}

////void FaultDisplay(void)
////{
////		if(F>0)
////		{
////
////				switch(err)
////				{
////					case 1:  if(err_group[1]!=0x00){err_LED=1;break;}else{};
////					case 2:  if(err_group[2]!=0x00){err_LED=2;break;}else{};
////					case 3:  if(err_group[3]!=0x00){err_LED=3;break;}else{};
////					case 4:  if(err_group[4]!=0x00){err_LED=4;break;}else{};
////					case 5:  if(err_group[5]!=0x00){err_LED=5;break;}else{};
////					case 6:  if(err_group[6]!=0x00){err_LED=6;break;}else{};
////					case 7:  if(err_group[7]!=0x00){err_LED=7;break;}else{};
////					case 8:  if(err_group[8]!=0x00){err_LED=8;break;}else{};
////					case 9:  if(err_group[9]!=0x00){err_LED=9;break;}else{};
////					case 10: if(err_group[10]!=0x00){err_LED=10;break;}else{};
////					case 11: if(err_group[11]!=0x00){err_LED=11;break;}else{};
////					case 12: if(err_group[12]!=0x00){err_LED=12;break;}else{};
////					case 13: if(err_group[13]!=0x00){err_LED=13;break;}else{};
////					case 14: if(err_group[14]!=0x00){err_LED=14;break;}else{};
////					case 15: if(err_group[15]!=0x00){err_LED=15;break;}else{};
////					case 16: if(err_group[16]!=0x00){err_LED=16;break;}else{};
////					default:break;
////
////				}
////		}

////	}

//void Faulthandling(void)
//{
//    Position1_gap = Position1_Data - Position1;
//    Position2_gap = Position2_Data - Position2;
//    Position3_gap = Position3_Data - Position3;
//    Position4_gap = Position4_Data - Position4;
//    Position5_gap = Position5_Data - Position5;
//    Position6_gap = Position6_Data - Position6;
//    if(Position_abs(Position1_gap) > 30 || Position_abs(Position2_gap) > 30 || Position_abs(Position3_gap) > 30 || Position_abs(Position4_gap) > 30 || Position_abs(Position5_gap) > 30 || Position_abs(Position6_gap) > 30)
//    {

//        if(Position_abs(Position1_gap) > 30) //1号轮转向异常
//        {
//            time_err_cnt1++;
//            if(time_err_cnt1 >= 40) //3s
//            {
//                err_flag = 1;
////     err_LED=0001;
//                err_group[1] = 1;

//            }
//        }
//        else
//        {
//            time_err_cnt1 = 0;
//            err_group[1] = 0;
//        }



//        if(Position_abs(Position2_gap) > 30) //2号轮转向异常
//        {
//            time_err_cnt2++;
//            if(time_err_cnt2 >= 40) //3s
//            {
//                err_flag = 1;
////	err_LED=0002;
//                err_group[2] = 1;
//            }
//        }
//        else
//        {
//            time_err_cnt2 = 0;
//            err_group[2] = 0;
//        }


//        if(Position_abs(Position3_gap) > 30) //3号轮转向异常
//        {
//            time_err_cnt3++;
//            if(time_err_cnt3 >= 40) //3s
//            {
//                err_flag = 1;
////	err_LED=0003;
//                err_group[3] = 1;
//            }
//        }
//        else
//        {
//            time_err_cnt3 = 0;
//            err_group[3] = 0;
//        }


//        if(Position_abs(Position4_gap) > 30) //4号轮转向异常
//        {
//            time_err_cnt4++;
//            if(time_err_cnt4 >= 40) //3s
//            {
//                err_flag = 1;
////     err_LED=0004;
//                err_group[4] = 1;
//            }
//        }
//        else
//        {
//            time_err_cnt4 = 0;
//            err_group[4] = 0;
//        }


//        if(Position_abs(Position5_gap) > 30) //5号轮转向异常
//        {
//            time_err_cnt5++;
//            if(time_err_cnt5 >= 40) //3s
//            {
//                err_flag = 1;
////	err_LED=0005;
//                err_group[5] = 1;
//            }
//        }
//        else
//        {
//            time_err_cnt5 = 0;
//            err_group[5] = 0;
//        }


//        if(Position_abs(Position6_gap) > 30) //6号轮转向异常
//        {
//            time_err_cnt6++;
//            if(time_err_cnt6 >= 40) //3s
//            {
//                err_flag = 1;
////	  err_LED=0006;
//                err_group[6] = 1;
//            }
//        }
//        else
//        {
//            time_err_cnt6 = 0;
//            err_group[6] = 0;
//        }

//    }
//    else
//    {
//        time_err_cnt1 = 0;
//        time_err_cnt2 = 0;
//        time_err_cnt3 = 0;
//        time_err_cnt4 = 0;
//        time_err_cnt5 = 0;
//        time_err_cnt6 = 0;
//        err_flag = 0;
//        err_LED = 0000;
//    }
//}

//void Faultcontrol(void)
//{
//    if(err_flag >= 1)
//    {
//        //motor1_speed=0;motor2_speed=0;motor3_speed=0;motor4_speed=0;motor5_speed=0;motor6_speed=0;

////			Position1_Data=SteeringMotor_MidNum;
//// 		  Position2_Data=SteeringMotor_MidNum;
////			Position3_Data=SteeringMotor_MidNum;
////			Position4_Data=SteeringMotor_MidNum;
////			Position5_Data=SteeringMotor_MidNum;
////			Position6_Data=SteeringMotor_MidNum;

//    }

//}

