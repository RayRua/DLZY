#ifndef __DRV750_CONTROL_
#define __DRV750_CONTROL_
/********************************include***********************************/
#include "drv750_protocol.h"
/********************************config***********************************/
#define USE_17_BIT
#if defined(USE_17_BIT)
#define ENCODE_RANGE 131072  // 2^17
#else 
#define ENCODE_RANGE 8388608  // 2^23
#endif

/********************************typedef***********************************/
/****P01****/
typedef enum{
			POSITION=0,
			VELOCITY,
			TORQUE,
			EtherCAT_CANopen,
			VELOCITY_TORQUE,
			POSITION_VELOCITY,
			POSITION_TORQUE,
			P_V_T
}Control_Mode;

typedef enum{
			CW=0,
			CCW
}Direction_rotation;
/****P01****/
/****P03****/
typedef enum{
			P_PULSE=0,
			P_STEPPING,
			P_MUL_POS,
			P_MASTER_0,
			P_MASTER_1,
			P_IO
}Position_Rec_Mode;

typedef enum{
			Incremental_Position_Mode=0,
			Absolute_Position_Mode
}Position_Run_Mode;
/****P03****/
/****P04****/
typedef enum{
			V_FIX_TARGET=0,
			V_MUL_VEL,
			V_MASTER_V,
			V_IO,
			V_ANALOG
}Velocity_Rec_Mode;

typedef enum{
			V_SLOW_TO_DOWN=0,
			V_RUN_FORWARD,
			V_RUN_BACKWARD,
			V_IO_FORWARD,
			V_IO_BACKWARD,
			V_STOP,
}Velocity_Trigger_Mode;

/****P04****/
/********************************define***********************************/




#endif
