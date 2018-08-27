#include "APP_MPU9250.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//24CXX驱动 代码(适合24C01~24C16,24C32~256未经过测试!有待验证!)		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define AT24C02_ADDR 0xA0
unsigned char BUF[10] = {10,10,10,10,0,0,0,0,0,0};       //接收数据缓存区
short T_X,T_Y,T_Z,T_T;		 //X,Y,Z轴，温度
short MPU9250_Data[9]; // ACCEL_xyz    GYRO_xyz    MAG_xyz
int TEMP[9];
unsigned char MPU9250_OutData[36];
float TEMP_Uart[3];
unsigned char MPU9250_OutData_Uart[12];

/******************************************************
***** 
***** 初始化IIC接口
***** 
******************************************************/
void AT24CXX_Init(void)
{
	IIC_Init();
}

/******************************************************
***** 在AT24CXX指定地址读出一个数据
***** ReadAddr:开始读数的地址  
***** 返回值  :读到的数据
******************************************************/
u8 AT24CXX_ReadOneByte(u8 SlaveAddr, u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(SlaveAddr);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址
		IIC_Wait_Ack();		 
	}else IIC_Send_Byte(SlaveAddr+((ReadAddr/256)<<1));   //发送器件地址,写数据 	 

	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(SlaveAddr+1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}

/******************************************************
***** 在AT24CXX指定地址写入一个数据
***** WriteAddr  :写入数据的目的地址  
***** DataToWrite:要写入的数据
******************************************************/
void AT24CXX_WriteOneByte(u8 SlaveAddr,u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(SlaveAddr);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址
 	}else
	{
		IIC_Send_Byte(SlaveAddr+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
	}	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 //delay_ms(10);	
}

/******************************************************
***** 在AT24CXX里面的指定地址开始写入长度为Len的数据
***** 该函数用于写入16bit或者32bit的数据
***** WriteAddr  :开始写入的地址  
***** DataToWrite:数据数组首地址
***** Len        :要写入数据的长度2,4
******************************************************/
void AT24CXX_WriteLenByte(u8 SlaveAddr, u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(SlaveAddr, WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

/******************************************************
***** 在AT24CXX里面的指定地址开始读出长度为Len的数据
***** 该函数用于读出16bit或者32bit的数据.
***** ReadAddr   :开始读出的地址 
***** 返回值     :数据
***** Len        :要读出数据的长度2,4
***** 
***** 
******************************************************/
u32 AT24CXX_ReadLenByte(u8 SlaveAddr, u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(SlaveAddr, ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
/******************************************************
***** 检查AT24CXX是否正常
***** 这里用了24XX的最后一个地址(255)来存储标志字.
***** 如果用其他24C系列,这个地址要修改
***** 返回1:检测失败
***** 返回0:检测成功
***** 
*****
******************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(AT24C02_ADDR, 255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(AT24C02_ADDR, 255,0X55);
	    temp=AT24CXX_ReadOneByte(AT24C02_ADDR, 255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

/******************************************************
***** 在AT24CXX里面的指定地址开始读出指定个数的数据
***** ReadAddr :开始读出的地址 对24c02为0~255
***** pBuffer  :数据数组首地址
***** NumToRead:要读出数据的个数
***** 
***** 
***** 
*****
******************************************************/
void AT24CXX_Read(u8 SlaveAddr, u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(SlaveAddr, ReadAddr++);	
		NumToRead--;
	}
}  

/******************************************************
***** 在AT24CXX里面的指定地址开始写入指定个数的数据
***** WriteAddr :开始写入的地址 对24c02为0~255
***** pBuffer   :数据数组首地址
***** NumToWrite:要写入数据的个数
***** 
***** 
*****
******************************************************/
void AT24CXX_Write(u8 SlaveAddr, u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(SlaveAddr, WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 

/******************************************************
***** 功能：初始化MPU9250
***** 
***** 
***** 
***** 
***** 
*****
******************************************************/
void Init_MPU9250(void)
{
/*
   Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
   Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
   Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //±2000°
   Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
   Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
*/
  AT24CXX_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x00);	//解除休眠状态
	AT24CXX_WriteOneByte(GYRO_ADDRESS,SMPLRT_DIV, 0x07);  //陀螺仪采样率
	AT24CXX_WriteOneByte(GYRO_ADDRESS,CONFIG, 0x06);      //低通滤波器
	AT24CXX_WriteOneByte(GYRO_ADDRESS,GYRO_CONFIG, 0x18); //
	AT24CXX_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG, 0x01);//
  //----------------
//	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode	

}


/******************************************************
***** 读取MPU9250数据
***** 
***** 
***** 
***** 
***** 
*****
******************************************************/

void READ_MPU9250_ACCEL(void)
{ 

   BUF[0]=AT24CXX_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_L); 
   BUF[1]=AT24CXX_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_H);
   MPU9250_Data[0]=	(BUF[1]<<8)|BUF[0];
   MPU9250_Data[0]/=164; 						   //读取计算X轴数据

   BUF[2]=AT24CXX_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_L);
   BUF[3]=AT24CXX_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_H);
   MPU9250_Data[1]=	(BUF[3]<<8)|BUF[2];
   MPU9250_Data[1]/=164; 						   //读取计算Y轴数据
   BUF[4]=AT24CXX_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_L);
   BUF[5]=AT24CXX_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_H);
   MPU9250_Data[2]=	(BUF[5]<<8)|BUF[4];
   MPU9250_Data[2]/=164; 					      //读取计算Z轴数据
 
}

void READ_MPU9250_GYRO(void)
{ 

   BUF[0]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_L); 
   BUF[1]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_H);
   MPU9250_Data[3]=	(BUF[1]<<8)|BUF[0];
   MPU9250_Data[3]/=16.4; 						   //读取计算X轴数据

   BUF[2]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_L);
   BUF[3]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_H);
   MPU9250_Data[4]=	(BUF[3]<<8)|BUF[2];
   MPU9250_Data[4]/=16.4; 						   //读取计算Y轴数据
   BUF[4]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_L);
   BUF[5]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_H);
   MPU9250_Data[5]=	(BUF[5]<<8)|BUF[4];
   MPU9250_Data[5]/=16.4; 					       //读取计算Z轴数据
 
 
  // BUF[6]=Single_Read(GYRO_ADDRESS,TEMP_OUT_L); 
  // BUF[7]=Single_Read(GYRO_ADDRESS,TEMP_OUT_H); 
  // T_T=(BUF[7]<<8)|BUF[6];
  // T_T = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
}
void READ_MPU9250_MAG(void)
{ 
   AT24CXX_WriteOneByte(GYRO_ADDRESS,0x37,0x02);//turn on Bypass Mode 
   delay_ms(10);	
   AT24CXX_WriteOneByte(MAG_ADDRESS,0x0A,0x01);
   delay_ms(10);	
   BUF[0]=AT24CXX_ReadOneByte (MAG_ADDRESS,MAG_XOUT_L);
   BUF[1]=AT24CXX_ReadOneByte (MAG_ADDRESS,MAG_XOUT_H);
   MPU9250_Data[6] = (BUF[1]<<8)|BUF[0];

   BUF[2]=AT24CXX_ReadOneByte(MAG_ADDRESS,MAG_YOUT_L);
   BUF[3]=AT24CXX_ReadOneByte(MAG_ADDRESS,MAG_YOUT_H);
   MPU9250_Data[7] =	(BUF[3]<<8)|BUF[2];
   						   //读取计算Y轴数据
	 
   BUF[4]=AT24CXX_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_L);
   BUF[5]=AT24CXX_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_H);
   MPU9250_Data[8] =	(BUF[5]<<8)|BUF[4];
 					       //读取计算Z轴数据
}

/******************************************************
***** 
***** 1 short -> 4 char
***** 
******************************************************/
void SplitByte(int *Original,u8 *Output)
{
	u8 i = 0;
	for(i=0;i<9;i++)
	{
		*Output = *Original & 0xff;
		Output ++;
		*Output = (*Original>>8) & 0xff;
		Output ++;
		*Output = (*Original>>16) & 0xff;
		Output ++;
		*Output = (*Original>>24) & 0xff;
		Output ++;		
		
		Original ++;
	}
}

/******************************************************
***** 
***** 1 short -> 4 char
***** 
******************************************************/
void SplitByte_fu(float Original,u8 *Output)
{
	Output = (u8*)&Original;
}

/******************************************************
***** 
***** 2 char -> 4 char
***** 
******************************************************/
void MakeByte(u8 *Original,u8 *Output)
{
	u8 i = 0;
	for(u8 j=0; j<3; j++) //3 int
	{
		for(i=0;i<4;i++)    //each int to 4 char
		{
			*Output = 0x00, Output++;	
			*Output = 0x00, Output++;	
			*Output = *Original, Original++, Output++;				
			*Output = *Original, Original++, Output++;
		}
	}
}


/******************************************************
***** 
***** 循环接收并处理数据
***** 
******************************************************/
void Robort_Get_MPU9250()
{	
	float Roll,Pitch,Yaw;
	for(u8 i=0;i<6;i++)
	{
		TEMP_Uart[i] = Rx_Process_Buffer2[6+i];
	}	
	Roll = ((float)(Rx_Process_Buffer2[6] + (Rx_Process_Buffer2[7] << 8)))/100.0f;
	Pitch = ((float)(Rx_Process_Buffer2[6] + (Rx_Process_Buffer2[7] << 8)))/100.0f;
	Yaw = ((float)(Rx_Process_Buffer2[6] + (Rx_Process_Buffer2[7] << 8)))/100.0f;
	
	SplitByte_fu(Roll,MPU9250_OutData_Uart);
	SplitByte_fu(Pitch,MPU9250_OutData_Uart+4);
	SplitByte_fu(Yaw,MPU9250_OutData_Uart+8);
//	MakeByte(TEMP_Uart,MPU9250_OutData_Uart);
}