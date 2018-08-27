#include "APP_MPU9250.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//24CXX���� ����(�ʺ�24C01~24C16,24C32~256δ��������!�д���֤!)		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define AT24C02_ADDR 0xA0
unsigned char BUF[10] = {10,10,10,10,0,0,0,0,0,0};       //�������ݻ�����
short T_X,T_Y,T_Z,T_T;		 //X,Y,Z�ᣬ�¶�
short MPU9250_Data[9]; // ACCEL_xyz    GYRO_xyz    MAG_xyz
int TEMP[9];
unsigned char MPU9250_OutData[36];
float TEMP_Uart[3];
unsigned char MPU9250_OutData_Uart[12];

/******************************************************
***** 
***** ��ʼ��IIC�ӿ�
***** 
******************************************************/
void AT24CXX_Init(void)
{
	IIC_Init();
}

/******************************************************
***** ��AT24CXXָ����ַ����һ������
***** ReadAddr:��ʼ�����ĵ�ַ  
***** ����ֵ  :����������
******************************************************/
u8 AT24CXX_ReadOneByte(u8 SlaveAddr, u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(SlaveAddr);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
		IIC_Wait_Ack();		 
	}else IIC_Send_Byte(SlaveAddr+((ReadAddr/256)<<1));   //����������ַ,д���� 	 

	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(SlaveAddr+1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}

/******************************************************
***** ��AT24CXXָ����ַд��һ������
***** WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ  
***** DataToWrite:Ҫд�������
******************************************************/
void AT24CXX_WriteOneByte(u8 SlaveAddr,u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(SlaveAddr);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
 	}else
	{
		IIC_Send_Byte(SlaveAddr+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 
	}	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 //delay_ms(10);	
}

/******************************************************
***** ��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
***** �ú�������д��16bit����32bit������
***** WriteAddr  :��ʼд��ĵ�ַ  
***** DataToWrite:���������׵�ַ
***** Len        :Ҫд�����ݵĳ���2,4
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
***** ��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
***** �ú������ڶ���16bit����32bit������.
***** ReadAddr   :��ʼ�����ĵ�ַ 
***** ����ֵ     :����
***** Len        :Ҫ�������ݵĳ���2,4
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
***** ���AT24CXX�Ƿ�����
***** ��������24XX�����һ����ַ(255)���洢��־��.
***** ���������24Cϵ��,�����ַҪ�޸�
***** ����1:���ʧ��
***** ����0:���ɹ�
***** 
*****
******************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(AT24C02_ADDR, 255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(AT24C02_ADDR, 255,0X55);
	    temp=AT24CXX_ReadOneByte(AT24C02_ADDR, 255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

/******************************************************
***** ��AT24CXX�����ָ����ַ��ʼ����ָ������������
***** ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
***** pBuffer  :���������׵�ַ
***** NumToRead:Ҫ�������ݵĸ���
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
***** ��AT24CXX�����ָ����ַ��ʼд��ָ������������
***** WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
***** pBuffer   :���������׵�ַ
***** NumToWrite:Ҫд�����ݵĸ���
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
***** ���ܣ���ʼ��MPU9250
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
   Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //��2000��
   Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
   Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
*/
  AT24CXX_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x00);	//�������״̬
	AT24CXX_WriteOneByte(GYRO_ADDRESS,SMPLRT_DIV, 0x07);  //�����ǲ�����
	AT24CXX_WriteOneByte(GYRO_ADDRESS,CONFIG, 0x06);      //��ͨ�˲���
	AT24CXX_WriteOneByte(GYRO_ADDRESS,GYRO_CONFIG, 0x18); //
	AT24CXX_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG, 0x01);//
  //----------------
//	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode	

}


/******************************************************
***** ��ȡMPU9250����
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
   MPU9250_Data[0]/=164; 						   //��ȡ����X������

   BUF[2]=AT24CXX_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_L);
   BUF[3]=AT24CXX_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_H);
   MPU9250_Data[1]=	(BUF[3]<<8)|BUF[2];
   MPU9250_Data[1]/=164; 						   //��ȡ����Y������
   BUF[4]=AT24CXX_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_L);
   BUF[5]=AT24CXX_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_H);
   MPU9250_Data[2]=	(BUF[5]<<8)|BUF[4];
   MPU9250_Data[2]/=164; 					      //��ȡ����Z������
 
}

void READ_MPU9250_GYRO(void)
{ 

   BUF[0]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_L); 
   BUF[1]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_H);
   MPU9250_Data[3]=	(BUF[1]<<8)|BUF[0];
   MPU9250_Data[3]/=16.4; 						   //��ȡ����X������

   BUF[2]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_L);
   BUF[3]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_H);
   MPU9250_Data[4]=	(BUF[3]<<8)|BUF[2];
   MPU9250_Data[4]/=16.4; 						   //��ȡ����Y������
   BUF[4]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_L);
   BUF[5]=AT24CXX_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_H);
   MPU9250_Data[5]=	(BUF[5]<<8)|BUF[4];
   MPU9250_Data[5]/=16.4; 					       //��ȡ����Z������
 
 
  // BUF[6]=Single_Read(GYRO_ADDRESS,TEMP_OUT_L); 
  // BUF[7]=Single_Read(GYRO_ADDRESS,TEMP_OUT_H); 
  // T_T=(BUF[7]<<8)|BUF[6];
  // T_T = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
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
   						   //��ȡ����Y������
	 
   BUF[4]=AT24CXX_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_L);
   BUF[5]=AT24CXX_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_H);
   MPU9250_Data[8] =	(BUF[5]<<8)|BUF[4];
 					       //��ȡ����Z������
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
***** ѭ�����ղ���������
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