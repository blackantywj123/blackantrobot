#include "APP_Capture.h"


/*******************************************************************
*****  编码器配置文件 
*****  
*****  资源：TIM4_1，TIM3_3
*****  作者：SunnnnnnnK
*******************************************************************/
#define K                   1        /* distance = k*n -------  编码器数值*k    *///BUG: r and distance
#define Reduce              6        /* reduce accuracy to avoid oversize number*/
#define distance            30000/K    /* initial distance: distance -> 0*///Output initial number
#define distance_high    distance / 0xff
#define distance_low     distance % 0xff

struct Capture_Info Capture_Data;  // upload data

// Capture right
float k = K;										  // distance = k*n -------  编码器数值 * k
u8 direct = Direction_1;				  // Direction_1 OR Direction_0
u8 TIM4CH1_CAPTURE_STATUS=0;
u8 TIM4CH1_CAPTURE_1 = distance_low;		   // num
u32 TIM4CH1_CAPTURE_OXFF = distance_high;   // num * 0xff
u32 TIM4CH1_CAPTURE_DISTANCE = distance;// read only ---------extern //编码器的distance,不是实际的distance
// Capture left
float k2 = 1/K;										// k = n/distance
u8 direct2 = 1;				//Direction_1 OR Direction_0
u8 TIM3CH3_CAPTURE_STATUS=0;
u8 TIM3CH3_CAPTURE_1 = distance_low;				//num
u32 TIM3CH3_CAPTURE_OXFF = distance_high;//num * 0xff
u32 TIM3CH3_CAPTURE_DISTANCE = distance;// read only ---------extern

/*******************************************************************
*****  中断库文件配置 -> Right
*****  硬件：PB6，PB7 对应封装 P7,p8
*****  软件：TIM4
*****  作者：SunnnnnnnK
*******************************************************************/
void Robot_Capture_Init()
{
	IN_Capture_r();
	IN_Capture_l();
}
void Robort_Get_Capture()
{
	Capture_Data.right.fright = TIM4CH1_CAPTURE_DISTANCE;
	Capture_Data.left.fleft = TIM3CH3_CAPTURE_DISTANCE;
}
/*******************************************************************
*****  中断库文件配置 -> Right
*****  硬件：PB6，PB7 对应封装 P7,p8
*****  软件：TIM4
*****  作者：SunnnnnnnK
*******************************************************************/
void IN_Capture_r()
{
	// capture 1 -------right
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	/* rcc enable -------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	/* GPIO_init_Capture_CH1_PB6-steel5---------*///P7 in hardware
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	/* GPIO_init_Direction_CH1_PB7 -steel6------*///P8 in hardware
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;				 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; //float in////////////////////////////
	GPIO_Init(GPIOB, &GPIO_InitStruct);		
	/* timer initial ----------------------*/
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_Period = 0xffff; //65535
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	/* input capture config ---------------*/
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;// rising signal
	TIM_ICInitStruct.TIM_ICFilter = 0x0000;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;//???
	TIM_ICInit(TIM4,&TIM_ICInitStruct);
	/* allow updata intertupt & allow CC1IE capture intertupt */
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	/* interrupt group config -------------*/
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	/* allow tim interrupt TIM5 -----------*/
	TIM_Cmd(TIM4,ENABLE);	
}


/*******************************************************************
*****  编码器配置 -> Right
*****  硬件：PB6，PB7 对应封装 P7,p8
*****  extern: TIM4CH1_CAPTURE_DISTANCE
*****  作者：SunnnnnnnK
*******************************************************************/
void TIM4_IRQHandler()//TIM4中断处理
{
	static u8 i;
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1)==SET)//time of capture
	{
		i++;
		if(i==Reduce)//-------------------------reduce accuracy to avoid oversize number
		{
			i=0;
			TIM4CH1_CAPTURE_STATUS |= 0x80;//a rising signal coming
			/* ---------count signal ---------*/
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==direct) TIM4CH1_CAPTURE_1++;
			else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1-direct) TIM4CH1_CAPTURE_1--;
			/* --------calculate part-------- */
			if(TIM4CH1_CAPTURE_1==0xff)
			{
				TIM4CH1_CAPTURE_OXFF ++;
				TIM4CH1_CAPTURE_1 = 0x00;
			}	
			else if((TIM4CH1_CAPTURE_1==0x00))
			{
				TIM4CH1_CAPTURE_OXFF --;
				TIM4CH1_CAPTURE_1 = 0xff;
			}			
			/* ------calculate distance------ */
			TIM4CH1_CAPTURE_DISTANCE = k*(TIM4CH1_CAPTURE_1+(0xff)*TIM4CH1_CAPTURE_OXFF);
		}
	}
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update); //clear interrupt flag bit
}


/*******************************************************************
*****  中断库文件配置 -> Left
*****  硬件：PB0，PB1 对应封装 P4,P5
*****  软件：TIM3
*****  作者：SunnnnnnnK
*******************************************************************/
void IN_Capture_l()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	/* rcc enable -------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	/* GPIO_init_Capture_CH1_PB6-steel5---------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	/* GPIO_init_Direction_CH1_PB7 -steel6-------*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;				 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; 		 //float in////////////////////
	GPIO_Init(GPIOB, &GPIO_InitStruct);		
	/* timer initial ----------------------*/
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_Period = 0xffff; //65535
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	/* input capture config ---------------*/
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_3;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;//???
	TIM_ICInitStruct.TIM_ICFilter = 0x0000;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;//???
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	/* allow updata intertupt & allow CC1IE capture intertupt */
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);
	/* interrupt group config -------------*/
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_Init(&NVIC_InitStruct);
	/* allow tim interrupt TIM5 -----------*/
	TIM_Cmd(TIM3,ENABLE);	
}


/*******************************************************************
*****  编码器配置 -> Left
*****  硬件：PB0，PB1 对应封装 P4,P5
*****  软件：TIM3
*****  extern: TIM3CH3_CAPTURE_DISTANCE
*****  作者：SunnnnnnnK
*******************************************************************/
void TIM3_IRQHandler()
{
	static u8 i;
	if(TIM_GetITStatus(TIM3,TIM_IT_CC3)==SET)//time of capture
	{
		i++;
		if(i==Reduce)//-------------------------reduce accuracy to avoid oversize number
		{
			i=0;
			TIM3CH3_CAPTURE_STATUS |= 0x80;//a rising signal coming
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==direct2) TIM3CH3_CAPTURE_1++;
			else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1-direct2) TIM3CH3_CAPTURE_1--;
			if(TIM3CH3_CAPTURE_1==0xff)
			{
				TIM3CH3_CAPTURE_OXFF ++;
				TIM3CH3_CAPTURE_1 = 0x00;
			}	
			else if((TIM3CH3_CAPTURE_1==0x00))
			{
				TIM3CH3_CAPTURE_OXFF --;
				TIM3CH3_CAPTURE_1 = 0xff;
			}
			//calculate distance	
			TIM3CH3_CAPTURE_DISTANCE = k2*(TIM3CH3_CAPTURE_1+(0xff)*TIM3CH3_CAPTURE_OXFF);
		}
	}
		
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC3|TIM_IT_Update); //clear interrupt flag bit
}

void Capture_Clear_Zero()
{
	TIM4CH1_CAPTURE_DISTANCE = distance;
	TIM3CH3_CAPTURE_DISTANCE = distance;
	TIM4CH1_CAPTURE_1 = distance_low;		   // num
	TIM4CH1_CAPTURE_OXFF = distance_high;
	TIM3CH3_CAPTURE_1 = distance_low;				//num
	TIM3CH3_CAPTURE_OXFF = distance_high;//num * 0xff	
}