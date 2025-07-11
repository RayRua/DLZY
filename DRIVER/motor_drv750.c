#include "motor_drv750.h"

/*********************************P01************************************/
uint8_t* DRV750_Ctr_Mode(uint8_t addr,DRV750_Control_Mode mode){
		return Modbus_Request_Write_Single_Register(addr,P01_00,mode);
}
uint8_t* DRV750_Ctr_Direction(uint8_t addr,DRV750_Direction_rotation dir){
		return Modbus_Request_Write_Single_Register(addr,P01_01,dir);
}
/*********************************P02************************************/
uint8_t* DRV750_IN_Select(uint8_t addr,DRV750_Terminal_Select trigger,DRV750_Feature_Select feature,DRV750_Logical_Select mode,uint8_t* total_num){
		uint16_t reg_addr;
		switch(trigger)
	{
		case P2_IN1:reg_addr=P02_00;break;
		case P2_IN2:reg_addr=P02_02;break;
		case P2_IN3:reg_addr=P02_04;break;
		case P2_IN4:reg_addr=P02_06;break;
		default:  
				return NULL;
	}
		uint16_t target_process[2];
		target_process[0]=feature;
		target_process[1]=mode;
		return Modbus_Request_Write_Mul_Register(addr,reg_addr,2,target_process,total_num);
}
/*********************************P03************************************/
/********************************initial************************************/
uint8_t* DRV750_Ctr_DRV750_POSITION_Cal_Ratio(uint8_t addr,uint32_t one_r_pos_com_num){
		return Modbus_Request_Write_Single_Register(addr,P03_06,one_r_pos_com_num);
}
/*********************************config************************************/
uint8_t* DRV750_Ctr_DRV750_POSITION_Set_Rec(uint8_t addr,DRV750_DRV750_POSITION_Rec_Mode rec){
		return Modbus_Request_Write_Single_Register(addr,P03_00,rec);
}

uint8_t* DRV750_Ctr_DRV750_POSITION_Set_Run_Mode(uint8_t addr, DRV750_DRV750_POSITION_Run_Mode run){
		return Modbus_Request_Write_Single_Register(addr,P03_53,run);
}

uint8_t* DRV750_Ctr_DRV750_POSITION_Set_Acc_time_ms(uint8_t addr, uint16_t tim){
		if(tim>65535)tim=65535;
		if(tim<1)tim=1;
		return Modbus_Request_Write_Single_Register(addr,P03_54,tim);
}

uint8_t* DRV750_Ctr_DRV750_POSITION_Set_Dec_time_ms(uint8_t addr, uint16_t tim){
		if(tim>65535)tim=65535;
		if(tim<1)tim=1;	
		return Modbus_Request_Write_Single_Register(addr,P03_55,tim);
}

uint8_t* DRV750_Ctr_DRV750_POSITION_Set_Run_RoSpeed(uint8_t addr, int32_t rpm){
		if(rpm>32768)rpm=32768;
		if(rpm<-32768)rpm=-32768;
		return Modbus_Request_Write_Single_Register(addr,P03_56,rpm);
}

/*********************************target************************************/
uint8_t* DRV750_Ctr_DRV750_POSITION_Expect_Target(uint8_t addr,uint32_t target,uint8_t* total_num){//mul
		uint16_t target_process[2];
		target_process[0]=target&0xFFFF;
		target_process[1]=(target>>16)&0xFFFF;
		return Modbus_Request_Write_Mul_Register(addr,P03_57,2,target_process,total_num);
}
/*********************************enable************************************/
uint8_t* DRV750_Ctr_DRV750_POSITION_Enable(uint8_t addr,DRV750_DRV750_POSITION_Enable_Mode mode){
		return Modbus_Request_Write_Single_Register(addr,P03_59,mode);
}

/*********************************P04************************************/
/*********************************config************************************/
uint8_t* DRV750_Ctr_DRV750_VELOCITY_Set_Rec_A_AND_B(uint8_t addr,DRV750_DRV750_VELOCITY_Rec_Source rec_a,DRV750_DRV750_VELOCITY_Rec_Source rec_b,uint8_t* total_num){//mul
		uint16_t target_process[2];
		target_process[0]=rec_a;
		target_process[1]=rec_b;
		return Modbus_Request_Write_Mul_Register(addr,P04_00,2,target_process,total_num);
}

uint8_t* DRV750_Ctr_DRV750_VELOCITY_Select_Rec(uint8_t addr, DRV750_DRV750_VELOCITY_Rec_Select rec){
		return Modbus_Request_Write_Single_Register(addr,P04_02,rec);
}

uint8_t* DRV750_Ctr_DRV750_VELOCITY_Set_Run_RoSpeed(uint8_t addr, int16_t rpm){
		if(rpm>6000)rpm=6000;
		if(rpm<-6000)rpm=-6000;
		return Modbus_Request_Write_Single_Register(addr,P04_03,rpm);
}

uint8_t* DRV750_Ctr_DRV750_VELOCITY_Set_Acc_time_ms(uint8_t addr, uint16_t tim){
		if(tim>65535)tim=65535;
		if(tim<1)tim=1;
		return Modbus_Request_Write_Single_Register(addr,P04_05,tim);
}

uint8_t* DRV750_Ctr_DRV750_VELOCITY_Set_Dec_time_ms(uint8_t addr, uint16_t tim){
		if(tim>65535)tim=65535;
		if(tim<1)tim=1;	
		return Modbus_Request_Write_Single_Register(addr,P04_06,tim);
}
/*********************************target************************************/
uint8_t* DRV750_Ctr_DRV750_VELOCITY_Expect_Target(uint8_t addr,uint32_t target,uint8_t* total_num){//mul
		uint16_t target_process[2];
		target_process[0]=target&0xFFFF;
		target_process[1]=(target>>16)&0xFFFF;
		return Modbus_Request_Write_Mul_Register(addr,P04_30,2,target_process,total_num);
}
//uint8_t* DRV750_Ctr_DRV750_VELOCITY_Trigger_Target(uint8_t addr,DRV750_VELOCITY_Trigger_Mode trigger){
//		return Modbus_Request_Write_Single_Register(addr,P12_10,trigger);
//}
/*********************************P13************************************/
uint8_t* DRV750_READ_STATUS(uint8_t addr){
		return Modbus_Request_Read_Register(addr,P02_07,1);
}
