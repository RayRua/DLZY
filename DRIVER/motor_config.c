#include "motor_config.h"
#include "cmsis_os.h"
DRV750_Current_Status_t yaw_current_status;
D_AI_Current_Status_t pitch_current_status;
DRV750_Current_Status_t yaw_target_status;
D_AI_Current_Status_t pitch_target_status;

void DRV750_VELOCITY_Config(uint8_t addr, DRV750_DRV750_VELOCITY_Rec_Select rec, uint16_t acc, uint16_t dec, uint32_t default_rpm);
void DRV750_POSITION_Config(uint8_t addr, DRV750_DRV750_POSITION_Run_Mode run, uint16_t acc, uint16_t dec, int32_t ro_speed, uint32_t target);

// when you debuging , you should adjust the delay time larger than 100ms so that you can receive the message clearly in serial assistant
/********************************MOTOR_CONFIG***********************************/
/********************************DRV750_Motor_Config***********************************/
void DRV750_Motor_Config(uint8_t addr)
{
	uint8_t *tx;
	uint8_t total_num;
	DRV750_Logical_Select logical = DRV750_TERMINAL_OPEN; // unenabel motor
	DRV750_Feature_Select feature = DRV750_FEATURE_SERVO_ENABLE;
	tx = DRV750_IN_Select(addr, P2_IN1, feature, logical, &total_num);
	RS485_Send(tx, total_num);
	delay_ms(4);
	DRV750_Direction_rotation dir = CCW;
	tx = DRV750_Ctr_Direction(addr, dir); // set dirction
	RS485_Send(tx, FIX_LENGTH);
	delay_ms(4);
	DRV750_VELOCITY_Config(addr, V_MASTER, 50, 50, 0); // DRV750_POSITION initial
	delay_ms(4);
	DRV750_POSITION_Config(addr, P_RUN_MODE_INCREMENTAL_DRV750_POSITION_MODE, 50, 50, 15, 0); // DRV750_VELOCITY initial
	delay_ms(4);
	DRV750_Control_Mode mode = DRV750_POSITION_VELOCITY;
	tx = DRV750_Ctr_Mode(addr, mode); // swtich mode
	RS485_Send(tx, FIX_LENGTH);
	delay_ms(4);
	logical = DRV750_TERMINAL_CLOSE; // DRV750_VELOCITY mode
	feature = DRV750_FEATURE_CONTROL_MODE_SELECTION;
	RS485_Send(DRV750_IN_Select(addr, P2_IN4, feature, logical, &total_num), total_num);
	delay_ms(4);
	logical = DRV750_TERMINAL_CLOSE; // enabel motor
	feature = DRV750_FEATURE_SERVO_ENABLE;
	tx = DRV750_IN_Select(addr, P2_IN1, feature, logical, &total_num);
	RS485_Send(tx, total_num);
	delay_ms(5);
	RS485_Send(DRV750_Ctr_DRV750_POSITION_Cal_Ratio(addr,3600), FIX_LENGTH);
	delay_ms(5);
}
/********************************DRV750_POSITION_CONFIG***********************************/
void DRV750_POSITION_Config(uint8_t addr, DRV750_DRV750_POSITION_Run_Mode run, uint16_t acc, uint16_t dec, int32_t ro_speed, uint32_t target)
{
	uint8_t *tx;
	uint8_t total_num;
	DRV750_DRV750_POSITION_Rec_Mode rec = P_REC_MODE_MASTER_0;
	tx = DRV750_Ctr_DRV750_POSITION_Set_Rec(addr, rec); // set control source
	RS485_Send(tx, FIX_LENGTH);
	delay_ms(4);
	tx = DRV750_Ctr_DRV750_POSITION_Set_Run_Mode(addr, run); // set DRV750_POSITION control mde
	RS485_Send(tx, FIX_LENGTH);
	delay_ms(4);
	tx = DRV750_Ctr_DRV750_POSITION_Set_Acc_time_ms(addr, acc); // set acc
	RS485_Send(tx, FIX_LENGTH);
	delay_ms(4);
	tx = DRV750_Ctr_DRV750_POSITION_Set_Dec_time_ms(addr, dec); // set dec
	RS485_Send(tx, FIX_LENGTH);
	delay_ms(4);
	tx = DRV750_Ctr_DRV750_POSITION_Set_Run_RoSpeed(addr, ro_speed * 100); // set run speed
	RS485_Send(tx, FIX_LENGTH);
	delay_ms(4);
	tx = DRV750_Ctr_DRV750_POSITION_Expect_Target(addr, target, &total_num); // set per target
	RS485_Send(tx, total_num);
	delay_ms(4);
}

void DRV750_Postion_Control_Set_Per_Target(uint8_t addr, uint32_t absolute_target)
{
	osDelay(4);
	uint8_t total_num;
	RS485_Send(DRV750_Ctr_DRV750_POSITION_Expect_Target(addr, absolute_target, &total_num), total_num);
}
/********************************DRV750_VELOCITY_CONFIG***********************************/
void DRV750_VELOCITY_Config(uint8_t addr, DRV750_DRV750_VELOCITY_Rec_Select rec, uint16_t acc, uint16_t dec, uint32_t default_rpm)
{
	uint8_t *tx;
	uint8_t total_num;
	delay_ms(4);
	tx = DRV750_Ctr_DRV750_VELOCITY_Select_Rec(addr, rec); // set control source
	RS485_Send(tx, FIX_LENGTH);
	delay_ms(4);
	tx = DRV750_Ctr_DRV750_VELOCITY_Set_Acc_time_ms(addr, acc); // set acc
	RS485_Send(tx, FIX_LENGTH);
	delay_ms(4);
	tx = DRV750_Ctr_DRV750_VELOCITY_Set_Dec_time_ms(addr, dec); // set dec
	RS485_Send(tx, FIX_LENGTH);
	delay_ms(4);
	tx = DRV750_Ctr_DRV750_VELOCITY_Expect_Target(addr, default_rpm, &total_num);
	RS485_Send(tx, total_num);
	delay_ms(4);
}

/******************************CONTROL_SHIFT***********************************/
void DRV750_Motor_Control_SHIFIT(uint8_t addr, DRV750_Control_Mode expect_mode)
{
	osDelay(10);
	uint8_t total_num;
	DRV750_Logical_Select logical; // DRV750_POSITION mode
	DRV750_Feature_Select feature;
	switch (expect_mode)
	{
	case DRV750_POSITION:
		logical = DRV750_TERMINAL_OPEN; // DRV750_POSITION mode
		feature = DRV750_FEATURE_CONTROL_MODE_SELECTION;
		RS485_Send(DRV750_IN_Select(addr, P2_IN4, feature, logical, &total_num), total_num);
		break;
	case DRV750_VELOCITY:
		logical = DRV750_TERMINAL_CLOSE; // DRV750_VELOCITY mode
		feature = DRV750_FEATURE_CONTROL_MODE_SELECTION;
		RS485_Send(DRV750_IN_Select(addr, P2_IN4, feature, logical, &total_num), total_num);
		break;
	case DRV750_TORQUE:
		break;
	default:
		break;
	}
	osDelay(10);
}
/******************************CONTROL_DRV750_POSITION***********************************/
void DRV750_POSITION_Control(uint8_t addr, float target)
{
	osDelay(4);
	DRV750_Direction_rotation dir;
	if (target > 0)
		dir = CW;
	else
	{
		dir = CCW;
		target = -target;
	}
	DRV750_Postion_Control_Set_Per_Target(addr, (uint32_t)(target * GIMBAL_DEGREE_TO_PULSE_NUM_RATIO/2.0f));
	DRV750_DRV750_POSITION_Enable_Mode mode;
	if (dir == CW)
		mode = P_ENABLE_FORWARD_ONCE;
	else
		mode = P_ENABLE_BACKWARD_ONCE;
	osDelay(4);
	RS485_Send(DRV750_Ctr_DRV750_POSITION_Enable(0x01, mode), FIX_LENGTH);
	osDelay(4);
}
/******************************CONTROL_DRV750_VELOCITY***********************************/
void DRV750_VELOCITY_Control(uint8_t addr, int32_t rpm)
{
	uint8_t total_num;
	osDelay(2);

	if (rpm > 9000000)
		rpm = 9000000;
	else if (rpm < -9000000)
		rpm = -9000000;

	RS485_Send(DRV750_Ctr_DRV750_VELOCITY_Expect_Target(addr, rpm, &total_num), total_num);
	osDelay(2);
}
/******************************CONTROL_D_AI_PID***********************************/
 void DRV750_PID(uint8_t addr){	
			delay_ms(4);
			RS485_Send(DRV750_VELOCITY_PID_P(addr,250),FIX_LENGTH);
			delay_ms(4);
			RS485_Send(DRV750_VELOCITY_PID_I(addr,3183),FIX_LENGTH);
			delay_ms(4);
			RS485_Send(DRV750_POSITION_PID_P(addr,400),FIX_LENGTH);
			delay_ms(4);
 }
/******************************CONTROL_DRV750_STATUS***********************************/
void YAW_Read_Status()
{
	osDelay(5);
	RS485_Send(DRV750_READ_STATUS(GIMBAL_YAW), FIX_LENGTH);
	osDelay(10);
}
/******************************SHUT_DOWN***********************************/
/****************************************************************************************************************************************/
/****************************************************************************************************************************************/
/********************************D_AI_Motor_Config***********************************/
void D_AI_Velocity_Config(uint8_t addr);
void D_AI_Position_Config(uint8_t addr);

void D_AI_Motor_Config(uint8_t addr)
{
	delay_ms(4);
	uint8_t total_num;
	D_AI_Terminal_Select trigger = H3_IN1;
	D_AI_Feature_Select feature = D_AI_FEATURE_SERVO_ENABLE;
	D_AI_Logical_Select mode_ = D_AI_TERMINAL_OPEN;
	RS485_Send(D_AI_IN_Select(addr, trigger, feature, mode_, &total_num), total_num); // servo_enabel
	D_AI_Velocity_Config(addr);
	D_AI_Position_Config(addr);
	delay_ms(4);
	RS485_Send(D_AI_Ctr_electronic_ratio_molecule( addr,131072), FIX_LENGTH);
	delay_ms(4);
	RS485_Send(D_AI_Ctr_electronic_ratio_denominator( addr,3600), FIX_LENGTH);  
}
void D_AI_Velocity_Config(uint8_t addr)
{
	delay_ms(4);
	/*velocity*/
	D_AI_Velocity_Select rec = D_AI_PRIMARY_SPEED;
	RS485_Send(D_AI_Ctr_VELOCITY_Select_Rec(addr, rec), FIX_LENGTH); // velocity resource
	delay_ms(4);
	RS485_Send(D_AI_Ctr_VELOCITY_Set_Acc_time_ms(addr, 50), FIX_LENGTH); // acc
	delay_ms(4);
	RS485_Send(D_AI_Ctr_VELOCITY_Set_Dec_time_ms(addr, 50), FIX_LENGTH); // dcc
	delay_ms(4);
	/*default*/
	RS485_Send(D_AI_Ctr_VELOCITY_Expect_Target(addr, 0), FIX_LENGTH);
	// expect_velocity
	delay_ms(4);
}

void D_AI_Position_Config(uint8_t addr)
{
	uint8_t total_num;
	delay_ms(4);
	D_AI_Terminal_Select trigger = H3_IN2;
	D_AI_Feature_Select feature = D_AI_FEATURE_SEGMENT_POSITION;
	D_AI_Logical_Select mode_ = D_AI_TERMINAL_OPEN;
	RS485_Send(D_AI_IN_Select(addr, trigger, feature, mode_, &total_num), total_num); // segment position
	delay_ms(4);
	RS485_Send(D_AI_Ctr_POSITION_Electronic_gear_ratio_Numerator(addr, 131072, &total_num), total_num);
	delay_ms(4);
	RS485_Send(D_AI_Ctr_POSITION_Electronic_gear_ratio_Denominator(addr, 3600, &total_num), total_num);
	delay_ms(4);
	D_AI_Position_Select rec = D_AI_REC_SEGMENT;
	RS485_Send(D_AI_Ctr_POSITION_Select_Rec(addr, rec), FIX_LENGTH); // select resource
	delay_ms(4);
	D_AI_Multi_segment_POSITION_operation mode = D_AI_SINGLE_CYCLE;
	RS485_Send(D_AI_Ctr_POSITION_Segment_Mode(addr, mode), FIX_LENGTH); // select segment mode
	delay_ms(4);
	RS485_Send(D_AI_Ctr_POSITION_Segment_Total(addr, 1), FIX_LENGTH); // segment total num
	delay_ms(4);
	RS485_Send(D_AI_Ctr_POSITION_Segment_Run_Speed(addr, 5000), FIX_LENGTH); // segment speed
	delay_ms(4);
	RS485_Send(D_AI_Ctr_POSITION_Segment_Displacement_Mode(addr, D_AI_RELATIVE), FIX_LENGTH);
	delay_ms(4);
	RS485_Send(D_AI_Ctr_POSITION_Segment_Acc_Dcc(addr, 50), FIX_LENGTH); // segment total num
	delay_ms(4);
	RS485_Send(D_AI_Ctr_POSITION_Segment_Per_Segment(addr, GIMBAL_PITCH_HORIZONTAL, &total_num), total_num); // segment total num
	delay_ms(4);
}
/******************************CONTROL_SHIFT***********************************/
void D_AI_Motor_Control_SHIFIT(uint8_t addr, D_AI_Control_Mode expect_mode)
{
	osDelay(4);
	uint8_t total_num;
	D_AI_Terminal_Select trigger = H3_IN1;
	D_AI_Feature_Select feature = D_AI_FEATURE_SERVO_ENABLE;
	D_AI_Logical_Select mode_ = D_AI_TERMINAL_OPEN;
	RS485_Send(D_AI_IN_Select(addr, trigger, feature, mode_, &total_num), total_num);
	osDelay(4);
	switch (expect_mode)
	{
	case D_AI_POSITION:
		RS485_Send(D_AI_Ctr_Mode(addr, D_AI_POSITION), FIX_LENGTH);
		break;
	case D_AI_VELOCITY:
		RS485_Send(D_AI_Ctr_Mode(addr, D_AI_VELOCITY), FIX_LENGTH);
		break;
	case D_AI_TORQUE:
		RS485_Send(D_AI_Ctr_Mode(addr, D_AI_TORQUE), FIX_LENGTH);
		break;
	default:
		break;
	}
	osDelay(10);
	mode_ = D_AI_TERMINAL_CLOSE;
	RS485_Send(D_AI_IN_Select(addr, trigger, feature, mode_, &total_num), total_num); // servo enabel
	osDelay(4);
}
/******************************CONTROL_D_AI_POSITION***********************************/
void D_AI_POSITION_Control(uint8_t addr, float target)
{
	uint8_t total_num;
	osDelay(10);
	RS485_Send(D_AI_Ctr_POSITION_Segment_Per_Segment(addr, (int32_t)(target * GIMBAL_DEGREE_TO_PULSE_NUM_RATIO), &total_num), total_num);

	osDelay(10);
	D_AI_Terminal_Select trigger = H3_IN2;
	D_AI_Feature_Select feature = D_AI_FEATURE_SEGMENT_POSITION;
	D_AI_Logical_Select mode_ = D_AI_TERMINAL_OPEN;
	RS485_Send(D_AI_IN_Select(addr, trigger, feature, mode_, &total_num), total_num); // servo_enabel

	osDelay(10);
	mode_ = D_AI_TERMINAL_CLOSE;
	RS485_Send(D_AI_IN_Select(addr, trigger, feature, mode_, &total_num), total_num);
	osDelay(4);
}
/******************************CONTROL_D_AI_VELOCITY***********************************/
void D_AI_VELOCITY_Control(uint8_t addr, int16_t rpm)
{
	osDelay(4);
	if (rpm > 6000)
		rpm = 6000;
	else if (rpm < -6000)
		rpm = -6000;
	RS485_Send(D_AI_Ctr_VELOCITY_Expect_Target(addr, rpm), FIX_LENGTH);
	osDelay(4);
}
/******************************CONTROL_D_AI_PID***********************************/
 void D_AI_PID(uint8_t addr){	
			delay_ms(4);
			RS485_Send(D_AI_VELOCITY_PID_P(addr,100),FIX_LENGTH);//default 200
			delay_ms(4);
			RS485_Send(D_AI_VELOCITY_PID_I(addr,1832),FIX_LENGTH);//default 1000
			delay_ms(4);
			RS485_Send(D_AI_POSITION_PID_P(addr,80),FIX_LENGTH);//default 100
			delay_ms(4);
			RS485_Send(D_AI_Load_moment_of_inertia_ratio(addr,0),FIX_LENGTH);//default 0
			delay_ms(4);
 }
/******************************CONTROL_D_AI_STATUS***********************************/
void PITCH_Read_Status()
{
	osDelay(4);
	RS485_Send(D_AI_READ_STATUS(GIMBAL_PITCH), FIX_LENGTH);
	osDelay(10);
}
/******************************MOTOR_INIT***********************************/
void motor_init()
{
	DRV750_Motor_Config(GIMBAL_YAW);
	DRV750_PID(GIMBAL_YAW);
	D_AI_Motor_Config(GIMBAL_PITCH);
	D_AI_PID(GIMBAL_PITCH);
	yaw_current_status.mode = DRV750_VELOCITY;
	yaw_target_status.mode = DRV750_VELOCITY;
	pitch_current_status.mode = D_AI_VELOCITY;
	pitch_target_status.mode = D_AI_VELOCITY;
}
