#ifndef __DRV750_GIBMAL_
#define __DRV750_GIBMAL_
/********************************include***********************************/
#include "motor_drv750.h"
#include "motor_d_ai.h"
#include "usart6.h"
#include "delay.h"
#include "motor_status.h"
/********************************macro***********************************/
#define REDUCTION_RATIO 100
#define GIMBAL_DEGREE_TO_MOTOR_DEGREE_RATIO REDUCTION_RATIO
#define GIMBAL_DEGREE_TO_PULSE_NUM_RATIO GIMBAL_DEGREE_TO_MOTOR_DEGREE_RATIO*10
//positon control
/********************************typedef***********************************/

/********************************define***********************************/
void DRV750_Motor_Config(uint8_t addr);

void DRV750_Motor_Control_SHIFIT(uint8_t addr,DRV750_Control_Mode expect_mode);
void DRV750_Postion_Control_Set_Per_Target(uint8_t addr,uint32_t target);

void DRV750_POSITION_Control(uint8_t addr,float target);

void DRV750_VELOCITY_Control(uint8_t addr, int32_t rpm);

void YAW_Read_Status(void);


/*********************************************************************/

void D_AI_Motor_Config(uint8_t addr);
void D_AI_Motor_Control_SHIFIT(uint8_t addr,D_AI_Control_Mode expect_mode);
void D_AI_POSITION_Control(uint8_t addr,float target);
void D_AI_VELOCITY_Control(uint8_t addr, int16_t rpm);
void D_AI_Parameter_Update(uint8_t addr);

void PITCH_Read_Status(void);
/*********************************************************************/

void motor_init(void);
#endif
