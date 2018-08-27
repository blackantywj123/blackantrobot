 #include "App_Sensor.h"
 
 
 
static uSensorInfoTag gRobortSensorInfo;



/**********************************************************************************************************
**** �ļ�����Sensor_AD.c
**** 
**** �ļ�������ADģ�麯����
****		   ����AD�˿ڣ�ADC0 ----- P1.7    ADC1 ----- P2.0    ADC2 ----- P2.1	ADC3 ----- P2.2
****					   ADC4 ----- P2.3	  ADC5 ----- P2.4	 ADC6 ----- P2.5	ADC7 ----- P2.6
****		   			   ADC8 ----- P2.7	  ADC9 ----- P3.0	 
****		  ��ӦADͨ������ADC0������1...ADC9����10
****
**** �����ˣ�FeiChen
**** 
**** �汾�ţ�1.0
**** 
**** �޸ļ�¼��
**** 
**** 
****
***********************************************************************************************************/

//�����������ֵ
static char IRData[256] = {	255,255,255,255,255,255,255,255,255,255,//��һ�飺10��
							200,200,200,200,200,150,150,150,150,150,//�ڶ��飺10��
							100,100,100,100,100, 90, 90, 90, 90, 90,//�����飺10��
							 85, 85, 85, 80, 80, 80, 75, 75, 70, 70,//�����飺10��
							 65, 60, 57, 55, 54, 51, 50, 48, 47, 46,//�����飺10��
							 45, 43, 40, 39, 38, 37, 36, 36, 35, 35,//�����飺10��
							 34, 34, 33, 33, 32, 32, 31, 31, 30, 30,//�����飺10��
							 29, 29, 28, 28, 27, 27, 27, 26, 26, 26,//�ڰ��飺10��
							 25, 25, 25, 24, 24, 24, 24, 23, 23, 23,//�ھ��飺10��
							 23, 22, 22, 22, 22, 21, 21, 21, 21, 20,//��ʮ�飺10��
							 20, 20, 20, 20, 19, 19, 19, 19, 18, 18,//��11�飺10��
							 18, 18, 18, 17, 17, 17, 17, 17, 17, 16,//��12�飺10��
							 16, 16, 16, 16, 16, 16, 15, 15, 15, 15,//��13�飺10��
							 15, 15, 14, 14, 14, 14, 14, 14, 14, 14,//��14�飺10��
							 14, 13, 13, 13, 13, 13, 13, 13, 13, 13,//��15�飺10��
							 13, 12, 12, 12, 12, 12, 12, 12, 12, 12,//��16�飺10��
							 12, 12, 11, 11, 11, 11, 11, 11, 11, 11,//��17�飺10��
							 11, 11, 11, 11, 11, 10, 10, 10, 10, 10,//��18�飺10��
							 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,//��19�飺10��
							 10,  9,  9,  9,  9,  9,  9,  9,  9,  9,//��20�飺10��
							  9,  9,  9,  9,  9,  9,  9,  9,  8,  8,//��21�飺10��
							  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,//��22�飺10��
							  8,  8,  8,  8,  8,  8,  8,  7,  7,  7,//��23�飺10��
							  7,  7,  6,  6,  6,  6,  6,  6,  0,  0,//��24�飺10��
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//��25�飺10��
							  0,  0,  0,  0,  0,  0,//��26�飺6��								
							};




/*************************************************************
//! ͷ�ļ�
**************************************************************/
#include "App_Sensor.h"

#define AD_NUM  10		// �ܹ���ѡ10·AD�˿�
/*************************************************************************************************
*****
***** ��������unsigned char GetIR_Distance(unsigned char channle)
*****
***** ��ڲ�����channle��ѡ��ĵڼ�·AD�˿�ֵ��ȡֵΪ1-10
*****
*****
***** ������������ȡ���⴫�����в���룬�Ⱦ���ADת����ȡ��Ӧ�ĵ�ѹֵ��Ȼ�󽫵�ѹֵ�������뺯��ת��
*****			�ϰ���ľ���10����80cm				
*****		   
*****
***** ����ֵ�����صľ��Ǻ��������ֵ��ȡֵ��Χ0-255������10��80����Ϊ��Ч���������ֵ
*****
***** ���ߣ�FeiChen
**************************************************************************************************/


static unsigned char GetIR_Distance(unsigned char channle)
{
	unsigned int ucIRDisValue = 0;


	//ADͨ��������
	if(channle>15)
	{
		return 0;
	}
	//��ȡADת��ֵ
	ucIRDisValue = STM32_ADC_Read(channle);

	//��ADת��ֵ�л��ɾ���ֵ
	ucIRDisValue = ((int)SensorDistanceProcess(ucIRDisValue));

	//���þ���ֵ������255
	if(ucIRDisValue>255)
	{
		ucIRDisValue = 255;
	}
	return (char)ucIRDisValue;


		
}




/*************************************************************************************************
*****
***** ��������double SensorDistanceProcess(double x )
*****
***** ��ڲ�����double x������Ӧ�����Ѿ��������AD�˿ڲ���ֵ
*****
*****
***** ����������ADCֵ����ת��
*****			ͨ����ѯGP2D12���߱�������Ӧ�ľ����ADֵ�Ķ�Ӧ������ϵ������ת��
*****						
*****		   
*****
***** ����ֵ�����ؾ�������ת����ľ���ֵ
*****
***** ���ߣ�FeiChen
**************************************************************************************************/
/*************************************************************
//! ��������double SensorDistanceProcess(double x )
//! ����˵����ADCֵ����ת��
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
 	//ADͨ��������
	
	//��ȡADת��ֵ
	fTemp = STM32_ADC_Read(0);   
    
    //...
    
    pdata->sensor.temperature = fTemp;
    pdata->Data_Type = 4;
}





static void GetHumidity(uSensorInfoTag *pdata)
{
    float fHum = 0;
 	//ADͨ��������

	//��ȡADת��ֵ
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


