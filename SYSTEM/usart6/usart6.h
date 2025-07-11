#ifndef __USART6_H
#define __USART6_H

// Define STM32 device type before including stm32f4xx.h
#ifndef STM32F40_41xxx
#define STM32F40_41xxx
#endif



#define RX_BUF_SIZE 64
// Modbus�ӻ���ַ������ʵ���޸ģ�
#define DRV750_ADDRESS      0x01

// �Ĵ�����ַ��DRV750������
#define TARGET_POS_HI_REG   0x0165  // P03.57��Ŀ��λ�ø�16λ��
#define TARGET_POS_LO_REG   0x00E1  // P03.58��Ŀ��λ�õ�16λ��
#define CURRENT_POS_HI_REG  0x0087  // P13.07����ǰλ�ø�16λ��
#define CURRENT_POS_LO_REG  0x0088  // P13.08����ǰλ�õ�16λ��

// RS485����������ţ�����Ӳ�������޸ģ�
#define RS485_DIR_GPIO      GPIOA
#define RS485_DIR_PIN       GPIO_Pin_8

// ��������
void USART6_Init(uint32_t bound);
void DRV750_Init(void);
void DRV750_SetTargetPosition(int32_t Position);
int32_t DRV750_GetCurrentPosition(void);
void RS485_Send(uint8_t* data, uint32_t len);

void Set_N_DuoJi_Position(uint16_t Position_Data1, uint16_t Position_Time1, uint16_t Position_Data2, uint16_t Position_Time2, uint16_t Position_Data3, uint16_t Position_Time3, uint16_t Position_Data4, uint16_t Position_Time4, uint16_t Position_Data5, uint16_t Position_Time5, uint16_t Position_Data6, uint16_t Position_Time6, uint16_t Position_Data7, uint16_t Position_Time7);
#endif



