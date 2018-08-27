 #include "App_Sensor.h"
 
 
 
static uSensorInfoTag gRobortSensorInfo;



/**********************************************************************************************************
**** 文件名：Sensor_AD.c
**** 
**** 文件描述：AD模块函数库
****		   可用AD端口：ADC0 ----- P1.7    ADC1 ----- P2.0    ADC2 ----- P2.1	ADC3 ----- P2.2
****					   ADC4 ----- P2.3	  ADC5 ----- P2.4	 ADC6 ----- P2.5	ADC7 ----- P2.6
****		   			   ADC8 ----- P2.7	  ADC9 ----- P3.0	 
****		  对应AD通道号是ADC0———1...ADC9——10
****
**** 创建人：FeiChen
**** 
**** 版本号：1.0
**** 
**** 修改记录：
**** 
**** 
****
***********************************************************************************************************/

//定义红外测距查表值
static char IRData[256] = {	255,255,255,255,255,255,255,255,255,255,//第一组：10个
							200,200,200,200,200,150,150,150,150,150,//第二组：10个
							100,100,100,100,100, 90, 90, 90, 90, 90,//第三组：10个
							 85, 85, 85, 80, 80, 80, 75, 75, 70, 70,//第四组：10个
							 65, 60, 57, 55, 54, 51, 50, 48, 47, 46,//第五组：10个
							 45, 43, 40, 39, 38, 37, 36, 36, 35, 35,//第六组：10个
							 34, 34, 33, 33, 32, 32, 31, 31, 30, 30,//第七组：10个
							 29, 29, 28, 28, 27, 27, 27, 26, 26, 26,//第八组：10个
							 25, 25, 25, 24, 24, 24, 24, 23, 23, 23,//第九组：10个
							 23, 22, 22, 22, 22, 21, 21, 21, 21, 20,//第十组：10个
							 20, 20, 20, 20, 19, 19, 19, 19, 18, 18,//第11组：10个
							 18, 18, 18, 17, 17, 17, 17, 17, 17, 16,//第12组：10个
							 16, 16, 16, 16, 16, 16, 15, 15, 15, 15,//第13组：10个
							 15, 15, 14, 14, 14, 14, 14, 14, 14, 14,//第14组：10个
							 14, 13, 13, 13, 13, 13, 13, 13, 13, 13,//第15组：10个
							 13, 12, 12, 12, 12, 12, 12, 12, 12, 12,//第16组：10个
							 12, 12, 11, 11, 11, 11, 11, 11, 11, 11,//第17组：10个
							 11, 11, 11, 11, 11, 10, 10, 10, 10, 10,//第18组：10个
							 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,//第19组：10个
							 10,  9,  9,  9,  9,  9,  9,  9,  9,  9,//第20组：10个
							  9,  9,  9,  9,  9,  9,  9,  9,  8,  8,//第21组：10个
							  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,//第22组：10个
							  8,  8,  8,  8,  8,  8,  8,  7,  7,  7,//第23组：10个
							  7,  7,  6,  6,  6,  6,  6,  6,  0,  0,//第24组：10个
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//第25组：10个
							  0,  0,  0,  0,  0,  0,//第26组：6个								
							};




/*************************************************************
//! 头文件
**************************************************************/
#include "App_Sensor.h"

#define AD_NUM  10		// 总共可选10路AD端口
/*************************************************************************************************
*****
***** 函数名：unsigned char GetIR_Distance(unsigned char channle)
*****
***** 入口参数：channle：选择的第几路AD端口值，取值为1-10
*****
*****
***** 功能描述：获取红外传感器感测距离，先经过AD转换获取相应的电压值，然后将电压值经过距离函数转成
*****			障碍物的距离10——80cm				
*****		   
*****
***** 返回值：返回的就是红外测距距离值，取值范围0-255，但是10—80区间为有效红外测距距离值
*****
***** 作者：FeiChen
**************************************************************************************************/


static unsigned char GetIR_Distance(unsigned char channle)
{
	unsigned int ucIRDisValue = 0;


	//AD通道错误处理
	if(channle>15)
	{
		return 0;
	}
	//获取AD转换值
	ucIRDisValue = STM32_ADC_Read(channle);

	//将AD转换值切换成距离值
	ucIRDisValue = ((int)SensorDistanceProcess(ucIRDisValue));

	//设置距离值不超过255
	if(ucIRDisValue>255)
	{
		ucIRDisValue = 255;
	}
	return (char)ucIRDisValue;


		
}




/*************************************************************************************************
*****
***** 函数名：double SensorDistanceProcess(double x )
*****
***** 入口参数：double x——对应的是已经处理过的AD端口采样值
*****
*****
***** 功能描述：ADC值距离转换
*****			通过查询GP2D12曲线表，制作相应的距离和AD值的对应函数关系，进行转换
*****						
*****		   
*****
***** 返回值：返回经过距离转换后的距离值
*****
***** 作者：FeiChen
**************************************************************************************************/
/*************************************************************
//! 函数名：double SensorDistanceProcess(double x )
//! 函数说明：ADC值距离转换
*************************************************************/
static double SensorDistanceProcess(double x )
{

	int IRIndex = 0;
	IRIndex = x/4;
	return IRData[IRIndex];
}    


static void GetTemperature(uSensorInfoTag *pdata)
{
    float fTemp = 0;
 	//AD通道错误处理
	
	//获取AD转换值
	fTemp = STM32_ADC_Read(0);   
    
    //...
    
    pdata->sensor.temperature = fTemp;
    pdata->Data_Type = 4;
}





static void GetHumidity(uSensorInfoTag *pdata)
{
    float fHum = 0;
 	//AD通道错误处理

	//获取AD转换值
	//fHum = STM32_ADC_Read(0);   
    
    //...
    
    pdata->sensor.humidity = fHum;
    pdata->Data_Type = 4;
}

static void StartToAcquireSensorInfo(void)
{
//     SENSOR_TYPE_TAG flg_sensor = 0;
//     unsigned char dVal = 0;
//     uint8 i = 0;
//     
//    // flg_sensor = Get_Sensor_Type();
//     
//     if (flg_sensor & DISTANCE_TYPE)   //energy time flag 
//     {
//         for (i=0; i<DISTANCE_NUM; i++)
//         {
//             dVal = GetIR_Distance(i);   // get distance value from HW

//         }

//     }
//     if (flg_sensor & HUMIDITY_TYPE)
//     {
//         
//     }
//     if (flg_sensor & TEMPERATURE_TYPE)
//     {

//     }
//     if (flg_sensor & ENERGY_TYPE)
//     {

//     }
}




static void Get_Distance(uSensorInfoTag *pdata)
{
    pdata->sensor.distance[0] = GetIR_Distance(ADC_CHANNEL10 );
    pdata->sensor.distance[1] = GetIR_Distance(ADC_CHANNEL11 );
    pdata->sensor.distance[2] = GetIR_Distance(ADC_CHANNEL12 );
    pdata->sensor.distance[3] = GetIR_Distance(ADC_CHANNEL13 );
    pdata->Data_Type = 4;
   return ;
}



static void Get_Energy_Quantity(uSensorInfoTag *pdata)
{
    unsigned short uEgyQuan = 0;
    float  fEnergyQuantity = 0;
    
   	uEgyQuan = STM32_ADC_Read(ADC_CHANNEL9);  
    
    //... covert to float value
    
    pdata->sensor.energy = uEgyQuan;
    pdata->Data_Type = 4;
}


void Robort_Get_Sensor_Data(SENSOR_TYPE_TAG type, uSensorInfoTag *pdata)
{
    switch (type)
    {
        case DISTANCE_TYPE:
            Get_Distance(pdata);
        break;
        case TEMPERATURE_TYPE:
            GetHumidity(pdata);
        break;
        case HUMIDITY_TYPE:
            GetHumidity(pdata);
        break;
        case ENERGY_TYPE:
            Get_Energy_Quantity(pdata);
        break;
        case POSITION_TYPE:
        
        break;
        
        default: break;
    }
}




void Robort_Sensor_Init(void)
{
    ADC1_Init();  //
}






//STM32_ADC_Read(u8 channel, other parameters)


