#include "drv750_control.h"
/*********************************P01************************************/
void DRV750_Ctr_Mode(uint8_t addr,Control_Mode mode){
		DRV750_Request_Write_Single_Register(addr,P01_00,mode);
}
void DRV750_Ctr_Direction(uint8_t addr,Direction_rotation dir){
		DRV750_Request_Write_Single_Register(addr,P01_01,dir);
}
/*********************************P03************************************/
/********************************initial************************************/
void DRV750_Ctr_Position_Cal_Ratio(uint8_t addr,uint32_t one_r_pos_com_num){
		uint16_t process[2];
		process[0]=one_r_pos_com_num&0xFFFF;
		process[1]=one_r_pos_com_num>>16&0xFFFF;
		uint8_t* total_num;
		DRV750_Request_Write_Mul_Register(addr,P03_06,2,process,total_num);
}
/*********************************config************************************/
void DRV750_Ctr_Position_Set_Rec(uint8_t addr,Position_Rec_Mode rec){
		DRV750_Request_Write_Single_Register(addr,P03_00,rec);
}

void DRV750_Ctr_Position_Set_Run_Mode(uint8_t addr, Position_Run_Mode run){
		DRV750_Request_Write_Single_Register(addr,P03_53,run);
}

void DRV750_Ctr_Position_Set_Acc_time_ms(uint8_t addr, uint16_t tim){
		if(tim>65535)tim=65535;
		if(tim<1)tim=1;
		DRV750_Request_Write_Single_Register(addr,P03_54,tim);
}

void DRV750_Ctr_Position_Set_Dec_time_ms(uint8_t addr, uint16_t tim){
		if(tim>65535)tim=65535;
		if(tim<1)tim=1;	
		DRV750_Request_Write_Single_Register(addr,P03_55,tim);
}

void DRV750_Ctr_Position_Set_Run_RoSpeed(uint8_t addr, uint16_t rpm){
		if(rpm>6000)rpm=6000;
		DRV750_Request_Write_Single_Register(addr,P03_56,rpm);
}
/*********************************target************************************/
void DRV750_Ctr_Position_Expect_Target(uint8_t addr,uint32_t target){
		uint16_t target_process[2];
		target_process[0]=target&0xFFFF;
		target_process[1]=(target>>16)&0xFFFF;
		uint8_t* total_num;
		DRV750_Request_Write_Mul_Register(addr,P03_57,2,target_process,total_num);
}
/*********************************P04************************************/
/*********************************config************************************/
void DRV750_Ctr_Velocity_Set_Rec(uint8_t addr,Velocity_Rec_Mode rec){
		DRV750_Request_Write_Single_Register(addr,P04_00,rec);
}

void DRV750_Ctr_Velocity_Set_Run_RoSpeed(uint8_t addr, uint16_t rpm){
		if(rpm>6000)rpm=6000;
		DRV750_Request_Write_Single_Register(addr,P04_62,rpm);
}

void DRV750_Ctr_Velocity_Set_Acc_time_ms(uint8_t addr, uint16_t tim){
		if(tim>65535)tim=65535;
		if(tim<1)tim=1;
		DRV750_Request_Write_Single_Register(addr,P04_63,tim);
}

void DRV750_Ctr_Velocity_Set_Dec_time_ms(uint8_t addr, uint16_t tim){
		if(tim>65535)tim=65535;
		if(tim<1)tim=1;	
		DRV750_Request_Write_Single_Register(addr,P04_64,tim);
}
/*********************************target************************************/
void DRV750_Ctr_Velocity_Expect_Target(uint8_t addr,uint32_t target){
		uint16_t target_process[2];
		target_process[0]=target&0xFFFF;
		target_process[1]=(target>>16)&0xFFFF;
		uint8_t* total_num;
		DRV750_Request_Write_Mul_Register(addr,P04_60,2,target_process,total_num);
}
void DRV750_Ctr_Velocity_Trigger_Target(uint8_t addr,Velocity_Trigger_Mode trigger){
		DRV750_Request_Write_Single_Register(addr,P12_10,trigger);
}
/*********************************P13************************************/