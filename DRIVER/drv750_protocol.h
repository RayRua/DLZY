#ifndef __DRV750_H_
#define __DRV750_H_
/********************************include***********************************/
#include "stdint.h"
#include "stdlib.h"
/********************************typedef***********************************/
typedef struct{
				_Bool flag;//0->error code 1->effective
				uint8_t addr;
				uint8_t cmd;
				uint8_t data_length;
				uint8_t register_num;
				uint16_t data[32];
				uint8_t error_code;
}Response;
Response response_data;
/********************************define***********************************/
uint8_t* DRV750_Request_Read_Register(uint8_t addr,uint16_t register_start_addr,uint16_t register_num);
uint8_t* DRV750_Request_Write_Single_Register(uint8_t addr,uint16_t single_register_addr,uint16_t data);
uint8_t* DRV750_Request_Write_Mul_Register(uint8_t addr,uint16_t register_start_addr,uint16_t register_num,uint16_t* data,uint8_t* total_num);

_Bool DRV750_Response_Register(volatile uint8_t* response);
/*************************************************************
**																													**
**				P00 Group Servo Drive/Motor Parameters						**
**																													**
*************************************************************/
#define P00_00 0x000
#define P00_01 0x001
#define P00_02 0x002
#define P00_03 0x003
#define P00_04 0x004
#define P00_05 0x005
#define P00_06 0x006
#define P00_07 0x007
#define P00_08 0x008
#define P00_09 0x009
#define P00_10 0x00A
#define P00_17 0x011
#define P00_18 0x012
#define P00_19 0x013
#define P00_20 0x014
#define P00_21 0x015
#define P00_22 0x016
#define P00_23 0x017
#define P00_24 0x018
#define P00_25 0x019
#define P00_26 0x01A
#define P00_27 0x01B
#define P00_28 0x01C
#define P00_29 0x01D
#define P00_30 0x01E
#define P00_31 0x01F
#define P00_32 0x020
#define P00_34 0x022
#define P00_35 0x023
#define P00_36 0x024
#define P00_37 0x025
#define P00_38 0x026
#define P00_39 0x027
#define P00_40 0x028
#define P00_41 0x029
#define P00_42 0x02A
#define P00_43 0x02B
#define P00_44 0x02C
#define P00_45 0x02D
#define P00_46 0x02E
#define P00_47 0x02F
#define P00_48 0x030
#define P00_49 0x031
#define P00_50 0x032
#define P00_51 0x033
#define P00_52 0x034
#define P00_53 0x035
#define P00_54 0x036
#define P00_55 0x037
#define P00_56 0x038
#define P00_57 0x039
#define P00_58 0x03A

/*************************************************************
**																													**
**				P01 group Basic control parameters								**
**																													**
*************************************************************/
#define P01_00 0x064
#define P01_01 0x065
#define P01_02 0x066
#define P01_20 0x078
#define P01_21 0x079
#define P01_22 0x07A
#define P01_23 0x07B
#define P01_24 0x07C
#define P01_25 0x07D
#define P01_26 0x07E
#define P01_27 0x07F
#define P01_28 0x080
#define P01_29 0x081
#define P01_33 0x085
#define P01_36 0x088
#define P01_37 0x089
#define P01_42 0x08E
#define P01_43 0x08F
#define P01_44 0x090
#define P01_45 0x091
#define P01_46 0x092
#define P01_47 0x093
#define P01_48 0x094
#define P01_49 0x095

/*************************************************************
**																													**
**			P02 Group Terminal Input/Output Parameters					**
**																													**
*************************************************************/
#define P02_00 0x0C8
#define P02_01 0x0C9
#define P02_02 0x0CA
#define P02_03 0x0CB
#define P02_04 0x0CC
#define P02_05 0x0CD
#define P02_06 0x0CE
#define P02_07 0x0CF
#define P02_08 0x0D0
#define P02_09 0x0D1
#define P02_10 0x0D2
#define P02_11 0x0D3
#define P02_12 0x0D4
#define P02_13 0x0D5
#define P02_14 0x0D6
#define P02_15 0x0D7
#define P02_16 0x0D8
#define P02_17 0x0D9
#define P02_32 0x0E8
#define P02_33 0x0E9
#define P02_34 0x0EA
#define P02_35 0x0EB
#define P02_36 0x0EC
#define P02_37 0x0ED
#define P02_52 0x0FC
#define P02_53 0x0FD
#define P02_54 0x0FE
#define P02_55 0x0FF
#define P02_56 0x100
#define P02_57 0x101
#define P02_58 0x102
#define P02_59 0x103
#define P02_60 0x104
#define P02_61 0x105
#define P02_62 0x106
#define P02_63 0x107

/*************************************************************
**																													**
**			P03 Group	Position control parameters								**
**																													**
*************************************************************/
#define P03_00 0x12C
#define P03_02 0x12E
#define P03_04 0x130
#define P03_05 0x131
#define P03_06 0x132
#define P03_07 0x133
#define P03_08 0x134
#define P03_09 0x135
#define P03_10 0x136
#define P03_11 0x137
#define P03_12 0x138
#define P03_13 0x139
#define P03_14 0x13A
#define P03_15 0x13B
#define P03_20 0x140
#define P03_21 0x141
#define P03_22 0x142
#define P03_23 0x143
#define P03_24 0x144
#define P03_25 0x145
#define P03_26 0x146
#define P03_27 0x147
#define P03_28 0x148
#define P03_29 0x149
#define P03_30 0x14A
#define P03_31 0x14B
#define P03_32 0x14C
#define P03_40 0x154
#define P03_41 0x155
#define P03_42 0x156
#define P03_43 0x157
#define P03_44 0x158
#define P03_45 0x159
#define P03_46 0x15A
#define P03_47 0x15B
#define P03_49 0x15D
#define P03_50 0x15E
#define P03_51 0x15F
#define P03_52 0x160
#define P03_53 0x161
#define P03_54 0x162
#define P03_55 0x163
#define P03_56 0x164
#define P03_57 0x165
#define P03_58 0x166

/*************************************************************
**																													**
**			P04 group speed control parameters									**
**																													**
*************************************************************/
#define P04_00 0x190
#define P04_01 0x191
#define P04_02 0x192
#define P04_04 0x194
#define P04_05 0x195
#define P04_06 0x196
#define P04_07 0x197
#define P04_14 0x19E
#define P04_15 0x19F
#define P04_16 0x1A0
#define P04_17 0x1A1
#define P04_18 0x1A2
#define P04_19 0x1A3
#define P04_20 0x1A4
#define P04_21 0x1A5
#define P04_22 0x1A6
#define P04_23 0x1A7
#define P04_24 0x1A8
#define P04_25 0x1A9
#define P04_60 0x1CC
#define P04_61 0x1CD
#define P04_62 0x1CE
#define P04_63 0x1CF
#define P04_64 0x1D0
#define P04_65 0x1D1
#define P04_66 0x1D2
#define P04_67 0x1D3
#define P04_68 0x1D4
#define P04_69 0x1D5
#define P04_70 0x1D6
#define P04_71 0x1D7
#define P04_72 0x1D8
#define P04_73 0x1D9
#define P04_74 0x1DA
#define P04_75 0x1DB
#define P04_76 0x1DC
#define P04_77 0x1DD
#define P04_78 0x1DE
#define P04_79 0x1DF
#define P04_80 0x1E0
#define P04_81 0x1E1
#define P04_82 0x1E2
#define P04_83 0x1E3
#define P04_84 0x1E4
#define P04_85 0x1E5

/*************************************************************
**																													**
**			P05 Group Torque Control Parameters									**
**																													**
*************************************************************/
#define P05_00 0x1F4
#define P05_01 0x1F5
#define P05_02 0x1F6
#define P05_03 0x1F7
#define P05_04 0x1F8
#define P05_05 0x1F9
#define P05_06 0x1FA
#define P05_07 0x1FB
#define P05_08 0x1FC
#define P05_09 0x1FD
#define P05_10 0x1FE
#define P05_11 0x1FF
#define P05_12 0x200
#define P05_13 0x201
#define P05_14 0x202
#define P05_15 0x203
#define P05_16 0x204
#define P05_17 0x205
#define P05_18 0x206
#define P05_19 0x207
#define P05_20 0x208
#define P05_21 0x209
#define P05_22 0x20A
#define P05_23 0x20B
#define P05_24 0x20C
#define P05_25 0x20D
#define P05_26 0x20E
#define P05_33 0x215
#define P05_34 0x216
#define P05_35 0x217
#define P05_36 0x218
#define P05_37 0x219

/*************************************************************
**																													**
**			P06 group Gain class parameters											**
**																													**
*************************************************************/
#define P06_00 0x258
#define P06_01 0x259
#define P06_02 0x25A
#define P06_03 0x25B
#define P06_04 0x25C
#define P06_05 0x25D
#define P06_06 0x25E
#define P06_07 0x25F
#define P06_08 0x260
#define P06_09 0x261
#define P06_10 0x262
#define P06_14 0x266
#define P06_15 0x267
#define P06_16 0x268
#define P06_17 0x269
#define P06_18 0x26A
#define P06_19 0x26B
#define P06_20 0x26C
#define P06_21 0x26D
#define P06_24 0x270
#define P06_25 0x271
#define P06_26 0x272
#define P06_27 0x273
#define P06_28 0x274
#define P06_29 0x275
#define P06_30 0x276
#define P06_31 0x277
#define P06_32 0x278
#define P06_33 0x279
#define P06_34 0x27A
#define P06_35 0x27B
#define P06_36 0x27C
#define P06_37 0x27D
#define P06_38 0x27E
#define P06_39 0x27F
#define P06_40 0x280
#define P06_45 0x285
#define P06_46 0x286
#define P06_47 0x287
#define P06_48 0x288
#define P06_49 0x289
#define P06_50 0x28A
#define P06_51 0x28B
#define P06_52 0x28C
#define P06_53 0x28D

/*************************************************************
**																													**
**			P08 Group Communication Parameters									**
**																													**
*************************************************************/
#define P08_00 0x320
#define P08_01 0x321
#define P08_02 0x322
#define P08_05 0x325
#define P08_06 0x326
#define P08_07 0x327
#define P08_08 0x328
#define P08_09 0x329
#define P08_10 0x32A
#define P08_30 0x33E
#define P08_31 0x33F
#define P08_32 0x340

/*************************************************************
**																													**
**			P09 group Multi-segment positional parameters				**
**																													**
*************************************************************/
#define P09_00 0x384
#define P09_01 0x385
#define P09_02 0x386
#define P09_03 0x387
#define P09_04 0x388
#define P09_12 0x390
#define P09_13 0x391
#define P09_14 0x392
#define P09_15 0x393
#define P09_16 0x394
#define P09_17 0x395
#define P09_18 0x396
#define P09_19 0x397
#define P09_20 0x398
#define P09_21 0x399
#define P09_22 0x39A
#define P09_23 0x39B
#define P09_24 0x39C
#define P09_25 0x39D
#define P09_26 0x39E
#define P09_27 0x39F
#define P09_28 0x3A0
#define P09_29 0x3A1
#define P09_30 0x3A2
#define P09_31 0x3A3
#define P09_32 0x3A4
#define P09_33 0x3A5
#define P09_34 0x3A6
#define P09_35 0x3A7
#define P09_36 0x3A8
#define P09_37 0x3A9
#define P09_38 0x3AA
#define P09_39 0x3AB
#define P09_40 0x3AC
#define P09_41 0x3AD
#define P09_42 0x3AE
#define P09_43 0x3AF
#define P09_44 0x3B0
#define P09_45 0x3B1
#define P09_46 0x3B2
#define P09_47 0x3B3
#define P09_48 0x3B4
#define P09_49 0x3B5
#define P09_50 0x3B6
#define P09_51 0x3B7
#define P09_52 0x3B8
#define P09_53 0x3B9
#define P09_54 0x3BA
#define P09_55 0x3BB
#define P09_56 0x3BC
#define P09_57 0x3BD
#define P09_58 0x3BE
#define P09_59 0x3BF
#define P09_60 0x3C0
#define P09_61 0x3C1
#define P09_62 0x3C2
#define P09_63 0x3C3
#define P09_64 0x3C4
#define P09_65 0x3C5
#define P09_66 0x3C6
#define P09_67 0x3C7
#define P09_68 0x3C8
#define P09_69 0x3C9
#define P09_70 0x3CA
#define P09_71 0x3CB
#define P09_72 0x3CC
#define P09_73 0x3CD
#define P09_74 0x3CE
#define P09_75 0x3CF
#define P09_76 0x3D0
#define P09_77 0x3D1
#define P09_78 0x3D2
#define P09_79 0x3D3
#define P09_80 0x3D4
#define P09_81 0x3D5
#define P09_82 0x3D6
#define P09_83 0x3D7
#define P09_84 0x3D8
#define P09_85 0x3D9
#define P09_86 0x3DA
#define P09_87 0x3DB
#define P09_88 0x3DC
#define P09_89 0x3DD
#define P09_90 0x3DE
#define P09_91 0x3DF

/*************************************************************
**																													**
**			P10 group Multi-segment velocity parameters					**
**																													**
*************************************************************/
#define P10_00 0x3E8
#define P10_01 0x3E9
#define P10_02 0x3EA
#define P10_03 0x3EB
#define P10_04 0x3EC
#define P10_05 0x3ED
#define P10_06 0x3EE
#define P10_07 0x3EF
#define P10_08 0x3F0
#define P10_09 0x3F1
#define P10_10 0x3F2
#define P10_11 0x3F3
#define P10_12 0x3F4
#define P10_13 0x3F5
#define P10_14 0x3F6
#define P10_15 0x3F7
#define P10_16 0x3F8
#define P10_20 0x3FC
#define P10_21 0x3FD
#define P10_22 0x3FE
#define P10_23 0x3FF
#define P10_24 0x400
#define P10_25 0x401
#define P10_26 0x402
#define P10_27 0x403
#define P10_28 0x404
#define P10_29 0x405
#define P10_30 0x406
#define P10_31 0x407
#define P10_32 0x408
#define P10_33 0x409
#define P10_34 0x40A
#define P10_35 0x40B
#define P10_36 0x40C
#define P10_37 0x40D
#define P10_38 0x40E
#define P10_39 0x40F
#define P10_40 0x410
#define P10_41 0x411
#define P10_42 0x412
#define P10_43 0x413
#define P10_44 0x414
#define P10_45 0x415
#define P10_46 0x416
#define P10_47 0x417
#define P10_48 0x418
#define P10_49 0x419
#define P10_50 0x41A
#define P10_51 0x41B
#define P10_52 0x41C
#define P10_53 0x41D
#define P10_54 0x41E
#define P10_55 0x41F
#define P10_56 0x420
#define P10_57 0x421
#define P10_58 0x422
#define P10_59 0x423
#define P10_60 0x424
#define P10_61 0x425
#define P10_62 0x426
#define P10_63 0x427
#define P10_64 0x428
#define P10_65 0x429
#define P10_66 0x42A
#define P10_67 0x42B

/*************************************************************
**																													**
**			P11 group Auxiliary display													**
**																													**
*************************************************************/
#define P11_00 0x44C
#define P11_01 0x44D
#define P11_02 0x44E
#define P11_03 0x44F
#define P11_04 0x450
#define P11_05 0x451
#define P11_06 0x452
#define P11_07 0x453
#define P11_08 0x454
#define P11_09 0x455
#define P11_10 0x456
#define P11_11 0x457
#define P11_12 0x458
#define P11_13 0x459
#define P11_14 0x45A
#define P11_15 0x45B
#define P11_16 0x45C
#define P11_17 0x45D
#define P11_18 0x45E
#define P11_19 0x45F
#define P11_20 0x460
#define P11_21 0x461
#define P11_22 0x462
#define P11_23 0x463
#define P11_24 0x464
#define P11_25 0x465
#define P11_26 0x466
#define P11_27 0x467
#define P11_28 0x468
#define P11_29 0x469
#define P11_30 0x46A
#define P11_31 0x46B
#define P11_32 0x46C
#define P11_33 0x46D
#define P11_34 0x46E
#define P11_35 0x46F
#define P11_36 0x470
#define P11_37 0x471
#define P11_38 0x472
#define P11_39 0x473
#define P11_40 0x474
#define P11_41 0x475
#define P11_42 0x476
#define P11_43 0x477
#define P11_44 0x478
#define P11_45 0x479
#define P11_46 0x47A
#define P11_47 0x47B
#define P11_48 0x47C
#define P11_49 0x47D
#define P11_50 0x47E
#define P11_51 0x47F
#define P11_52 0x480
#define P11_53 0x481
#define P11_54 0x482
#define P11_55 0x483
#define P11_56 0x484
#define P11_57 0x485
#define P11_58 0x486
#define P11_59 0x487
#define P11_60 0x488
#define P11_61 0x489
#define P11_62 0x48A
#define P11_63 0x48B
#define P11_64 0x48C
#define P11_65 0x48D
#define P11_66 0x48E
#define P11_67 0x48F
#define P11_68 0x490
#define P11_69 0x491
#define P11_70 0x492
#define P11_71 0x493
#define P11_72 0x494
#define P11_73 0x495
#define P11_74 0x496
#define P11_75 0x497
#define P11_76 0x498
#define P11_77 0x499
#define P11_78 0x49A
#define P11_79 0x49B
#define P11_80 0x49C
#define P11_81 0x49D
#define P11_82 0x49E
#define P11_83 0x49F
#define P11_84 0x4A0
#define P11_85 0x4A1
#define P11_86 0x4A2
#define P11_87 0x4A3
#define P11_88 0x4A4
#define P11_89 0x4A5
#define P11_90 0x4A6
#define P11_91 0x4A7
#define P11_92 0x4A8
#define P11_93 0x4A9
#define P11_94 0x4AA
#define P11_95 0x4AB
#define P11_96 0x4AC

/*************************************************************
**																													**
**			P12 group accessibility															**	
**																													**
*************************************************************/
#define P12_00 0x4B0
#define P12_01 0x4B1
#define P12_02 0x4B2
#define P12_03 0x4B3
#define P12_04 0x4B4
#define P12_05 0x4B5
#define P12_06 0x4B6
#define P12_07 0x4B7
#define P12_08 0x4B8
#define P12_09 0x4B9
#define P12_10 0x4BA
#define P12_11 0x4BB
#define P12_12 0x4BC
#define P12_13 0x4BD
#define P12_14 0x4BE
#define P12_15 0x4BF
#define P12_16 0x4C0
#define P12_17 0x4C1
#define P12_18 0x4C2
#define P12_19 0x4C3
#define P12_20 0x4C4
#define P12_21 0x4C5

/*************************************************************
**																													**
**			P13 group monitoring parameters											**
**																													**
*************************************************************/
#define P13_00 0x514
#define P13_01 0x515
#define P13_02 0x516
#define P13_03 0x517
#define P13_04 0x518
#define P13_05 0x519
#define P13_06 0x51A
#define P13_07 0x51B
#define P13_08 0x51C
#define P13_09 0x51D
#define P13_10 0x51E
#define P13_11 0x51F
#define P13_12 0x520
#define P13_13 0x521
#define P13_14 0x522
#define P13_15 0x523
#define P13_16 0x524
#define P13_17 0x525
#define P13_18 0x526
#define P13_19 0x527
#define P13_20 0x528
#define P13_21 0x529
#define P13_22 0x52A
#define P13_23 0x52B
#define P13_24 0x52C
#define P13_25 0x52D
#define P13_26 0x52E
#define P13_27 0x52F
#define P13_28 0x530
#define P13_29 0x531
#define P13_30 0x532
#define P13_31 0x533
#define P13_32 0x534
#define P13_33 0x535
#define P13_36 0x538
#define P13_40 0x53C
#define P13_41 0x53D
#define P13_42 0x53E
#define P13_43 0x53F

/**********************************CMD_Error_coding****************************************/
#define ERROR_FEATURE 0x01
#define ERROR_ADDRESS 0x02
#define ERROR_DATA 0x03
#define ERROR_SLAVE 0x04
/************************************CMD_coding********************************************/
#define CMD_READ_REGISTER 0x03
#define CMD_WRITE_SINGLE_DATA 0x06
#define CMD_WRITE_MULTIPLE_DATA 0x10

/*********************************CRC16************************************/
// unsigned short Modbus_CRC16(volatile uint8_t* pDataBuffer, unsigned long usDataLen);
unsigned short Modbus_CRC16_List(volatile unsigned char* pDataBuffer, unsigned long usDataLen);
#endif
