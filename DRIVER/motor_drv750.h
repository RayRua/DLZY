#ifndef _DRV750_CONTROL_
#define _DRV750_CONTROL_

// Define STM32 device type before including stm32f4xx.h
#ifndef STM32F40_41xxx
#define STM32F40_41xxx
#endif

/********************************include***********************************/
#include "Modbus_RTU_protocol.h"
/********************************config***********************************/
#define USE_17_BIT
#if defined(USE_17_BIT)
#define ENCODE_RANGE 131072 // 2^17
#else
#define ENCODE_RANGE 8388608 // 2^23
#endif

/********************************typedef***********************************/
/****P01****/
typedef enum
{
	DRV750_POSITION = 0,
	DRV750_VELOCITY,
	DRV750_TORQUE,
	DRV750_ETHERCAT_CANOPEN,
	DRV750_VELOCITY_TORQUE,
	DRV750_POSITION_VELOCITY,
	DRV750_POSITION_TORQUE,
	DRV750_P_V_T
} DRV750_Control_Mode;

typedef enum
{
	CCW = 0,
	CW = 1
} DRV750_Direction_rotation;
/****P01****/
/****P02****/
typedef enum
{
	P2_IN1 = 0,
	P2_IN2,
	P2_IN3,
	P2_IN4
} DRV750_Terminal_Select;

typedef enum
{
	DRV750_FEATURE_NULL = 0,
	DRV750_FEATURE_SERVO_ENABLE,
	DRV750_FEATURE_CONTROL_MODE_SELECTION = 10,
	DRV750_FEATURE_SPEED_DIR = 19,
} DRV750_Feature_Select;

typedef enum
{
	DRV750_TERMINAL_OPEN = 0,
	DRV750_TERMINAL_CLOSE,
	DRV750_TERMINAL_RISING,
	DRV750_TERMINAL_FALLING,
	DRV750_TERMINAL_RISE_AND_FALL
} DRV750_Logical_Select;
/****P02****/
/****P03****/
typedef enum
{
	P_REC_MODE_PULSE = 0,
	P_REC_MODE_STEPPING,
	P_REC_MODE_MUL_POS,
	P_REC_MODE_MASTER_0,
	P_REC_MODE_MASTER_1,
	P_REC_MODE_IO
} DRV750_DRV750_POSITION_Rec_Mode;

typedef enum
{
	P_RUN_MODE_INCREMENTAL_DRV750_POSITION_MODE = 0,
	P_RUN_MODE_ABSOLUTE_DRV750_POSITION_MODE
} DRV750_DRV750_POSITION_Run_Mode;

typedef enum
{
	P_ENABLE_NONE = 0,
	P_ENABLE_FORWARD_ONCE,
	P_ENABLE_BACKWARD_ONCE,
	P_ENABLE_FORWARD_TAP_ONCE,
	P_ENABLE_BACKWARD_TAP_ONCE,
	P_ENABLE_FORWARD_SLOW_TO_STOP,
	P_ENABLE_BACKWARD_SLOW_TO_STOP,
	P_ENABLE_START_TAP_WITH_DIR
} DRV750_DRV750_POSITION_Enable_Mode;
/****P03****/
/****P04****/
typedef enum
{
	V_FIX_TARGET = 0,
	V_AI1,
	V_AI2,
	V_MUL = 5
} DRV750_DRV750_VELOCITY_Rec_Source;

typedef enum
{
	V_A = 0,
	V_B,
	V_A_PLUS_B,
	V_A_B_SWITCH,
	V_MASTER
} DRV750_DRV750_VELOCITY_Rec_Select;

typedef enum
{
	V_SLOW_TO_DOWN = 0,
	V_RUN_FORWARD,
	V_RUN_BACKWARD,
	V_IO_FORWARD,
	V_IO_BACKWARD,
	V_STOP,
} DRV750_DRV750_VELOCITY_Trigger_Mode;

/****P04****/
/****P06****/
uint8_t *DRV750_VELOCITY_PID_P(uint8_t addr, uint16_t hz);
uint8_t *DRV750_VELOCITY_PID_I(uint8_t addr, uint16_t ms);
uint8_t *DRV750_POSITION_PID_P(uint8_t addr, uint16_t hz);
/****P06****/
/********************************define***********************************/
/********************************P1***********************************/
uint8_t *DRV750_Ctr_Mode(uint8_t addr, DRV750_Control_Mode mode);
uint8_t *DRV750_Ctr_Direction(uint8_t addr, DRV750_Direction_rotation dir);
/********************************P2***********************************/
uint8_t *DRV750_IN_Select(uint8_t addr, DRV750_Terminal_Select trigger, DRV750_Feature_Select feature, DRV750_Logical_Select mode, uint8_t *total_num);
/********************************P3***********************************/
/*********	Function reference template**********/ /*
 void SetDRV750_POSITION(){
			 uint8_t* tx;
			 uint8_t total_num;
			 Control_Mode mode=DRV750_POSITION;
			 tx=DRV750_Ctr_Mode(0x01,mode);//set mode
			 DRV750_Send(tx,FIX_LENGTH);
			 Delay_ms(100);
			 Direction_rotation dir=CCW;
			 tx=DRV750_Ctr_Direction(0x01,dir);//set dirction
			 DRV750_Send(tx,FIX_LENGTH);
			 Delay_ms(100);
			 DRV750_POSITION_Rec_Mode rec=P_Rec_Mode_MASTER_0;
			 tx=DRV750_Ctr_DRV750_POSITION_Set_Rec(0x01,rec);//set control source
			 DRV750_Send(tx,FIX_LENGTH);
			 Delay_ms(100);
			 DRV750_POSITION_Run_Mode run=P_RUN_MODE_INCREMENTAL_DRV750_POSITION_MODE;
			 tx=DRV750_Ctr_DRV750_POSITION_Set_Run_Mode(0x01,run);//set DRV750_POSITION control mde
			 DRV750_Send(tx,FIX_LENGTH);
			 Delay_ms(100);
			 tx=DRV750_Ctr_DRV750_POSITION_Set_Acc_time_ms(0x01, 100);//set acc
			 DRV750_Send(tx,FIX_LENGTH);
			 Delay_ms(100);
			 tx=DRV750_Ctr_DRV750_POSITION_Set_Dec_time_ms(0x01, 100);//set dec
			 DRV750_Send(tx,FIX_LENGTH);
			 Delay_ms(100);
			 tx=DRV750_Ctr_DRV750_POSITION_Set_Run_RoSpeed(0x01, 100);//set run speed
			 DRV750_Send(tx,FIX_LENGTH);
			 Delay_ms(100);
			 tx=DRV750_Ctr_DRV750_POSITION_Expect_Target(0x01, 10000,&total_num);//set run speed
			 DRV750_Send(tx,total_num);
			 Delay_ms(100);
			 Terminal_Logical mode__=TERMINAL_CLOSE;
			 DRV750_Enable_IN1(0x01,mode__);//enabel motor
			 DRV750_Send(tx,FIX_LENGTH);
			 Delay_ms(100);
			 DRV750_POSITION_Enable_Mode mode_ =7;
			 DRV750_Send(DRV750_Ctr_DRV750_POSITION_Enable(0x01,mode_),FIX_LENGTH);
 }
 */
												   /*********	Function reference template**********/
/********************************P2***********************************/
uint8_t *DRV750_READ_STATUS(uint8_t addr);
/********************************P3***********************************/
uint8_t *DRV750_Ctr_DRV750_POSITION_Cal_Ratio(uint8_t addr, uint32_t one_r_pos_com_num);
uint8_t *DRV750_Ctr_DRV750_POSITION_Set_Rec(uint8_t addr, DRV750_DRV750_POSITION_Rec_Mode rec);
uint8_t *DRV750_Ctr_DRV750_POSITION_Set_Run_Mode(uint8_t addr, DRV750_DRV750_POSITION_Run_Mode run);
uint8_t *DRV750_Ctr_DRV750_POSITION_Set_Acc_time_ms(uint8_t addr, uint16_t tim);
uint8_t *DRV750_Ctr_DRV750_POSITION_Set_Dec_time_ms(uint8_t addr, uint16_t tim);
uint8_t *DRV750_Ctr_DRV750_POSITION_Set_Run_RoSpeed(uint8_t addr, int32_t rpm);
uint8_t *DRV750_Ctr_DRV750_POSITION_Expect_Target(uint8_t addr, uint32_t target, uint8_t *total_num);
uint8_t *DRV750_Ctr_DRV750_POSITION_Enable(uint8_t addr, DRV750_DRV750_POSITION_Enable_Mode mode);
/********************************P4***********************************/
uint8_t *DRV750_Ctr_DRV750_VELOCITY_Set_Rec_A_AND_B(uint8_t addr, DRV750_DRV750_VELOCITY_Rec_Source rec_a, DRV750_DRV750_VELOCITY_Rec_Source rec_b, uint8_t *total_num);
uint8_t *DRV750_Ctr_DRV750_VELOCITY_Select_Rec(uint8_t addr, DRV750_DRV750_VELOCITY_Rec_Select rec);
uint8_t *DRV750_Ctr_DRV750_VELOCITY_Set_Run_RoSpeed(uint8_t addr, int16_t rpm);
uint8_t *DRV750_Ctr_DRV750_VELOCITY_Set_Acc_time_ms(uint8_t addr, uint16_t tim);
uint8_t *DRV750_Ctr_DRV750_VELOCITY_Set_Dec_time_ms(uint8_t addr, uint16_t tim);
uint8_t *DRV750_Ctr_DRV750_VELOCITY_Expect_Target(uint8_t addr, uint32_t target, uint8_t *total_num);
uint8_t *DRV750_Ctr_DRV750_VELOCITY_Trigger_Target(uint8_t addr, DRV750_DRV750_VELOCITY_Trigger_Mode trigger);
/********************************length***********************************/
// write single register /write mul registers by uint8_t* total_num /read mul registers by specific situation
#endif
