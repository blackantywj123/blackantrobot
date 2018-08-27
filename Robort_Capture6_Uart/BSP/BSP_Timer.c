#include "Common.h" 

static uint Task_10ms=0;
uchar Task_20ms=0,Task_50ms=0,Task_100ms=0,Task_500ms=0,Task_1s=0;


//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

//	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
// 
//	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
//		TIM3, //TIM2
//		TIM_IT_Update ,
//		ENABLE  //ʹ��
//		);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
//							 
//}
//void NVIC_Configuration(void)
//{

//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//}

//void TIMER_Init(void)
//{
//	NVIC_Configuration();
//	TIM3_Int_Init(99,7199);//10Khz�ļ���Ƶ�ʣ�������99Ϊ10ms 
//}

//void TIM3_IRQHandler(void)   //TIM3�ж�
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
//		{
//			
//			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//			Task_10ms++;
//			if( Task_10ms>=1000){
//				Task_10ms=0;
//			}
//			
//			if( Task_10ms%2==0)
//				Task_20ms=1;
//			
//			if( Task_10ms%5==0)
//				Task_50ms=1;
//			
//			if( Task_10ms%10==0)
//				Task_100ms=1;
//			
//			if( Task_10ms%50==0)
//				Task_500ms=1;
//			
//			if( Task_10ms%100==0)
//				Task_1s=1;
//	
//		}
//}



//u8 STM32_TIMER3_GetCounterFlag(void)
//{
//	
//	u8 temp;
//	temp=Task_20ms+(Task_50ms<<1)+(Task_100ms<<2)+(Task_500ms<<3)+ (Task_1s<<4);
//	return temp;
//	
//}
void TIM1_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM1, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx����
							 
}
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
}

void TIMER_Init(void)
{
	NVIC_Configuration();
	TIM1_Int_Init(39,3599);//10Khz�ļ���Ƶ�ʣ�������99Ϊ10ms 
}

void TIM1_UP_IRQHandler(void)   //TIM3�ж�
{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
//		{
			
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			Task_10ms++;
			if( Task_10ms>=1000){
				Task_10ms=0;
			}
			
			if( Task_10ms%2==0)
				Task_20ms=1;
			
			if( Task_10ms%5==0)
				Task_50ms=1;
			
			if( Task_10ms%10==0)
				Task_100ms=1;
			
			if( Task_10ms%50==0)
				Task_500ms=1;
			
			if( Task_10ms%100==0)
				Task_1s=1;
	
//		}
}



u8 STM32_TIMER3_GetCounterFlag(void)
{
	
	u8 temp;
	temp=Task_20ms+(Task_50ms<<1)+(Task_100ms<<2)+(Task_500ms<<3)+ (Task_1s<<4);
	return temp;

}
/*********************************************END OF FILE**********************/
