#ifndef _APP_LIGHT_H
#define _APP_LIGHT_H
#include "BSP_Gpio.h"

typedef enum
{
	ROBORT_LIGHT_ON = 0,
	ROBORT_LIGHT_OFF,
}ROBORT_LIGHT_TAG;

typedef enum
{
	ROBORT_LIGHT_ONE = 0,
	ROBORT_LIGHT_TWO
}ROBORT_LIGHT_NO_TAG;

void Robort_Light_Control(ROBORT_LIGHT_NO_TAG light_no, ROBORT_LIGHT_TAG light_status);
static void Control_1stLight(ROBORT_LIGHT_TAG light_status);
static void Control_2ndLight(ROBORT_LIGHT_TAG light_status);
void Robort_Light_Init(void);

#endif 
