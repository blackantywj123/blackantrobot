//#include "Common.h" 

#include "BSP_Gpio.h"

void STM32_GPIO_SetPin( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, u8 val)
 {
	 if( 1==val)
		 GPIO_SetBits(GPIOx,GPIO_Pin);						 
	 else if( 0== val)
		 GPIO_ResetBits( GPIOx, GPIO_Pin);
 }
 
