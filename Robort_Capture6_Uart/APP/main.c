#include "Common.h" 
#include "stm32f10x.h"                   
//#include "user_Config.h"                 
#include "misc.h"
#include "stm32f10x_exti.h" 
#include "stm32f10x_conf.h"
#include "App_Protocol.h"
#include "App_Message.h"
#include "APP_GPS.H"
#include "APP_Capture.h"
#include "APP_MPU9250.h"
/******************************/
#include "BSP_GPS.h"
#include <string.h>

extern void nmea_decode_test(void);

//static void Hardware_Test(void);

static void Motor_Test();

App_Robort_Dat_Tag g_ProtocolData = {0};
    
u8 u[17]="\r\nDMA 测试成功\r\n";
u8 w[17]="\r\nDMA 中断测试\r\n";
int main(void)
{
	u16 i=0;
	delay_init();					//延时函数
	TIMER_Init();					//时钟分配任务
//	Robort_Beep_Init();
//	Robort_Light_Init();
	Robort_Motor_Init();	//兼容大功率板 | 使得四个PWM输出为低
//	Robort_Steer_Init();	//对应舵机1~5
	Robort_Uart_Init();		//串口初始化
//	AT24CXX_Init();			//IIC初始化
//	Init_MPU9250();    //MPU9250初始化/////////////////////////////////////////
	Robort_GPS_Init();		//GPS初始化 
	GPS_DATA_Init(); 			//GPS相关
	Robot_Capture_Init();
//	printf("\r\n系统初始化完毕\r\n");
//	Steer_Test( );				//舵机初始化状态，对应舵机3~5
//	Motor_Test();
	while (1)
	{
		Robort_Get_MPU9250();
		Robort_Get_Nema();//循环接收GPS数据
		Robort_Get_Capture();
		if (Robort_Receive_Frame(&g_ProtocolData) == RET_OK)
		{
			Protocol_Parser(&g_ProtocolData);		
		}
		if(Task_20ms)
		{
			Task_20ms=0;	
			GPS_Message_Upload();//GPS数据更新？
			Capture_Clear_Zero();
		}
// ---------------------------------------------------------		
//		i++;
//		delay_ms(10);
//		if(i==20)
//		{
//			i=0;
//			printf("Short:\n");
//			printf("AX: %d	",MPU9250_Data[0]);
//			printf("AY: %d	",MPU9250_Data[1]);
//			printf("AZ: %d\r\n" ,MPU9250_Data[2]);
//			
//			printf("GX: %d	",MPU9250_Data[3]);
//			printf("GY: %d	",MPU9250_Data[4]);
//			printf("GZ: %d\r\n" ,MPU9250_Data[5]);
//			
//			printf("MX: %d	",MPU9250_Data[6]);
//			printf("MY: %d	",MPU9250_Data[7]);
//			printf("MZ: %d\r\n\n" ,MPU9250_Data[8]);
//			
//			//////////////////
//			printf("Int:\n");
//			printf("AX: %d	",TEMP[0]);
//			printf("AY: %d	",TEMP[1]);
//			printf("AZ: %d\r\n" ,TEMP[2]);
//			
//			printf("GX: %d	",TEMP[3]);
//			printf("GY: %d	",TEMP[4]);
//			printf("GZ: %d\r\n" ,TEMP[5]);
//			
//			printf("MX: %d	",TEMP[6]);
//			printf("MY: %d	",TEMP[7]);
//			printf("MZ: %d\r\n\n" ,TEMP[8]);
//	
//		}	
		

	}  //------------------------------------while

 
}



static void Motor_Test()
{

	Robort_Direction_Control(1, 2, 2, 30, 30);
	delay_ms(1000);
	Robort_Direction_Control(1, 0, 0, 30, 30);//FORWARD
	delay_ms(1000);
	Robort_Direction_Control(1, 1, 1, 30, 30);//BACKWARD
	delay_ms(1000);
	Robort_Direction_Control(1, 1, 0, 30, 30);//LEFT
	delay_ms(1000);
	Robort_Direction_Control(1, 0, 1, 30, 30);//RIGHT
	delay_ms(1000);
	Robort_Direction_Control(1, 0, 2, 30, 30);//LEFT_ONLY
	delay_ms(1000);
	Robort_Direction_Control(1, 2, 0, 30, 30);//RIGHT_ONLY
	delay_ms(1000);
	Robort_Direction_Control(1, 0, 0, 30, 30);//FORWARD
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	Robort_Direction_Control(1, 0, 0, 10, 10);//slow
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	Robort_Direction_Control(1, 0, 0, 64, 64);//fast
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	Robort_Direction_Control(50, 2, 2, 30, 30);
}

//static void Hardware_Test()
//{

//     App_Robort_Dat_Tag ProtocolData = {0};
//     uSensorInfoTag SensorEntry = {0};
///******************************************************************************************
//						BEEP TEST
//******************************************************************************************/   
//    // Robort_Beep_Control(ROBORT_BEEP_ON);
//     delay_ms(1000);
//     Robort_Beep_Control(ROBORT_BEEP_OFF);
//     delay_ms(1000);
//     
///******************************************************************************************
//						LIGHT TEST
//******************************************************************************************/     
//     Robort_Light_Control( ROBORT_LIGHT_ONE, ROBORT_LIGHT_ON);
//     delay_ms(1000);
//     Robort_Light_Control( ROBORT_LIGHT_ONE, ROBORT_LIGHT_OFF);
//     delay_ms(1000);
//     
///******************************************************************************************
//						MOTOR TEST
//******************************************************************************************/
//     //Robort_Direction_Control(ROBORT_STOP, 0);
//     delay_ms(10000);

//     Robort_Direction_Control(ROBORT_BACK, 50);
//     delay_ms(10000);
//     
//     Robort_Direction_Control(ROBORT_FORWARD, 50);
//     delay_ms(10000);
//     

//     
//     Robort_Direction_Control(ROBORT_LEFT, 50);
//     delay_ms(10000);
//     
//     Robort_Direction_Control(ROBORT_RIGHT, 50);
//     delay_ms(1000);
//     
///******************************************************************************************
//                       STEER TEST
//******************************************************************************************/
//     Robort_Steer_Control(ROBORT_STEER1,  0); //Stay 0 angle
//     Robort_Steer_Control(ROBORT_STEER2,  0); //Stay 0 angle
//     delay_ms(2000);
//    
//     Robort_Steer_Control(ROBORT_STEER1,  90); //Stay 90 angle
//     Robort_Steer_Control(ROBORT_STEER2,  90); //Stay 90 angle
//     delay_ms(2000);
//     
//     Robort_Steer_Control(ROBORT_STEER1,  180); //Stay 90 angle
//     Robort_Steer_Control(ROBORT_STEER2,  180); //Stay 90 angle
//     delay_ms(2000);
//    
///******************************************************************************************
//                       ADC TEST
//******************************************************************************************/
// 
//     Robort_Get_Sensor_Data(HUMIDITY_TYPE, &SensorEntry);
//     Robort_Get_Sensor_Data(TEMPERATURE_TYPE, &SensorEntry);
//     Robort_Get_Sensor_Data(ENERGY_TYPE, &SensorEntry);
//     Robort_Get_Sensor_Data(DISTANCE_TYPE, &SensorEntry);
//     
///******************************************************************************************
//                       UART TEST
//******************************************************************************************/     

//     Robort_Receive_Frame(&ProtocolData);
//     Robort_Send_Frame((void *)&ProtocolData);
//     
//    
//}


/******************* (C) COPYRIGHT 2015 X-SPACE *****END OF FILE****/



/*********************************************END OF FILE**********************/
