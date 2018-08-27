#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "stm32f10x.h"

void STM32_GPIO_SetPin( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, u8 val);


#endif
