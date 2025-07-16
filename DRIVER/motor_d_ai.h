#ifndef _MOOTOR_D_AI_PROTOCOL_
#define _MOOTOR_D_AI_PROTOCOL_

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
/****H02****/
typedef enum
{
	D_AI_VELOCITY = 0,
	D_AI_POSITION,
	D_AI_TORQUE,
	D_AI_NULL
} D_AI_Control_Mode;

uint8_t *D_AI_Ctr_Mode(uint8_t addr, D_AI_Control_Mode mode);
/****H02****/

/****H03****/
typedef enum
{
	H3_IN1 = 0,
	H3_IN2,
	H3_IN3,
	H3_IN4,
	H3_IN5,
	H3_IN8,
	H3_IN9
} D_AI_Terminal_Select;

typedef enum
{
	D_AI_FEATURE_NULL = 0,
	D_AI_FEATURE_SERVO_ENABLE,
	D_AI_FEATURE_SEGMENT_POSITION = 28
} D_AI_Feature_Select;

typedef enum
{
	D_AI_TERMINAL_OPEN = 0,
	D_AI_TERMINAL_CLOSE,
} D_AI_Logical_Select;
uint8_t *D_AI_IN_Select(uint8_t addr, D_AI_Terminal_Select trigger, D_AI_Feature_Select feature, D_AI_Logical_Select mode, uint8_t *total_num);
uint8_t *D_AI_Ctr_electronic_ratio_molecule(uint8_t addr, uint32_t rec);
uint8_t *D_AI_Ctr_electronic_ratio_denominator(uint8_t addr, uint32_t rec);
/****H03****/
/****H05****/
typedef enum
{
	D_AI_REC_PULSE = 0,
	D_AI_REC_SEGMENT = 2
} D_AI_Position_Select;
uint8_t *D_AI_Ctr_POSITION_Select_Rec(uint8_t addr, D_AI_Position_Select rec);
uint8_t *D_AI_Ctr_POSITION_Electronic_gear_ratio_Numerator(uint8_t addr, uint32_t numerator, uint8_t *total_num);
uint8_t *D_AI_Ctr_POSITION_Electronic_gear_ratio_Denominator(uint8_t addr, uint16_t denominator, uint8_t *total_num);
/****H05****/
/****H06****/
typedef enum
{
	D_AI_PRIMARY_SPEED = 0,
	D_AI_ASSIST_SPEED
} D_AI_Velocity_Select;

uint8_t *D_AI_Ctr_Write_EEPROM(uint8_t addr);
uint8_t *D_AI_Ctr_VELOCITY_Set_Acc_time_ms(uint8_t addr, uint16_t tim);
uint8_t *D_AI_Ctr_VELOCITY_Set_Dec_time_ms(uint8_t addr, uint16_t tim);
uint8_t *D_AI_Ctr_VELOCITY_Select_Rec(uint8_t addr, D_AI_Velocity_Select rec);
/****H06****/
/*********************************target************************************/
uint8_t *D_AI_Ctr_VELOCITY_Expect_Target(uint8_t addr, int16_t target);

/****H06****/
/****H08****/
uint8_t *D_AI_VELOCITY_PID_P(uint8_t addr, uint16_t hz);
uint8_t *D_AI_VELOCITY_PID_I(uint8_t addr, uint16_t ms);
uint8_t *D_AI_POSITION_PID_P(uint8_t addr, uint16_t hz);
uint8_t *D_AI_Load_moment_of_inertia_ratio(uint8_t addr, uint16_t ms);
/****H08****/
/****H11****/
typedef enum
{
	D_AI_SINGLE_CYCLE = 0,
	D_AI_CYCLE,
	D_AI_DI_SWITCH
} D_AI_Multi_segment_POSITION_operation;
typedef enum
{
	D_AI_RELATIVE = 0,
	D_AI_ABSOLUTE,
} D_AI_Displacement_Mode;
uint8_t *D_AI_Ctr_POSITION_Segment_Mode(uint8_t addr, D_AI_Multi_segment_POSITION_operation mode);
uint8_t *D_AI_Ctr_POSITION_Segment_Total(uint8_t addr, uint16_t num);
uint8_t *D_AI_Ctr_POSITION_Segment_Displacement_Mode(uint8_t addr, D_AI_Displacement_Mode mode);
uint8_t *D_AI_Ctr_POSITION_Segment_Run_Speed(uint8_t addr, uint16_t speed);
uint8_t *D_AI_Ctr_POSITION_Segment_Acc_Dcc(uint8_t addr, uint16_t tim);
uint8_t *D_AI_Ctr_POSITION_Segment_Per_Segment(uint8_t addr, int32_t target, uint8_t *total_num);
/****H11****/
/****H0B****/
uint8_t *D_AI_READ_STATUS(uint8_t addr);
/****H0B****/
/********************************length***********************************/
#define FIX_LENGTH 8 // write single register /write mul registers by uint8_t* total_num /read mul registers by specific situation

#endif
