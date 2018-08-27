#ifndef __APP_MPU9250_H
#define __APP_MPU9250_H

#include "Common.h"
#include "BSP_IIC.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//24CXX驱动 代码(适合24C01~24C16,24C32~256未经过测试!有待验证!)		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

// 定义MPU9250内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			  0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
//#define ACCEL_CONFIG2 0x1c  //加速度计测量范围 0x18 +-16g
#define	ACCEL_XOUT_H	0x3B  //加速度计输出数据
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41  //温度计输出数据
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43  //陀螺仪输出数据
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03  //磁力计输出数据
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08


#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		  0x71	//IIC地址寄存器(默认数值0x68，只读)//0x71//0x75


//****************************
#define	GYRO_ADDRESS   0xD0	  //陀螺地址
#define MAG_ADDRESS    0x18   //磁场地址
#define ACCEL_ADDRESS  0xD0   ///////////////////////////////

//////////////////////////////





/////////////////////////////////////////////////////////////////////////////////

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	  8191
#define AT24C128	16383
#define AT24C256	32767  
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C02



extern short MPU9250_Data[9];
extern int TEMP[9];
void SplitByte(int *Original,u8 *Output);
extern unsigned char MPU9250_OutData[36];

extern float TEMP_Uart[3];
extern unsigned char MPU9250_OutData_Uart[12];
/************************************************************/

void AT24CXX_Init(void);

u8 AT24CXX_ReadOneByte(u8 SlaveAddr, u16 ReadAddr);                  // Read  1 Byte
void AT24CXX_WriteOneByte(u8 SlaveAddr,u16 WriteAddr,u8 DataToWrite);// Write 1 Byte

void AT24CXX_WriteLenByte(u8 SlaveAddr, u16 WriteAddr,u32 DataToWrite,u8 Len);// Write Len Byte
u32 AT24CXX_ReadLenByte(u8 SlaveAddr, u16 ReadAddr,u8 Len);                   // Read Len Byte

u8 AT24CXX_Check(void); //Check AT24C02

void AT24CXX_Read(u8 SlaveAddr, u16 ReadAddr,u8 *pBuffer,u16 NumToRead); //Read 
void AT24CXX_Write(u8 SlaveAddr, u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);//Write

/************************************************************/
void Init_MPU9250(void);
void READ_MPU9250_ACCEL(void);
void READ_MPU9250_GYRO(void);
void READ_MPU9250_MAG(void);
void Robort_Get_MPU9250(void);


#endif


