#include "App_Light.h"


void Robort_Light_Control(ROBORT_LIGHT_NO_TAG light_no, ROBORT_LIGHT_TAG light_status)
{

    switch (light_no)
    {
        case ROBORT_LIGHT_ONE:
              Control_1stLight(light_status);
        break;
        
        case ROBORT_LIGHT_TWO:
              Control_2ndLight(light_status);
        break;
        
        default: break;
    }
    
}

static void Control_1stLight(ROBORT_LIGHT_TAG light_status)
{
    (ROBORT_LIGHT_ON==light_status) ? 
        STM32_GPIO_SetPin(GPIOA, GPIO_Pin_11, 1) : STM32_GPIO_SetPin(GPIOA, GPIO_Pin_11, 0); 
}

static void Control_2ndLight(ROBORT_LIGHT_TAG light_status)
{

}

void Robort_Light_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 			
  	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;        //GPIOA_Pin11 -- LIGHT
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    STM32_GPIO_SetPin(GPIOA, GPIO_Pin_11, 0); 
}

