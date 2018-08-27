#include "App_Message.h"
#include "APP_GPS.H"
#include "APP_Capture.h"
#include "APP_MPU9250.h"

u8 Message_uPload_Buff[100]={0};
/***********************************************************************************
*函数名：Robort_Receive_Frame
*函数功能： 判断程序是否有错   与GPS有关
*参数： 无
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
*********************************************************************/
int Robort_Receive_Frame(App_Robort_Dat_Tag *pdata) //Frame框架
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
*函数名：Crc16
*函数功能： 循环冗余检测
*参数： *ptr，count（指针，串口计数）
*返回值：crc&oxffff
*时间：2018.8.13
*备注：wenjie.Y
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
*函数名：Robort_Send_Frame
*函数功能： 数据发送检测，监测数据是否有误
*参数： *ptr，count（指针，串口计数）
*返回值：crc&oxffff
*时间：2018.8.13
*备注：wenjie.Y
*********************************************************************/
void Robort_Send_Frame(App_Robort_Dat_Tag *pdata)
{
     STM32_UART_Write(STM32_UART1, pdata, sizeof(App_Robort_Dat_Tag));
}

/*************************************************************************************
*函数名：GPS_Message_Upload
*函数功能:上传GPS数据给路由器
*时间：2016-10-15
*************************************************************************************/
void GPS_Message_Upload( void)
{
  int Crc;
//	while( DMA1_Uart1_Status_Get())			                //查询DMA_UART是否空闲
//	{
//		;
//	}
	
	Message_uPload_Buff[0]=0xff;                        //组装数据包帧头
	Message_uPload_Buff[1]=0x02;                        //地址位
	Message_uPload_Buff[2]=64;                          //低字节 有64个数据(0x40)
	Message_uPload_Buff[3]=0;														//高字节
	Message_uPload_Buff[4]=0x10;                        //状态位 (not use)
	
	for( u8 i=0;i<4;i++)                                 //装入右轮信息5 6 7 8 
	{
//		Message_uPload_Buff[i+5]=1;
		Message_uPload_Buff[i+5]=Capture_Data.right.uright[i];
	}

  for( u8 i=0;i<4;i++)                                 //装入左轮信息9 10 11 12 
	{
		Message_uPload_Buff[i+9]=Capture_Data.left.uleft[i];
//		Message_uPload_Buff[i+9]=2;
	}
	
	for( u8 i=0;i<4;i++)                                 //装入纬度信息13 14 15 16
	{
		Message_uPload_Buff[i+13]=GPS_Info_Data.lon.uLon[i];
	}	
	
	for( u8 i=0;i<4;i++)                                 //装入经度信息17 18 19 20
	{
		Message_uPload_Buff[i+17]=GPS_Info_Data.lat.uLat[i];
	}	

  for( u8 i=0;i<4;i++)                                 //装入power信息21 22 23 24
	{
		Message_uPload_Buff[i+21]=3;
	}	
			

   for( u8 i=0;i<4;i++)                                //装入温度信息25 26 27 28
	{
		Message_uPload_Buff[i+25]=4;
	}	
	
	 for( u8 i=0;i<4;i++)                                //装入湿度信息29 30 31 32
	{
		Message_uPload_Buff[i+29]=MPU9250_OutData_Uart[8+i];
	}
	
	 for( u8 i=0;i<36;i++)                               //装入MPU9250 33 ...(36)...68 -> num = 68-4 = 64
	{
		Message_uPload_Buff[i+33]=MPU9250_OutData[i];
	}

	Message_uPload_Buff[69]=Crc&0xff;                     //确定一下之前是否装入46个数据
	Message_uPload_Buff[70]=(Crc>>8)&0xff;
//	STM32_UART_Write(1,Message_uPload_Buff,25);
	STM32_UART1_Write(Message_uPload_Buff,71);            //确定是否一共48个数据
}

//void GPS_Message_Upload( void)
//{
//  int Crc;
////	while( DMA1_Uart1_Status_Get())			                //查询DMA_UART是否空闲
////	{
////		;
////	}
//	
//	Message_uPload_Buff[0]=0xff;                        //组装数据包帧头
//	Message_uPload_Buff[1]=0x02;												//地址位
//	Message_uPload_Buff[2]=18;													//低字节
//	Message_uPload_Buff[3]=0;														//高字节
//	Message_uPload_Buff[4]=0x10;												//状态位
//				
//	for( u8 i=0;i<4;i++)                                 //装入经度信息  5 6 7 8
//	{
//		Message_uPload_Buff[i+5]=GPS_Info_Data.lat.uLat[i];
//	}
//				
//	for( u8 i=0;i<4;i++)                                 //装入纬度信息  9 10 11 12
//	{
//		Message_uPload_Buff[i+9]=GPS_Info_Data.lon.uLon[i];
//	}
//				
//	for( u8 i=0;i<4;i++)                                  //装入航向信息 13 14 15 16
//	{
//		Message_uPload_Buff[i+13]=GPS_Info_Data.direction.uDir[i];
//	}				
//				
//	for( u8 i=0;i<4;i++)                                  //装入速度信息 17 18 19 20
//	{
//		Message_uPload_Buff[i+17]=GPS_Info_Data.speed.uSpeed[i];
//	}	 
//	Message_uPload_Buff[21]=GPS_Info_Data.sAtinfo_iNuse;   //在用卫星数目 21
//	Message_uPload_Buff[22]=GPS_Info_Data.sAtinfo_iNview;  //可见卫星数目 22
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
*函数名：Robort_Uart_Init
*函数功能： 串口初始化
*参数： 无
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
*********************************************************************/
void Robort_Uart_Init(void)
{
    UART_Init();
}
