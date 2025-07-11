#include "detect_task.h"
#include "motor_config.h"
#include "pin.h"
#include "cmsis_os.h"

#include "gimbal_task.h"
extern DRV750_Current_Status_t yaw_current_status;
extern D_AI_Current_Status_t pitch_current_status;
uint8_t pd4_level;
uint8_t pd5_level;



void detect_task()
	{
				pd4_level = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6);
				pd5_level	= GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5);
	}
 

