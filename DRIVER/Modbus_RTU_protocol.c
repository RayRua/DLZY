#include "Modbus_RTU_protocol.h"
#include "string.h"
#include "motor_status.h"
/*********************************global************************************/
static uint8_t request_yaw[32];
static uint8_t request_pitch[32];
uint8_t response_yaw[16];
uint8_t response_pitch[16];
/*********************************CRC16************************************/
unsigned short crc16Table[256]={0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
															0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
															0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
															0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
															0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
															0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
															0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
															0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
															0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
															0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
															0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
															0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
															0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
															0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
															0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
															0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
															0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
															0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
															0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
															0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
															0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
															0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
															0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
															0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
															0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
															0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
															0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
															0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
															0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
															0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
															0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
															0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040,}; 

//static unsigned short Modbus_CRC16(volatile uint8_t* pDataBuffer, unsigned long usDataLen) {
//    const unsigned short POLYNOMIAL = 0xA001;
//    unsigned short wCrc;
//    int iBite, iBit;
//    wCrc = 0xFFFF;
//    for(iBite = 0; iBite < usDataLen; iBite++) {
//        wCrc ^= * (pDataBuffer + iBite);
//        for(iBit = 0; iBit <= 7; iBit++) {
//            if(wCrc & 0x0001) {
//                wCrc >>= 1;
//                wCrc ^= POLYNOMIAL;
//            } else {
//                wCrc >>= 1;
//            }
//        }
//    }
//    return wCrc;
//}
static unsigned short Modbus_CRC16_List(volatile unsigned char* pDataBuffer, unsigned long usDataLen) {
    unsigned short wCrc = 0xFFFF; // initial
    for (unsigned long iByte = 0; iByte < usDataLen; iByte++) {
        unsigned char index = (wCrc ^ *pDataBuffer++) & 0xFF; // calculate index
        wCrc = (wCrc >> 8) ^ crc16Table[index]; // ListCRC
    }
    return wCrc;
}

uint16_t CRC16_Check_Sum(uint8_t* pchMessage, uint32_t dwLength)
{
    uint16_t wExpected = 0;
    if ((pchMessage == 0) || (dwLength <= 2))
    {
        return 0;
    }
    wExpected = Modbus_CRC16_List(pchMessage, dwLength - 2);
    return (((wExpected & 0xff) == pchMessage[dwLength - 1]) &&
        (((wExpected >> 8) & 0xff) == pchMessage[dwLength - 2]));
}
/*********************************CRC16************************************/
/*****************************Modbus_REQUEST************************************/

//had tested
uint8_t* Modbus_Request_Read_Register(uint8_t addr,uint16_t register_start_addr,uint16_t register_num){
		if(addr==GIMBAL_YAW){
			request_yaw[0+16]=  addr;
			request_yaw[1+16]=  CMD_READ_REGISTER;					//0x03
			request_yaw[2+16]= (register_start_addr>>8)&0xFF;
			request_yaw[3+16]=  register_start_addr&0xFF;
			request_yaw[4+16]= (register_num>>8)&0xFF;
			request_yaw[5+16]=  register_num&0xFF;
		uint16_t crc = Modbus_CRC16_List(request_yaw+16, 6);
			request_yaw[6+16]=  crc&0xFF;
			request_yaw[7+16]=  (crc>>8)&0xFF;
			return request_yaw+16;
		}
		else {
			request_pitch[0+16]=  addr;
			request_pitch[1+16]=  CMD_READ_REGISTER;					//0x03
			request_pitch[2+16]= (register_start_addr>>8)&0xFF;
			request_pitch[3+16]=  register_start_addr&0xFF;
			request_pitch[4+16]= (register_num>>8)&0xFF;
			request_pitch[5+16]=  register_num&0xFF;
		uint16_t crc = Modbus_CRC16_List(request_pitch+16, 6);
			request_pitch[6+16]=  crc&0xFF;
			request_pitch[7+16]=  (crc>>8)&0xFF;
			return request_pitch+16;
		}
}
//had tested
uint8_t* Modbus_Request_Write_Single_Register(uint8_t addr,uint16_t single_register_addr,uint16_t data){
			if(addr==GIMBAL_YAW){
			request_yaw[0]=  addr;
			request_yaw[1]=  CMD_WRITE_SINGLE_DATA;			//0x06
			request_yaw[2]= (single_register_addr>>8)&0xFF;
			request_yaw[3]=  single_register_addr&0xFF;
			request_yaw[4]= (data>>8)&0xFF;
			request_yaw[5]=  data&0xFF;
		uint16_t crc = Modbus_CRC16_List(request_yaw, 6);
			request_yaw[6]=  crc&0xFF;
			request_yaw[7]=  (crc>>8)&0xFF;
			return request_yaw;
				}
			else {
			request_pitch[0]=  addr;
			request_pitch[1]=  CMD_WRITE_SINGLE_DATA;			//0x06
			request_pitch[2]= (single_register_addr>>8)&0xFF;
			request_pitch[3]=  single_register_addr&0xFF;
			request_pitch[4]= (data>>8)&0xFF;
			request_pitch[5]=  data&0xFF;
		uint16_t crc = Modbus_CRC16_List(request_pitch, 6);
			request_pitch[6]=  crc&0xFF;
			request_pitch[7]=  (crc>>8)&0xFF;
			return request_pitch;
		}
}
//had tested
uint8_t* Modbus_Request_Write_Mul_Register(uint8_t addr,uint16_t register_start_addr,uint16_t register_num,uint16_t* data,uint8_t* total_num){
		uint8_t data_length=register_num*2;			//9+N*2
		*total_num=1+1+2+2+1+data_length;//without crc
			if(addr==GIMBAL_YAW){
			request_yaw[0]=  addr;
			request_yaw[1]=  CMD_WRITE_MULTIPLE_DATA;		//0x10
			request_yaw[2]= (register_start_addr>>8)&0xFF;
			request_yaw[3]=  register_start_addr&0xFF;
			request_yaw[4]= (register_num>>8)&0xFF;
			request_yaw[5]=  register_num&0xFF;
			request_yaw[6]=  data_length;	
		uint8_t i=0;
			for(;i<register_num;i++){
				request_yaw[7+i*2]=  (data[i]>>8)&0xFF;
				request_yaw[7+i*2+1]=  data[i]&0xFF;
			}
		uint16_t crc = Modbus_CRC16_List(request_yaw, *total_num);
			request_yaw[7+i*2]=  crc&0xFF;
			request_yaw[7+i*2+1]=  (crc>>8)&0xFF;
			*total_num+=2;
			return request_yaw;
			}
			else {
			request_pitch[0]=  addr;
			request_pitch[1]=  CMD_WRITE_MULTIPLE_DATA;		//0x10
			request_pitch[2]= (register_start_addr>>8)&0xFF;
			request_pitch[3]=  register_start_addr&0xFF;
			request_pitch[4]= (register_num>>8)&0xFF;
			request_pitch[5]=  register_num&0xFF;
			request_pitch[6]=  data_length;	
		uint8_t i=0;
			for(;i<register_num;i++){
				request_pitch[7+i*2]=  (data[i]>>8)&0xFF;
				request_pitch[7+i*2+1]=  data[i]&0xFF;
			}
		uint16_t crc = Modbus_CRC16_List(request_pitch, *total_num);
			request_pitch[7+i*2]=  crc&0xFF;
			request_pitch[7+i*2+1]=  (crc>>8)&0xFF;
			*total_num+=2;
			return request_pitch;
			}
}
/*****************************Modbus_REQUEST************************************/
/*****************************Modbus_RESPONSE************************************/
//had tested
//static uint8_t s_ucWitDataBuff[256];
//static uint32_t s_uiWitDataCnt = 0,\
//								s_uiProtoclo = 0,\
//								s_uiReadRegIndex = 0;
//int16_t sReg[0x90];
void Modbus_Response_Register(){
		
		if(response_yaw[1]==0x03){
			switch((DRV750_Control_Mode)(((uint16_t)response_yaw[3] << 8) | response_yaw[4])){
				case DRV750_POSITION: yaw_current_status.mode =DRV750_POSITION ; break;
				case DRV750_VELOCITY:	yaw_current_status.mode =DRV750_VELOCITY ; break;
				default: break;
			
			}
		}
		if(response_pitch[1]==0x03){			
			switch((D_AI_Control_Mode)(((uint16_t)response_pitch[3] << 8) | response_pitch[4])){
				case DRV750_POSITION: pitch_current_status.mode =	D_AI_VELOCITY; break;
				case DRV750_VELOCITY:	pitch_current_status.mode =	D_AI_POSITION; break;
				default: break;
			
			}
		}
		
}




/*****************************Modbus_RESPONSE************************************/
/*********************************Modbus************************************/
