#include "motor_d_ai.h"

/*********************************H02************************************/
uint8_t *D_AI_Ctr_Mode(uint8_t addr, D_AI_Control_Mode mode)
{
	return Modbus_Request_Write_Single_Register(addr, H02_00, mode);
}

/*********************************H0C************************************/
uint8_t *D_AI_Ctr_Write_EEPROM(uint8_t addr)
{
	return Modbus_Request_Write_Single_Register(addr, H0C_13, 1);
}

/*********************************H03************************************/
uint8_t *D_AI_IN_Select(uint8_t addr, D_AI_Terminal_Select trigger, D_AI_Feature_Select feature, D_AI_Logical_Select mode, uint8_t *total_num)
{
	uint16_t reg_addr;
	switch (trigger)
	{
	case H3_IN1:
		reg_addr = H03_02;
		break;
	case H3_IN2:
		reg_addr = H03_04;
		break;
	case H3_IN3:
		reg_addr = H03_06;
		break;
	case H3_IN4:
		reg_addr = H03_08;
		break;
	default:
		return NULL;
	}
	uint16_t target_process[2];
	target_process[0] = feature;
	target_process[1] = mode;
	return Modbus_Request_Write_Mul_Register(addr, reg_addr, 2, target_process, total_num);
}
/*********************************H05************************************/
uint8_t *D_AI_Ctr_POSITION_Select_Rec(uint8_t addr, D_AI_Position_Select rec)
{
	return Modbus_Request_Write_Single_Register(addr, H05_00, rec);
}
uint8_t *D_AI_Ctr_POSITION_Electronic_gear_ratio_Numerator(uint8_t addr, uint32_t numerator, uint8_t *total_num)
{
	uint16_t target_process[2];
	target_process[0] = numerator & 0xFFFF;
	target_process[1] = (numerator >> 16) & 0xFFFF;
	return Modbus_Request_Write_Mul_Register(addr, H05_07, 2, target_process, total_num);
}
uint8_t *D_AI_Ctr_POSITION_Electronic_gear_ratio_Denominator(uint8_t addr, uint16_t denominator, uint8_t *total_num)
{
	uint16_t target_process[2];
	target_process[0] = denominator & 0xFFFF;
	target_process[1] = (denominator >> 16) & 0xFFFF;
	return Modbus_Request_Write_Mul_Register(addr, H05_09, 2, target_process, total_num);
}
uint8_t *D_AI_Ctr_electronic_ratio_molecule(uint8_t addr, uint32_t rec)
{
	return Modbus_Request_Write_Single_Register(addr, H05_07, rec);
}
uint8_t *D_AI_Ctr_electronic_ratio_denominator(uint8_t addr, uint32_t rec)
{
	return Modbus_Request_Write_Single_Register(addr, H05_09, rec);
}
/*********************************H06************************************/
/*********************************config************************************/
uint8_t *D_AI_Ctr_VELOCITY_Set_Acc_time_ms(uint8_t addr, uint16_t tim)
{
	if (tim > 65535)
		tim = 65535;
	if (tim < 1)
		tim = 1;
	return Modbus_Request_Write_Single_Register(addr, H06_05, tim);
}

uint8_t *D_AI_Ctr_VELOCITY_Set_Dec_time_ms(uint8_t addr, uint16_t tim)
{
	if (tim > 65535)
		tim = 65535;
	if (tim < 1)
		tim = 1;
	return Modbus_Request_Write_Single_Register(addr, H06_06, tim);
}

uint8_t *D_AI_Ctr_VELOCITY_Select_Rec(uint8_t addr, D_AI_Velocity_Select rec)
{
	return Modbus_Request_Write_Single_Register(addr, H06_02, rec);
}
/*********************************target************************************/
uint8_t *D_AI_Ctr_VELOCITY_Expect_Target(uint8_t addr, int16_t target)
{
	return Modbus_Request_Write_Single_Register(addr, H06_03, target);
}
/*********************************H06************************************/
/*********************************H08************************************/
uint8_t *D_AI_VELOCITY_PID_P(uint8_t addr, uint16_t hz)
{ // 1~20000
	return Modbus_Request_Write_Single_Register(addr, H08_00, hz);
}
uint8_t *D_AI_VELOCITY_PID_I(uint8_t addr, uint16_t ms)
{ // 15~51200
	return Modbus_Request_Write_Single_Register(addr, H08_01, ms);
}
uint8_t *D_AI_POSITION_PID_P(uint8_t addr, uint16_t hz)
{ // 15~51200
	return Modbus_Request_Write_Single_Register(addr, H08_02, hz);
}
uint8_t *D_AI_Load_moment_of_inertia_ratio(uint8_t addr, uint16_t ms)
{ // 0~12000
	return Modbus_Request_Write_Single_Register(addr, H08_15, ms);
}
/*********************************H08************************************/
/*********************************H11************************************/
uint8_t *D_AI_Ctr_POSITION_Segment_Mode(uint8_t addr, D_AI_Multi_segment_POSITION_operation mode)
{
	return Modbus_Request_Write_Single_Register(addr, H11_00, mode);
}
uint8_t *D_AI_Ctr_POSITION_Segment_Total(uint8_t addr, uint16_t num)
{
	return Modbus_Request_Write_Single_Register(addr, H11_01, num);
}
uint8_t *D_AI_Ctr_POSITION_Segment_Displacement_Mode(uint8_t addr, D_AI_Displacement_Mode mode)
{
	return Modbus_Request_Write_Single_Register(addr, H11_04, mode);
}
uint8_t *D_AI_Ctr_POSITION_Segment_Run_Speed(uint8_t addr, uint16_t speed)
{
	return Modbus_Request_Write_Single_Register(addr, H11_14, speed);
}
uint8_t *D_AI_Ctr_POSITION_Segment_Acc_Dcc(uint8_t addr, uint16_t tim)
{
	return Modbus_Request_Write_Single_Register(addr, H11_15, tim);
}

uint8_t *D_AI_Ctr_POSITION_Segment_Per_Segment(uint8_t addr, int32_t target, uint8_t *total_num)
{
	
	uint16_t target_process[2];
	target_process[0] = target & 0xFFFF;
	target_process[1] = (target >> 16) & 0xFFFF;
	return Modbus_Request_Write_Mul_Register(addr, H11_12, 2, target_process, total_num);
}
/*********************************H11************************************/
/*********************************H0B************************************/
uint8_t *D_AI_READ_STATUS(uint8_t addr)
{
	return Modbus_Request_Read_Register(addr, H02_00, 1);
}
/*********************************H0B************************************/
