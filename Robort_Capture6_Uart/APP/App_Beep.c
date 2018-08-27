#include "App_Beep.h"



void Robort_Beep_Control(ROBORT_BEEP_TAG beep_status)
{
    if (ROBORT_BEEP_ON==beep_status) 
        STM32_GPIO_SetPin(GPIOA, GPIO_Pin_12, 1);
    else
        STM32_GPIO_SetPin(GPIOA, GPIO_Pin_12, 0); 

}


void Robort_Beep_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 			
  	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;        //GPIOA_Pin12 -- Beep
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    STM32_GPIO_SetPin(GPIOA, GPIO_Pin_12, 0); 
}




