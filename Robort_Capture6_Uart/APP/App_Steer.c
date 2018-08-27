#include "App_Steer.h"  
//舵机控制   云台控制

/*************************************************************************************
*函  数   名：calDutyCycle
*函 数 功 能：根据角度值计算输出占空比
*参       数：turnAngle 转过的角度
*返  回   值：out_dutycycle  占空比
*时       间：2018.8.3
*备       注：Z Rui
*************************************************************************************/
static unsigned char calDutyCycle(short turnAngle)
{
	uint8 out_dutycycle = 0;
	
	//计算占空比
	out_dutycycle = turnAngle * ((ANGLE180_DUTY_CYCLE-ANGLE0_DUTY_CYCLE) * 1.0 / 
                    (ANGLE_MAX_ANGLE - ANGLE_MIN_ANGLE)) + ANGLE0_DUTY_CYCLE;
	
    return out_dutycycle;
}


/*************************************************************************************
*函  数   名：SteerOneControl
*函 数 功 能：控制单个舵机，通过需要转过的角度，配置相应tim的PWM输出
*参       数：turnAngle 转过的角度
*返  回   值：无
*时       间：2018.8.3
*备       注：Z Rui
*************************************************************************************/


//1号舵机角度控制
static void  SteerOneControl(short turnAngle)
{
	uint8 dutyCycle= 0;
	
	dutyCycle = calDutyCycle(turnAngle); //计算转动后对应的脉冲cnt	

	STM32_TIMER3_PWMControl(TIM3_PWM1, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}

//2号舵机角度控制
static void  SteerTwoControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //计算转动后对应的脉冲cnt	

	STM32_TIMER3_PWMControl(TIM3_PWM2, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}

//3号舵机角度控制
static void  SteerThreeControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//计算转动后对应的脉冲cnt	

	STM32_TIMER3_PWMControl(TIM3_PWM3, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}
//4号舵机角度控制
static void  SteerFourControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//计算转动后对应的脉冲cnt	

	STM32_TIMER3_PWMControl(TIM3_PWM4, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}
//5号舵机角度控制
static void  SteerFiveControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//计算转动后对应的脉冲cnt	

	STM32_TIMER3_PWMControl(TIM4_PWM1, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}
//6号舵机角度控制
static void  SteerSixControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//计算转动后对应的脉冲cnt	

	STM32_TIMER3_PWMControl(TIM4_PWM2, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}
//7号舵机角度控制
static void  SteerSevenControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//计算转动后对应的脉冲cnt	

	STM32_TIMER3_PWMControl(TIM4_PWM3, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}
//8号舵机角度控制
static void  SteerEightControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//计算转动后对应的脉冲cnt	

	STM32_TIMER3_PWMControl(TIM4_PWM4, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}


/*************************************************************************************
*函  数   名：Robort_Steer_Control
*函 数 功 能：舵机的角度控制  综合控制
*参       数：steerNo：第NO号舵机  turnAngle：转过的角度
*返  回   值：无
*时       间：2018.8.3
*备       注：Z Rui
*************************************************************************************/

char Robort_Steer_Control(ROBORT_STEER_TAG steerNo,  short turnAngle)
{
//	if (steerNo!=ROBORT_STEER1 && steerNo!=ROBORT_STEER2)
//		return -1;
	
	if (turnAngle<0 || turnAngle>180)//错误反馈-1
		return -1;

	if (steerNo == ROBORT_STEER1)
	{
		SteerOneControl(turnAngle); //1号舵机控制
	}
	else if( steerNo== ROBORT_STEER2)
	{
		SteerTwoControl(turnAngle); //2号舵机控制
	}
	else if( steerNo== ROBORT_STEER3)
	{
		SteerThreeControl(turnAngle); //3号舵机控制
	}
	else if( steerNo== ROBORT_STEER4)
	{
		SteerFourControl(turnAngle); //4号舵机控制
	}
	else if( steerNo== ROBORT_STEER5)
	{
		SteerFiveControl(turnAngle); //5号舵机控制
	}
	else if( steerNo== ROBORT_STEER6)
	{
		SteerSixControl(turnAngle); //6号舵机控制
	}
	else if( steerNo== ROBORT_STEER7)
	{
		SteerSevenControl(turnAngle); //7号舵机控制
	}
	else if( steerNo== ROBORT_STEER8)
	{
		SteerEightControl(turnAngle); //8号舵机控制
	}
	return 0;
}

/*************************************************************************************
*函  数   名：Robort_Steer_Init
*函 数 功 能：舵机初始化
*参       数：无
*返  回   值：无
*时       间：2018.8.3
*备       注：Z Rui
*************************************************************************************/
void Robort_Steer_Init(void)
{
    TIM3_PWM_Init(20000,71);
    STM32_TIMER3_PWMControl(TIM3_PWM1, CLOUD_STAGE_STEER_PERIOD, 75); //steer1-PA6: 20ms/1.5ms 
    STM32_TIMER3_PWMControl(TIM3_PWM2, CLOUD_STAGE_STEER_PERIOD, 75); //steer2-PA7: 20ms/1.5ms 
	  STM32_TIMER3_PWMControl(TIM3_PWM3, CLOUD_STAGE_STEER_PERIOD, 75); //steer1-PA6: 20ms/1.5ms 
    STM32_TIMER3_PWMControl(TIM3_PWM4, CLOUD_STAGE_STEER_PERIOD, 75); //steer2-PA7: 20ms/1.5ms 
	  TIM4_PWM_Init(20000,71);
	  STM32_TIMER4_PWMControl(TIM4_PWM1, CLOUD_STAGE_STEER_PERIOD, 75); //steer1-PA6: 20ms/1.5ms 
     
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                            Next is  Arm steer 's controller
/////////////////////////////////////////////////////////////////////////////////////////////////////