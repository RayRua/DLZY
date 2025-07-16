#include "servo.h"
#include "stdint.h"
#include "motor_status.h"
#include "uart4.h"
#include "cmsis_os.h"
uint8_t request[32];
uint8_t calculateChecksum(uint8_t ID, uint8_t commandType, uint8_t *parameters, uint8_t parameterCount)
{
	uint8_t dataLength = parameterCount + 2;
	uint8_t checksum = ID + dataLength + commandType; //
	for (uint8_t i = 0; i < parameterCount; i++)
	{							   //
		checksum += parameters[i]; //
	}
	checksum = (~checksum) & 0xFF; //
	return checksum;			   //
}

uint8_t *servo_send(uint8_t addr, uint8_t reg_addr, SERVO_REQUEST_TYPE commandType, uint8_t *data, uint8_t *total_num, uint8_t length)
{
	request[0] = 0x12;
	request[1] = 0x4c;
	request[2] = addr;
	request[3] = 1 + 1 + 1 + length; // addr+commandType+ reg +data,reg_addr not in data
	request[4] = commandType;
	request[5] = reg_addr;
	for (uint8_t i = 0; i < length; i++)
	{
		request[6 + i] = data[i];
	}
	request[8] = calculateChecksum(addr, commandType, request + 5, length + 1);
	*total_num = 2 + 1 + 1 + 1 + length + 1 + 1; // fix(2)+addr(1)+length(1)+commandtype(1) + length (2)+reg_addr(1)+ check(1)
	return request;
}
void servo_target(uint8_t addr, uint16_t target)
{
	uint8_t data[2];
	uint8_t total_num;
	data[0] = target >> 8;
	data[1] = target;
	Uart4_Send(servo_send(addr, SERVO_REG_TARGET_POSITION, SERVO_WRITE_DATA, data, &total_num, sizeof(data)), total_num);
	osDelay(5);
	return;
}

/*****************************************************************************************************************************************************/
// 0~4095 old servo
uint8_t *servo_old_send(uint8_t addr, uint8_t reg_addr, SERVO_REQUEST_TYPE commandType, uint8_t *data, uint8_t *total_num, uint8_t length)
{
	request[0] = 0xFF;
	request[1] = 0xFF;
	request[2] = addr;
	request[3] = 1 + 1 + 1 + length; // reg_addr not in data
	request[4] = commandType;
	request[5] = reg_addr;
	for (uint8_t i = 0; i < length; i++)
	{
		request[6 + i] = data[i];
	}
	request[6 + length] = calculateChecksum(addr, commandType, request + 5, length + 1);
	*total_num = 2 + 1 + 1 + 1 + length + 1 + 1; // fix(2)+addr(1)+length(1)+commandtype(1) + length+reg_addr(1)+ check(1)
	return request;
}
void servo_old_target(uint8_t addr, uint16_t target, uint16_t time)
{
	uint8_t data[4];
	uint8_t total_num;
	data[0] = target >> 8;
	data[1] = target;
	data[2] = time >> 8;
	data[3] = time;
	osDelay(5);
	Uart4_Send(servo_old_send(addr, SERVO_REG_TARGET_POSITION, SERVO_WRITE_DATA, data, &total_num, sizeof(data)), total_num);
	osDelay(100);
	return;
}
void servo_old_torque(uint8_t addr, _Bool status)
{
	uint8_t data[1];
	uint8_t total_num;
	data[0] = status;
	osDelay(5);
	Uart4_Send(servo_old_send(addr, SERVO_REG_TORQUE, SERVO_WRITE_DATA, data, &total_num, sizeof(data)), total_num);
	osDelay(100);
	return;
}
