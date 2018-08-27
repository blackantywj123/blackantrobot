#ifndef _APP_MOTOR_H
#define _APP_MOTOR_H

#include "BSP_Gpio.h"
#include "BSP_Pwm.h"


#define MOTOR_PERIOD_DEFAULT 20000 //20ms period

#define uint8 unsigned char
#define uint16 unsigned short 


typedef enum
{
	ROBORT_SPEED_DEFAULT = 0,
	ROBORT_SPEED_VARIBLE  
}ROBORT_SPEED_VAL_TAG;  

 typedef enum
{
	ROBORT_SPEED_L = 0,
	ROBORT_SPEED_R
}ROBORT_SPEED_TAG;    

typedef enum
{
	ROBORT_STOP = 0,
	ROBORT_FORWARD,
	ROBORT_BACK,
	ROBORT_LEFT,
	ROBORT_RIGHT
}ROBORT_DIRECTION_TAG;

typedef enum
{
    MOTOR_LT = 0, //Left-Top
    MOTOR_RT,
    MOTOR_LB,     //Left-Bottom
    MOTOR_RB,
    MOTOR_NONE
}MOTOR_TAG;

typedef enum
{
    DIR_POSITIVE = 0,
    DIR_NEGTIVE,
    DIR_STOPPED
}DIRECTION_TAG;

void Robort_Motor_Init(void);
void Robort_Direction_Control(uint8 EN,uint8 ctl_l, uint8 ctl_r,uint8 speed_l,uint8 speed_r);
void Robort_Speed_Control(ROBORT_SPEED_TAG speed_edge, uint8 speed_val);

static void Set_Motor_Direction(MOTOR_TAG motorNo, DIRECTION_TAG  dir);
static void Set_Motor_Speed(MOTOR_TAG motorNo, uint8 speed);

static uint16  CalPWMDutyCycle(unsigned char speed);

#endif