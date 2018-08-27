#include "App_Message.h"
#include "APP_GPS.H"
#include "APP_Capture.h"
#include "APP_MPU9250.h"

u8 Message_uPload_Buff[100]={0};
/***********************************************************************************
*��������Robort_Receive_Frame
*�������ܣ� �жϳ����Ƿ��д�   ��GPS�й�
*������ ��
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
int Robort_Receive_Frame(App_Robort_Dat_Tag *pdata) //Frame���
{
    uint8 ret = 0;
    
    ret = STM32_UART_Read(STM32_UART1, pdata, sizeof(App_Robort_Dat_Tag));
    if (ret==0)
    {
        return RET_ERROR;
    }
    
    return RET_OK;    
}
/***********************************************************************************
*��������Crc16
*�������ܣ� ѭ��������
*������ *ptr��count��ָ�룬���ڼ�����
*����ֵ��crc&oxffff
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
static int Crc16(u8 *ptr, int count)
{
	int crc, i;
	crc = 0;
	while (--count >= 0){
		crc = (crc ^ (((int)*ptr++) << 8));
		for (i = 0; i < 8; ++i)
		if (crc & 0x8000)
		crc = ((crc << 1) ^ 0x1021);
		else
		crc = crc << 1;
	}
	return (crc & 0xFFFF);
}
/***********************************************************************************
*��������Robort_Send_Frame
*�������ܣ� ���ݷ��ͼ�⣬��������Ƿ�����
*������ *ptr��count��ָ�룬���ڼ�����
*����ֵ��crc&oxffff
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
void Robort_Send_Frame(App_Robort_Dat_Tag *pdata)
{
     STM32_UART_Write(STM32_UART1, pdata, sizeof(App_Robort_Dat_Tag));
}

/*************************************************************************************
*��������GPS_Message_Upload
*��������:�ϴ�GPS���ݸ�·����
*ʱ�䣺2016-10-15
*************************************************************************************/
void GPS_Message_Upload( void)
{
  int Crc;
//	while( DMA1_Uart1_Status_Get())			                //��ѯDMA_UART�Ƿ����
//	{
//		;
//	}
	
	Message_uPload_Buff[0]=0xff;                        //��װ���ݰ�֡ͷ
	Message_uPload_Buff[1]=0x02;                        //��ַλ
	Message_uPload_Buff[2]=64;                          //���ֽ� ��64������(0x40)
	Message_uPload_Buff[3]=0;														//���ֽ�
	Message_uPload_Buff[4]=0x10;                        //״̬λ (not use)
	
	for( u8 i=0;i<4;i++)                                 //װ��������Ϣ5 6 7 8 
	{
//		Message_uPload_Buff[i+5]=1;
		Message_uPload_Buff[i+5]=Capture_Data.right.uright[i];
	}

  for( u8 i=0;i<4;i++)                                 //װ��������Ϣ9 10 11 12 
	{
		Message_uPload_Buff[i+9]=Capture_Data.left.uleft[i];
//		Message_uPload_Buff[i+9]=2;
	}
	
	for( u8 i=0;i<4;i++)                                 //װ��γ����Ϣ13 14 15 16
	{
		Message_uPload_Buff[i+13]=GPS_Info_Data.lon.uLon[i];
	}	
	
	for( u8 i=0;i<4;i++)                                 //װ�뾭����Ϣ17 18 19 20
	{
		Message_uPload_Buff[i+17]=GPS_Info_Data.lat.uLat[i];
	}	

  for( u8 i=0;i<4;i++)                                 //װ��power��Ϣ21 22 23 24
	{
		Message_uPload_Buff[i+21]=3;
	}	
			

   for( u8 i=0;i<4;i++)                                //װ���¶���Ϣ25 26 27 28
	{
		Message_uPload_Buff[i+25]=4;
	}	
	
	 for( u8 i=0;i<4;i++)                                //װ��ʪ����Ϣ29 30 31 32
	{
		Message_uPload_Buff[i+29]=MPU9250_OutData_Uart[8+i];
	}
	
	 for( u8 i=0;i<36;i++)                               //װ��MPU9250 33 ...(36)...68 -> num = 68-4 = 64
	{
		Message_uPload_Buff[i+33]=MPU9250_OutData[i];
	}

	Message_uPload_Buff[69]=Crc&0xff;                     //ȷ��һ��֮ǰ�Ƿ�װ��46������
	Message_uPload_Buff[70]=(Crc>>8)&0xff;
//	STM32_UART_Write(1,Message_uPload_Buff,25);
	STM32_UART1_Write(Message_uPload_Buff,71);            //ȷ���Ƿ�һ��48������
}

//void GPS_Message_Upload( void)
//{
//  int Crc;
////	while( DMA1_Uart1_Status_Get())			                //��ѯDMA_UART�Ƿ����
////	{
////		;
////	}
//	
//	Message_uPload_Buff[0]=0xff;                        //��װ���ݰ�֡ͷ
//	Message_uPload_Buff[1]=0x02;												//��ַλ
//	Message_uPload_Buff[2]=18;													//���ֽ�
//	Message_uPload_Buff[3]=0;														//���ֽ�
//	Message_uPload_Buff[4]=0x10;												//״̬λ
//				
//	for( u8 i=0;i<4;i++)                                 //װ�뾭����Ϣ  5 6 7 8
//	{
//		Message_uPload_Buff[i+5]=GPS_Info_Data.lat.uLat[i];
//	}
//				
//	for( u8 i=0;i<4;i++)                                 //װ��γ����Ϣ  9 10 11 12
//	{
//		Message_uPload_Buff[i+9]=GPS_Info_Data.lon.uLon[i];
//	}
//				
//	for( u8 i=0;i<4;i++)                                  //װ�뺽����Ϣ 13 14 15 16
//	{
//		Message_uPload_Buff[i+13]=GPS_Info_Data.direction.uDir[i];
//	}				
//				
//	for( u8 i=0;i<4;i++)                                  //װ���ٶ���Ϣ 17 18 19 20
//	{
//		Message_uPload_Buff[i+17]=GPS_Info_Data.speed.uSpeed[i];
//	}	 
//	Message_uPload_Buff[21]=GPS_Info_Data.sAtinfo_iNuse;   //����������Ŀ 21
//	Message_uPload_Buff[22]=GPS_Info_Data.sAtinfo_iNview;  //�ɼ�������Ŀ 22
//	//////////////////////////////////////////////////////////////////////////////
//	//															HERE
//	//////////////////////////////////////////////////////////////////////////////
//	Crc=Crc16(Message_uPload_Buff,22);
//	
//	Message_uPload_Buff[23]=Crc&0xff;                      //             23
//	Message_uPload_Buff[24]=(Crc>>8)&0xff;                 //             24
////	STM32_UART_Write(1,Message_uPload_Buff,25);
//	STM32_UART1_Write(Message_uPload_Buff,25);
//}

/***********************************************************************************
*��������Robort_Uart_Init
*�������ܣ� ���ڳ�ʼ��
*������ ��
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
void Robort_Uart_Init(void)
{
    UART_Init();
}
