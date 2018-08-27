#include "BSP_Pwm.h"

/*************************************************************************************
*��  ��   ����TIM2_PWM_Init
*�� �� �� �ܣ�TIM2  PWM�����ʼ��
*��       ����arr���Զ���װֵ   psc��ʱ��Ԥ��Ƶ��
*��  ��   ֵ����
*��       �룺��
*��       ����TIM2�����·PWM,ͨ��GPIO���Ÿ��ã���TIM2���ĸ����ͨ�����Ŷ�ӦΪ  PA0 PA1 PA2 PA3
*ʱ       �䣺2018.7.31
*��       ע��Z Rui
*************************************************************************************/

void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//ʹ��TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    
	
	//ʹ��GPIOA����ʱ��ʹ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  
	   
	//GPIOA  ��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //TIM_CH1��2��3��4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //GPIOA  ��ʼ��
	
	//���û���Ƶ�ʣ������������PWM������
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIM2ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
  //TIM2��CH1 2 3 4�������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�	
	
	//TIM2��CH1 2 3 4�������ʼ��
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIM2��CH1
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIM2��CH2
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIM2��CH3
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIM2��CH4

	//MOE �����ʹ��	
   TIM_CtrlPWMOutputs(TIM2,ENABLE);	

	//װ��ʹ��
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��
	
	//ʹ��TIM2  
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIM2��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_SetCompare1(TIM2,100);
	TIM_SetCompare2(TIM2,100);
	TIM_SetCompare3(TIM2,100);
	TIM_SetCompare4(TIM2,100);
	TIM_Cmd(TIM2, ENABLE);  
}


/*************************************************************************************
*��  ��   ����STM32_TIMER2_PWMControl
*�� �� �� �ܣ�����TIM2ĳͨ�����PWM�����ڣ�ռ�ձ�
*��       ����PWMNO��TIM2�ĵ�NO��ͨ��  period:�Զ���װ��ֵ     dutycycle:ռ�ձȡ�dutyCycleȡֵ0~1000,����0.1%~100.0%
*��  ��   ֵ����
*ʱ       �䣺2018.7.31
*��       ע��Z Rui
*************************************************************************************/
void STM32_TIMER2_PWMControl(u8 pwmNo, u32 period, u16 dutyCycle)
{
	u16 Temp;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = period;//�����Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	Temp =period-dutyCycle*(period/1000);  //���������Ƶ�ʱȽϵıȽ�ֵ
	switch(pwmNo)
		{
			case 1 : TIM_SetCompare1(TIM2,Temp); break;  //TIM2_CH1��ʼ��temp���忪ʼ���
			case 2 : TIM_SetCompare2(TIM2,Temp); break; //TIM2_CH2
			case 3 : TIM_SetCompare3(TIM2,Temp); break; //TIM2_CH3
			case 4 : TIM_SetCompare4(TIM2,Temp); break; //TIM2_CH4
			default:                             break;
	}
}
/*************************************************************************************
*��  ��   ����TIM3_PWM_Init
*�� �� �� �ܣ�TIM3  PWM�����ʼ��
*��       ����arr���Զ���װֵ   psc��ʱ��Ԥ��Ƶ��
*��  ��   ֵ����
*��       �룺��
*��       ���������·PWM,ͨ��GPIO���Ÿ��ã���TIM3���ĸ����ͨ�����ŶԶ�ӦΪ  PA0 PA1 PA2 PA3
*ʱ       �䣺2018.7.31
*��       ע��Z Rui
*************************************************************************************/

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIOB����ʱ��ʹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIOA����ʱ��ʹ��                                                                    	
	
	//û����ӳ�䣬TIM3���ĸ�ͨ�� ch1 2 3 4 �ֱ��ӦPA6 PA7 PB0 PB1
	
	//GPIOA  ��ʼ������    ���ø�����Ϊ�����������,���TIM3 CH1 TIM3 _CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //TIM3_CH1---PA6   TIM3_CH2--PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//GPIOB ��ʼ������    ���ø�����Ϊ�����������,���TIM3 CH3 TIM3 _CH4��PWM���岨��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM3_CH3---PB0   TIM3_CH3--PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//���û���Ƶ�ʣ������������PWM������
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

  //TIM3��CH1 2 3 4�������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
		
	//TIM3��CH1 2 3 4�������ʼ��
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	//MOE �����ʹ��	
  TIM_CtrlPWMOutputs(TIM3,ENABLE);	

	//װ��ʹ��
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	//ʹ��TIM3  
	TIM_SetCompare1(TIM3,100);
	TIM_SetCompare2(TIM3,100);
	TIM_SetCompare3(TIM3,100);
	TIM_SetCompare4(TIM3,100);
	TIM_Cmd(TIM3, ENABLE);  
   
}
/*************************************************************************************
*��  ��   ����STM32_TIMER3_PWMControl
*�� �� �� �ܣ�����TIM3ĳͨ�����PWM�����ڣ�ռ�ձ�
*��       ����PWMNO��TIM3�ĵ�NO��ͨ��  period:�Զ���װ��ֵ     dutycycle:ռ�ձȡ���dutyCycleȡֵ0~1000,����0.1%~100.0%
*��  ��   ֵ����
*ʱ       �䣺2018.7.31
*��       ע��Z Rui
*************************************************************************************/
void STM32_TIMER3_PWMControl(u8 pwmNo, u32 period, u16 dutyCycle)
{
	u16 Temp;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = period;//�����Զ���װ��ֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	Temp =period-dutyCycle*(period/1000); //���������Ƶ�ʱȽϵıȽ�ֵ
	
	switch(pwmNo)
		{
			case TIM3_PWM1: TIM_SetCompare1(TIM3,Temp); break;//TIM3_CH1��ʼ��temp���忪ʼ���
			case TIM3_PWM2: TIM_SetCompare2(TIM3,Temp); break;//TIM3_CH2
			case TIM3_PWM3 : TIM_SetCompare3(TIM3,Temp); break;//TIM3_CH3
			case TIM3_PWM4 : TIM_SetCompare4(TIM3,Temp); break;//TIM3_CH4
			default:                             break;
	}
}
/*************************************************************************************
*��  ��   ����TIM4_PWM_Init
*�� �� �� �ܣ�TIM4  PWM�����ʼ��
*��       ����arr���Զ���װֵ   psc��ʱ��Ԥ��Ƶ��
*��  ��   ֵ����
*��       �룺��
*��       ���������·PWM,ͨ��GPIO���Ÿ��ã���TIM4���ĸ����ͨ�����Ŷ�ӦΪ  PB6 PB7 PB8 PB9
*ʱ       �䣺2018.7.31
*��       ע��Z Rui
*************************************************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
                                                                   	
  //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

  //TIM4��CH1 2 3 4�������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
		//TIM4��CH1 2 3 4�������ʼ��
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	//MOE �����ʹ��	
  TIM_CtrlPWMOutputs(TIM4,ENABLE);	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	//ʹ��TIM4  PWM��ʼ���
	TIM_SetCompare1(TIM4,100);
	TIM_SetCompare2(TIM4,100);
	TIM_SetCompare3(TIM4,100);
	TIM_SetCompare4(TIM4,100);
	TIM_Cmd(TIM4, ENABLE);  
   
}
/*************************************************************************************
*��  ��   ����STM32_TIMER3_PWMControl
*�� �� �� �ܣ�����TIM4ĳͨ�����PWM�����ڣ�ռ�ձ�
*��       ����PWMNO��TIM4�ĵ�NO��ͨ��  period:�Զ���װ��ֵ     dutycycle:ռ�ձȡ�dutyCycleȡֵ0~1000,����0.1%~100.0%
*��  ��   ֵ����
*ʱ       �䣺2018.7.31
*��       ע��Z Rui
*************************************************************************************/
void STM32_TIMER4_PWMControl(u8 pwmNo, u32 period, u16 dutyCycle)
{
	u16 Temp;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = period;//�����Զ���װ��ֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	Temp =period-dutyCycle*(period/1000);//���������Ƶ�ʱȽϵıȽ�ֵ
	
	switch(pwmNo)
		{
			case 1 : TIM_SetCompare1(TIM4,Temp); break;//TIM4_CH1��ʼ��temp���忪ʼ���
			case 2 : TIM_SetCompare2(TIM4,Temp); break;//TIM4_CH2
			case 3 : TIM_SetCompare3(TIM4,Temp); break;//TIM4_CH3
			case 4 : TIM_SetCompare4(TIM4,Temp); break;//TIM4_CH4
			default:                             break;
	}
}

/*********************************************END OF FILE**********************/
