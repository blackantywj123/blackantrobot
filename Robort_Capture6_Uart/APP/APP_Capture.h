#ifndef __APP_CAPTURE_H
#define __APP_CAPTURE_H
#include "stm32f10x.h"


typedef enum
{
	Direction_0=0,
	Direction_1=1
}MOTOR_DIR;


struct Capture_Info{
	// 右轮信息
	union{
		int fleft;
		u8   uleft[4];
	}left;
	// 左轮信息
	union {
		int fright;
		u8    uright[4];   
	}right;

};

extern struct Capture_Info Capture_Data;

void Robot_Capture_Init(void);
void Robort_Get_Capture(void);
void IN_Capture_r(void);
void IN_Capture_l(void);

void Capture_Clear_Zero(void);
#endif

