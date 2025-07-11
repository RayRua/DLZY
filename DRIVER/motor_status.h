#ifndef __GIBMAL_STATUS_
#define __GIBMAL_STATUS_
/********************************include***********************************/
#include "motor_drv750.h"
#include "motor_d_ai.h"
/********************************typedef***********************************/
typedef struct{
			DRV750_Control_Mode mode;
}DRV750_Current_Status_t;

typedef struct{
			D_AI_Control_Mode mode;
			
}D_AI_Current_Status_t;

extern DRV750_Current_Status_t yaw_current_status;
extern D_AI_Current_Status_t pitch_current_status;
extern DRV750_Current_Status_t yaw_target_status;
extern D_AI_Current_Status_t pitch_target_status;
/********************************define***********************************/

#define GIMBAL_YAW	0x01
#define GIMBAL_PITCH 0x02

#define SERVO_LETF 0x01
#define SERVO_RIGHT 0x02

#define GIMBAL_PITCH_HORIZONTAL 0
#define GIMBAL_PITCH_MIN 42047
#define GIMBAL_PITCH_MAX 58578
#endif
