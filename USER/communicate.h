#ifndef ___COMMUNICATE_H___
#define ___COMMUNICATE_H___
#include "stm32f4xx.h"
typedef enum
{
	USART,
	ETHERNET,
	CAN,
} PORT;

#define COMMUNICATE_BUF 256

typedef struct
{
	unsigned char rx_buf[COMMUNICATE_BUF];
	unsigned char tx_buf[COMMUNICATE_BUF];
	// unsigned char tmp_buf[COMMUNICATE_BUF];
	float tmp_float;
	short int tmp_int;
	unsigned char rx_wr_index;
	unsigned char rx_wr_size;
	unsigned char checksum;
	unsigned char txmit_task_active;
	unsigned char txmit_len;
	unsigned long message_count;
	unsigned char NewDataArrive;

	PORT DataPort;
} COMMUNICATE;
/*******************************/
typedef enum
{
	MASTER_VELOCITY = 0,
	MASTER_POSITION,
	MASTER_NULL
} Master_Status;
typedef struct
{
	float yaw;
	float pitch;
	_Bool left_shoot;
	_Bool right_shoot;
	Master_Status master_status;
} Gimbal_Master;
void gimbal_protocol(COMMUNICATE *p);
extern Gimbal_Master gimbal_master;
extern _Bool master_receive_flag;
/*******************************/
extern COMMUNICATE uart1, can1;

extern void SpacifyCharacter(COMMUNICATE *p);
extern void CommunicateInit(COMMUNICATE *p);
extern unsigned char sumCheck(COMMUNICATE *p);
extern void CommunicateEntry(void);
extern u8 DecodeFrame1By1(unsigned char newdata, COMMUNICATE *p);
extern u8 DecodeFrameAllIN1(COMMUNICATE *p);

void Sent_Date_PC(void);
void Get_PC_Date(void);

#endif
