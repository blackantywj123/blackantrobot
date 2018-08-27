#ifndef _APP_BEEP_H
#define _APP_BEEP_H

#include "BSP_Gpio.h"


#define uint8 unsigned char
#define uint16 unsigned short

typedef enum
{
	ROBORT_BEEP_ON = 0,
	ROBORT_BEEP_OFF
}ROBORT_BEEP_TAG;


void Robort_Beep_Control(ROBORT_BEEP_TAG beep_status);
void Robort_Beep_Init(void);

extern void STM32_GPIO_SetPin( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, u8 val);


#endif
