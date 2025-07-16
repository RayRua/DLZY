#include "gimbal_task.h"
#include "ins_task.h"
#include "motor_config.h"
#include "motor_status.h"
#include "communicate.h"
#include "watchdog.h"
#include "servo.h"
#include "usart6.h"

#include "led.h"

#include "detect_task.h"
#include "cmsis_os.h"
uint16_t count_gimbal = 0;
/*parameter*/

extern u16 MBUS_CH[17];
extern u8 MBUS_ON_OFF;
extern Gimbal_Master gimbal_master;
extern _Bool master_receive_flag;
extern double gimbal_angle[3];
Gimbal_State gimbal;
Remote_control remote;
uint8_t temp_key5;
uint8_t temp_key6;
uint8_t current_key5;
uint8_t current_key6;
_Bool flag_remote_shut = 0;
_Bool flag_remote_start = 1;
_Bool last_temp_left = 0;
_Bool last_temp_right = 0;
/*function*/
void remote_process(void);
void motor_position_control(void);
void motor_velocity_control(void);
void pitch_remotecontrol_stop(void);
void yaw_remotecontrol_stop(void);
void D_AI_POSITION_Stop(uint8_t addr);
void D_AI_POSITION_Start(uint8_t addr);
void gimbal_task()
{
	/**********************remote_control**************************/

	if (flag_remote_shut || (cmd_lost_flag && MBUS_ON_OFF == 0x10&&gimbal_master.master_status==MASTER_VELOCITY))//tick maybe not come in here in time,if no gimbal_master.master_status==MASTER_VELOCITY
	{
		remote.right_key = 0;
		remote.left_key = 0;

		//		DRV750_POSITION_Control(GIMBAL_YAW, 0);
		//		D_AI_POSITION_Control(GIMBAL_PITCH, 0);

		servo_old_torque(SERVO_LETF, SERVO_TORQUE_OFF);
		servo_old_torque(SERVO_RIGHT, SERVO_TORQUE_OFF);

		pitch_remotecontrol_stop();
		yaw_remotecontrol_stop();
		flag_remote_shut = 0;

		if (count_gimbal > 10)
		{
			LED1 = !LED1;
		} // if the motor switching mode is abnormal, LED1 will not blink
		if (count_gimbal > 10)
			count_gimbal = 0;
		return;
	}
	while ((yaw_current_status.mode != yaw_target_status.mode) || (pitch_current_status.mode != pitch_target_status.mode))
	{
		YAW_Read_Status();
		PITCH_Read_Status();

		if (yaw_current_status.mode != yaw_target_status.mode)
		{
			if (yaw_target_status.mode == DRV750_VELOCITY)
				DRV750_Motor_Control_SHIFIT(GIMBAL_YAW, DRV750_VELOCITY);
			else
				DRV750_Motor_Control_SHIFIT(GIMBAL_YAW, DRV750_POSITION);
			osDelay(5);
		}
		if (pitch_current_status.mode != pitch_target_status.mode)
		{
			if (yaw_target_status.mode == D_AI_VELOCITY)
				D_AI_Motor_Control_SHIFIT(GIMBAL_YAW, D_AI_VELOCITY);
			else
				D_AI_Motor_Control_SHIFIT(GIMBAL_YAW, D_AI_POSITION);
			osDelay(5);
		}
		if (count_gimbal > 10)
			count_gimbal = 0; // if the motor switching mode is abnormal, LED1 will not blink
	}
	if (count_gimbal > 10)
	{
		LED1 = !LED1;
		count_gimbal = 10;
	}

	servo_old_torque(SERVO_LETF, SERVO_TORQUE_ON);
	servo_old_torque(SERVO_RIGHT, SERVO_TORQUE_ON);

	if (MBUS_ON_OFF == 0x00)
	{ // remote_control
		// while
		remote_process();
		if (gimbal.yaw_position | gimbal.pitch_position)
		{
			servo_old_torque(SERVO_LETF, SERVO_TORQUE_ON);
			servo_old_torque(SERVO_RIGHT, SERVO_TORQUE_ON);
			motor_position_control();
			return;
		}
		// switch to velocity

		if ((gimbal.yaw > 0.1f || gimbal.yaw < -0.1f) && (yaw_current_status.mode != DRV750_VELOCITY))
		{
			DRV750_Motor_Control_SHIFIT(GIMBAL_YAW, DRV750_VELOCITY);
			yaw_target_status.mode = DRV750_VELOCITY;
		}
		if ((gimbal.pitch > 0.1f || gimbal.pitch < -0.1f) && (pitch_current_status.mode != D_AI_VELOCITY))
		{
			D_AI_Motor_Control_SHIFIT(GIMBAL_PITCH, D_AI_VELOCITY);
			pitch_target_status.mode = D_AI_VELOCITY;
		}
		motor_velocity_control();
		return;
	}
	/**********************remote_control**************************/
	/**********************master_control**************************/
	if (master_receive_flag && MBUS_ON_OFF == 0x10)
	{ // master
		/*servo*/
		while (!gimbal_master.left_shoot)
		{
			if (last_temp_left == gimbal_master.left_shoot)
				break;

			servo_old_target(SERVO_LETF, 0, 5);
			last_temp_left = gimbal_master.left_shoot;
			break;
		}
		while (gimbal_master.left_shoot)
		{
			if (last_temp_left == gimbal_master.left_shoot)
				break;

			servo_old_target(SERVO_LETF, 2048, 5);
			last_temp_left = gimbal_master.left_shoot;
			break;
		}

		while (!gimbal_master.right_shoot)
		{
			if (last_temp_right == gimbal_master.right_shoot)
				break;

			servo_old_target(SERVO_RIGHT, 0, 5);
			last_temp_right = gimbal_master.right_shoot;
			break;
		}
		while (gimbal_master.right_shoot)
		{
			if (last_temp_right == gimbal_master.right_shoot)
				break;

			servo_old_target(SERVO_RIGHT, 2048, 5);
			last_temp_right = gimbal_master.right_shoot;
			break;
		}
		/*servo*/
		/////****************
		/*position*/
		if (gimbal_master.master_status == MASTER_POSITION)
		{
			// yaw
			if (yaw_current_status.mode != DRV750_POSITION)
			{
				DRV750_Motor_Control_SHIFIT(GIMBAL_YAW, DRV750_POSITION);
				yaw_target_status.mode = DRV750_POSITION;
			}
			DRV750_POSITION_Control(GIMBAL_YAW, gimbal.yaw);
			gimbal.yaw = 0;
			// pitch
			if (pitch_current_status.mode != D_AI_POSITION)
			{
				D_AI_Motor_Control_SHIFIT(GIMBAL_PITCH, D_AI_POSITION);
				pitch_target_status.mode = D_AI_POSITION;
			}
			if (pitch_current_status.mode == D_AI_POSITION)
			{
				if (pd4_level == 0  && gimbal.pitch > 0.0f) // limit
				{
					gimbal.pitch = 0.0f;
					return;
				} // limit position
				else if (pd5_level == 0 && gimbal.pitch < 0.0f)
				{
					gimbal.pitch = 0.0f;
					return;
				}
				D_AI_POSITION_Control(GIMBAL_PITCH, gimbal.pitch);
				gimbal.pitch = 0;
			}
		}
		/*position*/
		/*velocity*/
		else if (gimbal_master.master_status == MASTER_VELOCITY)
		{

			yaw_target_status.mode = DRV750_VELOCITY;
			pitch_target_status.mode = D_AI_VELOCITY;
			if (yaw_current_status.mode != yaw_target_status.mode)
				DRV750_Motor_Control_SHIFIT(GIMBAL_YAW, DRV750_VELOCITY);
			if (pitch_current_status.mode != pitch_target_status.mode)
				D_AI_Motor_Control_SHIFIT(GIMBAL_PITCH, D_AI_VELOCITY);
			motor_velocity_control();
		}
		master_receive_flag = 0;
		/*velocity*/
	}

	/**********************master_control**************************/
}

void remote_process()
{ // Different remote controls need to be adapted according to the specific channel values
	remote.channel1 = MBUS_CH[1] - CH1_ZERO;
	remote.channel2 = -(MBUS_CH[2] - CH2_ZERO);
	remote.channel3 = MBUS_CH[3] - CH3_ZERO;
	remote.channel4 = MBUS_CH[4] - CH4_ZERO;
	remote.left_key = MBUS_CH[5];
	remote.right_key = MBUS_CH[6];
	switch (remote.left_key)
	{ // direct take maybe have wrong data
	case 200:
		temp_key5 = 1;
		break;
	case 1000:
		temp_key5 = 0;
		break;
	case 1800:
		temp_key5 = 2;
		break;
	}
	switch (remote.right_key)
	{
	case 200:
		temp_key6 = 1;
		break;
	case 1000:
		temp_key6 = 0;
		break;
	case 1800:
		temp_key6 = 2;
		break;
	}
	if (flag_remote_start)
	{
		current_key5 = temp_key5;
		current_key6 = temp_key6;
		flag_remote_start = 0;
	}
	else
	{
		// left_key
		if (temp_key5 != current_key5)
		{
			if (current_key5 == 0 && temp_key5 == 2)
				gimbal.yaw_position = 1;
			if (current_key5 == 0 && temp_key5 == 1)
				gimbal.yaw_position = 1;
			current_key5 = temp_key5;
		}

		// right_key
		if (temp_key6 != current_key6)
		{
			if (current_key6 == 0 && temp_key6 == 2)
				gimbal.pitch_position = 1;
			if (current_key6 == 0 && temp_key6 == 1)
				gimbal.pitch_position = 1;
			current_key6 = temp_key6;
		}
	}

	// yaw
	if (remote.channel4 > 100)
	{

		gimbal.yaw = (int32_t)remote.channel4 / 808.0f * GIMBAL_YAW_MAX_RPM;
	}
	else if (remote.channel4 < -100)
	{

		gimbal.yaw = (int32_t)remote.channel4 / 779.0 * GIMBAL_YAW_MAX_RPM;
	}
	else
		gimbal.yaw = 0;
	if (remote.channel3 > 250)
	{

		gimbal.pitch = (int32_t)remote.channel3 / 800.0 * GIMBAL_PITCH_MAX_RPM;
	}
	else if (remote.channel3 < -220)
	{

		gimbal.pitch = (int32_t)remote.channel3 / 785.0 * GIMBAL_PITCH_MAX_RPM;
	}
	else
		gimbal.pitch = 0;
}

void motor_position_control()
{
	// yaw
	if (gimbal.yaw_position)
	{
		if (yaw_current_status.mode != DRV750_POSITION)
			DRV750_Motor_Control_SHIFIT(GIMBAL_YAW, DRV750_POSITION);
		if (current_key5 == 1)
		{
			DRV750_POSITION_Control(GIMBAL_YAW, 1);
			//	servo_old_target(SERVO_LETF,0,5);//if the remote need show shoot function
		}
		else
		{
			DRV750_POSITION_Control(GIMBAL_YAW, -1);
			//		servo_old_target(SERVO_LETF,2048,5);
		}
		yaw_target_status.mode = DRV750_POSITION;
		gimbal.yaw_position = 0;
	}
	// pitch
	if (gimbal.pitch_position)
	{ // substitute pitch as shoot
		if (pitch_current_status.mode != D_AI_POSITION)
			D_AI_Motor_Control_SHIFIT(GIMBAL_PITCH, D_AI_POSITION);

		if (current_key6 == 1)
		{
			if (pd4_level != 0)
			{
				D_AI_POSITION_Control(GIMBAL_PITCH, 1);
			}

			// servo_old_target(SERVO_RIGHT,0,5);
		}
		else
		{
			if (pd5_level != 0)
			{
				D_AI_POSITION_Control(GIMBAL_PITCH, -1);
			}

			// servo_old_target(SERVO_RIGHT,2048,5);
		}
		pitch_target_status.mode = D_AI_POSITION;
		gimbal.pitch_position = 0;
	}
}
void motor_velocity_control()
{

	if (gimbal.yaw > GIMBAL_YAW_MAX_RPM)
	{
		gimbal.yaw = GIMBAL_YAW_MAX_RPM;
	}
	if (gimbal.yaw < -GIMBAL_YAW_MAX_RPM)
	{
		gimbal.yaw = -GIMBAL_YAW_MAX_RPM;
	}
	if (gimbal.pitch > GIMBAL_PITCH_MAX_RPM)
	{
		gimbal.pitch = GIMBAL_PITCH_MAX_RPM;
	}
	if (gimbal.pitch < -GIMBAL_PITCH_MAX_RPM)
	{
		gimbal.pitch = -GIMBAL_PITCH_MAX_RPM;
	}

	DRV750_VELOCITY_Control(GIMBAL_YAW, (int32_t)(gimbal.yaw * 1000 * REDUCTION_RATIO)); // 1000 0.001rpm/ 100 redction ratio

	if (pd4_level == 0 && gimbal.pitch > 0)
		gimbal.pitch = 0; // limit velocity
	if (pd5_level == 0 && gimbal.pitch < 0)
		gimbal.pitch = 0;

	D_AI_VELOCITY_Control(GIMBAL_PITCH, (int16_t)(gimbal.pitch * REDUCTION_RATIO)); // reduction ratio
	return;
}

void pitch_remotecontrol_stop(void)
{
	gimbal.pitch = 0;
	D_AI_VELOCITY_Control(GIMBAL_PITCH, gimbal.pitch);
}
void yaw_remotecontrol_stop(void)
{
	gimbal.yaw = 0;
	DRV750_VELOCITY_Control(GIMBAL_YAW, gimbal.yaw);
}

void D_AI_POSITION_Stop(uint8_t addr)
{
	uint8_t total_num;
	D_AI_Terminal_Select trigger = H3_IN2;
	D_AI_Feature_Select feature = D_AI_FEATURE_SEGMENT_POSITION;
	D_AI_Logical_Select mode_ = D_AI_TERMINAL_OPEN;
	RS485_Send(D_AI_IN_Select(addr, trigger, feature, mode_, &total_num), total_num); // servo_enabel
	osDelay(10);
}

void D_AI_POSITION_Start(uint8_t addr)
{
	uint8_t total_num;
	D_AI_Terminal_Select trigger = H3_IN2;
	D_AI_Feature_Select feature = D_AI_FEATURE_SEGMENT_POSITION;
	D_AI_Logical_Select mode_ = D_AI_TERMINAL_CLOSE;
	RS485_Send(D_AI_IN_Select(addr, trigger, feature, mode_, &total_num), total_num); // servo_enabel
	osDelay(10);
}
