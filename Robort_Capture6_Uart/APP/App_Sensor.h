#ifndef _APP_SENSOR_H
#define _APP_SENSOR_H

#include "BSP_ADC.H"

#define uint8 unsigned char
#define uint16 unsigned short

#define DISTANCE_NUM  8

typedef enum
{
    SENSOR_DISTANCE1 = 0x01, 
    SNESOR_DISTANCE2 = 0x02, 
    SNESOR_DISTANCE3 = 0x04, 
    SNESOR_DISTANCE4 = 0x08, 
    SNESOR_DISTANCE5 = 0x10, 
    SNESOR_DISTANCE6 = 0x20, 
    SNESOR_DISTANCE7 = 0x40,  
    SNESOR_DISTANCE8 = 0x80   
}SENSOR_DISTANCE_TAG;

typedef enum
{
    DISTANCE_TYPE=0,
    HUMIDITY_TYPE,
    TEMPERATURE_TYPE,
    ENERGY_TYPE,
    POSITION_TYPE,
}SENSOR_TYPE_TAG;





typedef struct
{
    uint8 Data_Type;
    uint16 Data_Length;
    union
    {
        float temperature;
        float humidity;
        unsigned char distance[DISTANCE_NUM];
        float energy;
        //reserve
    }sensor;
}uSensorInfoTag;


static double SensorDistanceProcess(double x );
static unsigned int GetIRDistance(unsigned char channle);
static unsigned int GetIRDistance(unsigned char channle);
static unsigned char GetIR_Distance(unsigned char channle);

static void GetHumidity(uSensorInfoTag *pdata);
static void StartToAcquireSensorInfo(void);
static void GetTemperature(uSensorInfoTag *pdata);

static void Get_Energy_Quantity(uSensorInfoTag *pdata);
static void Get_Distance(uSensorInfoTag *pdata);

void Robort_Sensor_Init(void);
void Robort_Get_Sensor_Data(SENSOR_TYPE_TAG type, uSensorInfoTag *pdata);

#endif

