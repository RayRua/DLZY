#ifndef __SERVO_H_
#define __SERVO_H_
/********************************include***********************************/
#include "Modbus_RTU_protocol.h"
/********************************typedef***********************************/
typedef enum
{
	SERVO_PING = 0X01,
	SERVO_READ_DATA,
	SERVO_WRITE_DATA,
	SERVO_REGWRITE,
	SERVO_ACTION,
	SERVO_RESET,
	SERVO_SYNC_WRITE = 0X83
} SERVO_REQUEST_TYPE;
/********************************define***********************************/

#define SERVO_REG_TARGET_POSITION 0x2A
#define SERVO_REG_TORQUE 0x28
#define SERVO_TORQUE_ON 0x01
#define SERVO_TORQUE_OFF 0x00

void servo_target(uint8_t addr, uint16_t target);
void servo_old_target(uint8_t addr, uint16_t target, uint16_t time);
void servo_old_torque(uint8_t addr, _Bool status);
#endif
