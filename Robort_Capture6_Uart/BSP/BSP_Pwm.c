#include "BSP_Pwm.h"

/*************************************************************************************
*函  数   名：TIM2_PWM_Init
*函 数 功 能：TIM2  PWM输出初始化
*参       数：arr：自动重装值   psc：时钟预分频数
*返  回   值：无
*输       入：无
*输       出：TIM2输出四路PWM,通过GPIO引脚复用，对TIM2的四个输出通道引脚对应为  PA0 PA1 PA2 PA3
*时       间：2018.7.31
*备       注：Z Rui
*************************************************************************************/

void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//使能TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    
	
	//使能GPIOA外设时钟使能
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  
	   
	//GPIOA  初始化配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //TIM_CH1、2、3、4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //GPIOA  初始化
	
	//设置基本频率，用来控制输出PWM的周期
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIM2时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
  //TIM2的CH1 2 3 4输出配置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高	
	
	//TIM2的CH1 2 3 4的输出初始化
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIM2的CH1
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIM2的CH2
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIM2的CH3
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIM2的CH4

	//MOE 主输出使能	
   TIM_CtrlPWMOutputs(TIM2,ENABLE);	

	//装载使能
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1预装载使能
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH2预装载使能
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH3预装载使能
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH4预装载使能
	
	//使能TIM2  
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIM2在ARR上的预装载寄存器
	TIM_SetCompare1(TIM2,100);
	TIM_SetCompare2(TIM2,100);
	TIM_SetCompare3(TIM2,100);
	TIM_SetCompare4(TIM2,100);
	TIM_Cmd(TIM2, ENABLE);  
}


/*************************************************************************************
*函  数   名：STM32_TIMER2_PWMControl
*函 数 功 能：控制TIM2某通道输出PWM的周期，占空比
*参       数：PWMNO：TIM2的第NO个通道  period:自动重装载值     dutycycle:占空比—dutyCycle取值0~1000,代表0.1%~100.0%
*返  回   值：无
*时       间：2018.7.31
*备       注：Z Rui
*************************************************************************************/
void STM32_TIMER2_PWMControl(u8 pwmNo, u32 period, u16 dutyCycle)
{
	u16 Temp;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = period;//设置自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	Temp =period-dutyCycle*(period/1000);  //计算与固有频率比较的比较值
	switch(pwmNo)
		{
			case 1 : TIM_SetCompare1(TIM2,Temp); break;  //TIM2_CH1开始以temp脉冲开始输出
			case 2 : TIM_SetCompare2(TIM2,Temp); break; //TIM2_CH2
			case 3 : TIM_SetCompare3(TIM2,Temp); break; //TIM2_CH3
			case 4 : TIM_SetCompare4(TIM2,Temp); break; //TIM2_CH4
			default:                             break;
	}
}
/*************************************************************************************
*函  数   名：TIM3_PWM_Init
*函 数 功 能：TIM3  PWM输出初始化
*参       数：arr：自动重装值   psc：时钟预分频数
*返  回   值：无
*输       入：无
*输       出：输出四路PWM,通过GPIO引脚复用，对TIM3的四个输出通道引脚对对应为  PA0 PA1 PA2 PA3
*时       间：2018.7.31
*备       注：Z Rui
*************************************************************************************/

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIOB外设时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIOA外设时钟使能                                                                    	
	
	//没有重映射，TIM3的四个通道 ch1 2 3 4 分别对应PA6 PA7 PB0 PB1
	
	//GPIOA  初始化配置    设置该引脚为复用输出功能,输出TIM3 CH1 TIM3 _CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //TIM3_CH1---PA6   TIM3_CH2--PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//GPIOB 初始化配置    设置该引脚为复用输出功能,输出TIM3 CH3 TIM3 _CH4的PWM脉冲波形
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM3_CH3---PB0   TIM3_CH3--PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//设置基本频率，用来控制输出PWM的周期
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

  //TIM3的CH1 2 3 4输出配置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
		
	//TIM3的CH1 2 3 4的输出初始化
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	//MOE 主输出使能	
  TIM_CtrlPWMOutputs(TIM3,ENABLE);	

	//装载使能
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1预装载使能
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH2预装载使能
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH3预装载使能
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH4预装载使能
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	//使能TIM3  
	TIM_SetCompare1(TIM3,100);
	TIM_SetCompare2(TIM3,100);
	TIM_SetCompare3(TIM3,100);
	TIM_SetCompare4(TIM3,100);
	TIM_Cmd(TIM3, ENABLE);  
   
}
/*************************************************************************************
*函  数   名：STM32_TIMER3_PWMControl
*函 数 功 能；控制TIM3某通道输出PWM的周期，占空比
*参       数：PWMNO：TIM3的第NO个通道  period:自动重装载值     dutycycle:占空比——dutyCycle取值0~1000,代表0.1%~100.0%
*返  回   值：无
*时       间：2018.7.31
*备       注：Z Rui
*************************************************************************************/
void STM32_TIMER3_PWMControl(u8 pwmNo, u32 period, u16 dutyCycle)
{
	u16 Temp;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = period;//设置自动重装载值 
	TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	Temp =period-dutyCycle*(period/1000); //计算与固有频率比较的比较值
	
	switch(pwmNo)
		{
			case TIM3_PWM1: TIM_SetCompare1(TIM3,Temp); break;//TIM3_CH1开始以temp脉冲开始输出
			case TIM3_PWM2: TIM_SetCompare2(TIM3,Temp); break;//TIM3_CH2
			case TIM3_PWM3 : TIM_SetCompare3(TIM3,Temp); break;//TIM3_CH3
			case TIM3_PWM4 : TIM_SetCompare4(TIM3,Temp); break;//TIM3_CH4
			default:                             break;
	}
}
/*************************************************************************************
*函  数   名：TIM4_PWM_Init
*函 数 功 能：TIM4  PWM输出初始化
*参       数：arr：自动重装值   psc：时钟预分频数
*返  回   值：无
*输       入：无
*输       出：输出四路PWM,通过GPIO引脚复用，对TIM4的四个输出通道引脚对应为  PB6 PB7 PB8 PB9
*时       间：2018.7.31
*备       注：Z Rui
*************************************************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设时钟使能
                                                                   	
  //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

  //TIM4的CH1 2 3 4输出配置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
		//TIM4的CH1 2 3 4的输出初始化
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	//MOE 主输出使能	
  TIM_CtrlPWMOutputs(TIM4,ENABLE);	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1预装载使能
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH2预装载使能
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH3预装载使能
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH4预装载使能
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	//使能TIM4  PWM开始输出
	TIM_SetCompare1(TIM4,100);
	TIM_SetCompare2(TIM4,100);
	TIM_SetCompare3(TIM4,100);
	TIM_SetCompare4(TIM4,100);
	TIM_Cmd(TIM4, ENABLE);  
   
}
/*************************************************************************************
*函  数   名：STM32_TIMER3_PWMControl
*函 数 功 能：控制TIM4某通道输出PWM的周期，占空比
*参       数：PWMNO：TIM4的第NO个通道  period:自动重装载值     dutycycle:占空比—dutyCycle取值0~1000,代表0.1%~100.0%
*返  回   值：无
*时       间：2018.7.31
*备       注：Z Rui
*************************************************************************************/
void STM32_TIMER4_PWMControl(u8 pwmNo, u32 period, u16 dutyCycle)
{
	u16 Temp;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = period;//设置自动重装载值 
	TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	Temp =period-dutyCycle*(period/1000);//计算与固有频率比较的比较值
	
	switch(pwmNo)
		{
			case 1 : TIM_SetCompare1(TIM4,Temp); break;//TIM4_CH1开始以temp脉冲开始输出
			case 2 : TIM_SetCompare2(TIM4,Temp); break;//TIM4_CH2
			case 3 : TIM_SetCompare3(TIM4,Temp); break;//TIM4_CH3
			case 4 : TIM_SetCompare4(TIM4,Temp); break;//TIM4_CH4
			default:                             break;
	}
}

/*********************************************END OF FILE**********************/
