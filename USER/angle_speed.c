#include "angle_speed.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"
#include "uart5.h"
#include "math.h"
#include <string.h>
#include "communicate.h"
#include "watchdog.h"
/**********************************************************************************************
**                                        前                                                 **
**                                  O--29--|--29--O                                          **
**                                         |                                                 **
**                                         66                                                **
**                                         |                                                 **
**                          O-----70-------|------70-----0                              中*线**
**                                         |                                                 **
**                                        轴                                                 **
**                                        线                                                 **
**                                         |                                                 **
**                                         66                                                **
**                                         |                                                 **
**                              O---51-----|-----51---O                                      **
***********************************************************************************************/

#define PI 3.1415926  //圆周率
#define CMD_LOST_MS 500
#define change_value_L 256
#define change_value_R 768
#define SteeringMotor_MidNum 512               //转向器的中值
#define GESR_RATIO 5                           //减速比 1：5
#define WHEEL_R        0.33                    //轮子的直径是   0.32     米 
#define WHEEL_L        WHEEL_R*PI              //轮子的周长是   0.32*PI  米
#define MAXSPEED    3.0 //指令控制速度限制
#define MINSPEED    0.00
#define MAXANGLESPEED     1.5
#define MINANGLESPEED     0.00
#define DUOJI_START 128
#define d1f 0.710 //前轮到中间轮的垂直距离
#define d1r 0.770 //中间轮到后轮的垂直距离
#define d2f 0.560 //前面两轮间距，即1号到4号
#define d2m 1.500 //中间两轮的间距，即2号到5号的间距
#define d2r 1.000 //后面两轮的间距
#define angle_initf (atan(d1f/(d2f/2.0)))*(180/PI) //57.99462 //ThetaI = atan(d1f/(d2f/2)) 弧度转角度
#define angle_initr (atan(d1r/(d2r/2.0)))*(180/PI) //57.99461 //ThetaI = atan(d1r/(d2r/2)) 弧度转角度
#define step_cnt  2.84444  //4096/360
#define K2 60*GESR_RATIO/(WHEEL_L)    //K2 = (60*5)/(2*3.14*r) r:轮子半径，单位米 （每秒60）*（减速比5）/（周长1米）。    r/分钟≈300*n米/秒
#define B2Front (d2f/2)*(K2)     			//B2f = d2f/2* K2
#define B2Mid (d2m/2)*(K2)       			//B2Mid = d2m/2*K2   
#define B2Rear (d2r/2)*(K2)      			//B2Mid = d2r/2*K2
#define K5 K2  				//等于K2
#define B5Mid B2Mid   //等于B2Mid

float liner_x_speed, liner_x_speed_pre, liner_y_speed, liner_y_speed_pre, anglespeed, anglespeed_pre;
float m2_target_buf, m5_target_buf;
float turn_R;
float s1, s2, s3, s4, s5, s6;
float ThetaOf, ThetaIr, ThetaOr, ThetaIf, ThetaM;
//遥控器数据（最大/最小）对中间值的最大差值
extern u8 recive_ok;
extern COMMUNICATE* pSelect;
extern uint8_t MBus485_FLAG;
extern u8 step;
extern u16 x_speed;
extern u16 y_speed;
extern u16 z_speed;
extern u16 k_number, key5, key6;
extern u8 MBUS_ON_OFF; //遥控器开关机状态
extern u16 Mbus_Time_OFF_cnt;
extern u16 Mbus_Time_OFF_flag;
extern u16 Position1_Data, Position2_Data, Position3_Data, Position4_Data, Position5_Data, Position6_Data; //计算目标位置
extern u16 Position1, Position2, Position3, Position4, Position5, Position6, Position7, Position8;//实时编码器角度位置
extern s16 motor1_speed, motor2_speed, motor3_speed, motor4_speed, motor5_speed, motor6_speed, motor7_speed, motor8_speed; //电机速度

extern volatile uint8_t cmd_lost_flag;
float mbus_max = 800;//遥控器最大最小到中间位置的差值
float mbus_zero = 1000;//遥控器中间位置
extern int reset_but;//复位
extern u16 pitch;
u16 x_speed_zero = 30;
u16 y_speed_zero = 100;
u16 z_speed_zero = 30;

float speed_abs(float x)
{
    float y;
    y = x;
    if(y > 0) y = y;
    else y = -y;
    return y;
}

/************************************************************/
u16 Position1_a = 34;
s16 motor1_speedaa = 62;
void speeddecode(void)
{
    Position2 = 8000;
    if(liner_x_speed == 0) {
        motor2_speed = 0;
    } else {
        motor2_speed = liner_x_speed * 1000;
        if(liner_x_speed > 0) {
            if(Position2 < 6500) {
                motor2_speed = 0;
            }
        }
        if(liner_x_speed < 0) {
            if(Position2 > 9500)
            {
                motor2_speed = 0;
            }
        }
        motor2_speed = (motor2_speed);
    }
    if(anglespeed == 0) {
        motor1_speed = 0;
    } else {
        motor1_speed = -(anglespeed * 3000);
    }
}

void speedLoopXYZ(int state)
{

    if(state == 1) {
        memcpy( & liner_x_speed, pSelect->rx_buf + 6, 4);
        memcpy( & liner_y_speed, pSelect->rx_buf + 10, 4);
        memcpy( & anglespeed, pSelect->rx_buf + 14, 4);
    }
    if(isfinite(liner_x_speed) == 0 || isfinite(liner_y_speed) == 0 || isfinite(anglespeed) == 0)         //解决NAN问题
    {
        liner_x_speed = 0;
        liner_y_speed = 0;
        anglespeed = 0;
    }
    if(liner_x_speed > (float)MAXSPEED)   //limit max liner speed
        liner_x_speed = MAXSPEED;

    if(liner_x_speed < -MAXSPEED)
        liner_x_speed = -MAXSPEED;

    if(liner_x_speed < (float)MINSPEED && liner_x_speed > 0)   //limit min liner speed
        liner_x_speed = MINSPEED;

    if(liner_x_speed > -MINSPEED && liner_x_speed < 0)
        liner_x_speed = -MINSPEED;

    if(liner_y_speed > (float)MAXSPEED)   //limit max liner speed
        liner_y_speed = MAXSPEED;

    if(liner_y_speed < -MAXSPEED)
        liner_y_speed = -MAXSPEED;

    if(liner_y_speed < (float)MINSPEED && liner_y_speed > 0)   //limit min liner speed
        liner_y_speed = MINSPEED;

    if(liner_y_speed > -MINSPEED && liner_y_speed < 0)
        liner_y_speed = -MINSPEED;

    if(anglespeed > (float)MAXANGLESPEED)   //limit max angler speed
        anglespeed = MAXANGLESPEED;

    if(anglespeed < -MAXANGLESPEED)
        anglespeed = -MAXANGLESPEED;

    if(anglespeed < (float)MINANGLESPEED && anglespeed > 0)   //limit min angler speed
        anglespeed = MINANGLESPEED;

    if(anglespeed > -MINANGLESPEED && anglespeed < 0)
        anglespeed = -MINANGLESPEED;

    speeddecode();
}

void MBUS_TO_MotorSpeed(void)
{
    if(MBus485_FLAG == 0)
    {
        mbus_max = 128; //遥控器数据（最大/最小）对中间值的最大差值
        mbus_zero = 128;
        x_speed_zero = 1; //30
        y_speed_zero = 1; //100
        z_speed_zero = 1; //30
    }

    if(((x_speed <= 550) && (y_speed >= 1350) && (k_number >= 1300)) || (reset_but == 1 && x_speed > 1900 && pitch > 1900))   //调中值状态
    {
        step = 1;
        motor1_speed = 0;
        motor2_speed = 0;
        motor3_speed = 0;
        motor4_speed = 0;
        motor5_speed = 0;
        motor6_speed = 0;
        //Read_AS5600_State();
    }
    else
    {
        step = 2;

        if((x_speed == 0) && (y_speed == 0) && (z_speed == 0) && (k_number == 0))  //先开主机，后开遥控器，不可让机器行走。
        {
            liner_x_speed = 0;
            liner_y_speed = 0;
            anglespeed = 0;
        }
        else
        {
            if(speed_abs(x_speed - mbus_zero) > x_speed_zero)
            {
                liner_x_speed = x_speed*((float)MAXSPEED / (float)mbus_max) - mbus_zero*((float)MAXSPEED / (float)mbus_max);
            }
            else
            {
                liner_x_speed = 0;
            }

            if(speed_abs(y_speed - mbus_zero) > y_speed_zero)
            {
                liner_y_speed = y_speed*((float)MAXSPEED / (float)mbus_max) - mbus_zero*((float)MAXSPEED / (float)mbus_max);
            }
            else
            {
                liner_y_speed = 0;
            }

            if(speed_abs(z_speed - mbus_zero) > z_speed_zero)
            {
                anglespeed = z_speed*((float)MAXANGLESPEED / (float)mbus_max) - mbus_zero*((float)MAXANGLESPEED / (float)mbus_max);
            }
            else
            {
                anglespeed = 0;
            }

            anglespeed = -anglespeed;//遥控器模式时z轴速度取反
        }

        speedLoopXYZ(0);
    }
}

/************************************************************/

/*上电判断遥控器按键值进行进入调中*/
void setMagneticEncoder_Code() {
    if(((x_speed <= 550) && (y_speed >= 1350) && (k_number >= 1300)) || (reset_but == 1 && x_speed > 1900 && pitch > 1900))   //调中值状态
    {
        step = 1;
        motor1_speed = 0;
        motor2_speed = 0;
        motor3_speed = 0;
        motor4_speed = 0;
        motor5_speed = 0;
        motor6_speed = 0;
       // Read_AS5600_State();
    } else {
        step = 2;
    }
}

/*所有电机速度和舵机角度值计算*/
void ALL_speed_OUT(void)
{
    switch(MBUS_ON_OFF)
    {
    case 0x00:/*遥控器开机状态*/
        cmd_lost_time = 0;
        Mbus_Time_OFF_cnt = 0;
        MBUS_TO_MotorSpeed();
        break;
    case 0x10:/*遥控器关机状态*/
        if(step == 1)
        {
            motor1_speed = 0;
            motor2_speed = 0;
            motor3_speed = 0;
            motor4_speed = 0;
            motor5_speed = 0;
            motor6_speed = 0;
        } else {
            step = 2;
        }
        if(step == 0)
        {
            Mbus_Time_OFF_flag = 1;
        }

        if(cmd_lost_flag)   //超过XXms，强制target 清零。指令中断
        {
            //cmd_lost_time = CMD_LOST_MS + 1;
            liner_x_speed = 0;
            liner_y_speed = 0;
            anglespeed = 0;
            motor1_speed = 0;
            motor2_speed = 0;
            motor3_speed = 0;
            motor4_speed = 0;
            motor5_speed = 0;
            motor6_speed = 0;

        } else {
            if(recive_ok)
            {
                recive_ok = 0;
                if(pSelect->rx_buf[2] == 0x0F)   //16*8bit，速度控制指令
                {
                    if(pSelect->rx_buf[3] == 0x12)
                    {
                        switch(pSelect->rx_buf[4])
                        {
                        case 0x00:
                            break;  //registerconfig
                        case 0x01:
                            speedLoopXYZ(1);
                            break;  //speedconfig
                        case 0x02:
                            break;  //zhuanjumoshi
                        default :
                            break;
                        }
                    }
                }
            }
        }

        break;
    default: /*遥控器按下急停*/
        motor1_speed = 0;
        motor2_speed = 0;
        motor3_speed = 0;
        motor4_speed = 0;
        motor5_speed = 0;
        motor6_speed = 0;
        break;
    }
}



