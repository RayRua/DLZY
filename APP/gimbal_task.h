#ifndef _GIMBAL_TASK_
#define _GIMBAL_TASK_
#include "stdint.h"
typedef struct
{
	float yaw;
	_Bool yaw_position;
	float pitch;
	_Bool pitch_position;
} Gimbal_State;

typedef struct
{
	int32_t channel1;
	int32_t channel2;
	int32_t channel3;
	int32_t channel4;
	uint16_t left_key;
	uint16_t right_key;
} Remote_control;

#define CH1_ZERO 958  // 1340~958~702
#define CH2_ZERO 1040 // 635~1040~1430

#define CH3_ZERO 1000 // 1800~1000~215
#define CH4_ZERO 1124  // 1326~928~528

#define GIMBAL_YAW_MAX_RPM 15.0f  // 0.016667~10
#define GIMBAL_PITCH_MAX_RPM 10.0f // 0.016667~5

extern Gimbal_State gimbal;
extern _Bool flag_remote_shut;	// when MCU start,and close remote
extern _Bool flag_remote_start; // when MCU start,and open remote
extern uint16_t count_gimbal;

void gimbal_task(void);
#endif
