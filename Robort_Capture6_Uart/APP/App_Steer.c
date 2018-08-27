#include "App_Steer.h"  
//�������   ��̨����

/*************************************************************************************
*��  ��   ����calDutyCycle
*�� �� �� �ܣ����ݽǶ�ֵ�������ռ�ձ�
*��       ����turnAngle ת���ĽǶ�
*��  ��   ֵ��out_dutycycle  ռ�ձ�
*ʱ       �䣺2018.8.3
*��       ע��Z Rui
*************************************************************************************/
static unsigned char calDutyCycle(short turnAngle)
{
	uint8 out_dutycycle = 0;
	
	//����ռ�ձ�
	out_dutycycle = turnAngle * ((ANGLE180_DUTY_CYCLE-ANGLE0_DUTY_CYCLE) * 1.0 / 
                    (ANGLE_MAX_ANGLE - ANGLE_MIN_ANGLE)) + ANGLE0_DUTY_CYCLE;
	
    return out_dutycycle;
}


/*************************************************************************************
*��  ��   ����SteerOneControl
*�� �� �� �ܣ����Ƶ��������ͨ����Ҫת���ĽǶȣ�������Ӧtim��PWM���
*��       ����turnAngle ת���ĽǶ�
*��  ��   ֵ����
*ʱ       �䣺2018.8.3
*��       ע��Z Rui
*************************************************************************************/


//1�Ŷ���Ƕȿ���
static void  SteerOneControl(short turnAngle)
{
	uint8 dutyCycle= 0;
	
	dutyCycle = calDutyCycle(turnAngle); //����ת�����Ӧ������cnt	

	STM32_TIMER3_PWMControl(TIM3_PWM1, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}

//2�Ŷ���Ƕȿ���
static void  SteerTwoControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //����ת�����Ӧ������cnt	

	STM32_TIMER3_PWMControl(TIM3_PWM2, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}

//3�Ŷ���Ƕȿ���
static void  SteerThreeControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//����ת�����Ӧ������cnt	

	STM32_TIMER3_PWMControl(TIM3_PWM3, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}
//4�Ŷ���Ƕȿ���
static void  SteerFourControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//����ת�����Ӧ������cnt	

	STM32_TIMER3_PWMControl(TIM3_PWM4, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}
//5�Ŷ���Ƕȿ���
static void  SteerFiveControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//����ת�����Ӧ������cnt	

	STM32_TIMER3_PWMControl(TIM4_PWM1, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}
//6�Ŷ���Ƕȿ���
static void  SteerSixControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//����ת�����Ӧ������cnt	

	STM32_TIMER3_PWMControl(TIM4_PWM2, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}
//7�Ŷ���Ƕȿ���
static void  SteerSevenControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//����ת�����Ӧ������cnt	

	STM32_TIMER3_PWMControl(TIM4_PWM3, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}
//8�Ŷ���Ƕȿ���
static void  SteerEightControl(short turnAngle)
{
	unsigned char dutyCycle = 0;

	dutyCycle= calDutyCycle(turnAngle); //;//����ת�����Ӧ������cnt	

	STM32_TIMER3_PWMControl(TIM4_PWM4, CLOUD_STAGE_STEER_PERIOD, dutyCycle); 
}


/*************************************************************************************
*��  ��   ����Robort_Steer_Control
*�� �� �� �ܣ�����ĽǶȿ���  �ۺϿ���
*��       ����steerNo����NO�Ŷ��  turnAngle��ת���ĽǶ�
*��  ��   ֵ����
*ʱ       �䣺2018.8.3
*��       ע��Z Rui
*************************************************************************************/

char Robort_Steer_Control(ROBORT_STEER_TAG steerNo,  short turnAngle)
{
//	if (steerNo!=ROBORT_STEER1 && steerNo!=ROBORT_STEER2)
//		return -1;
	
	if (turnAngle<0 || turnAngle>180)//������-1
		return -1;

	if (steerNo == ROBORT_STEER1)
	{
		SteerOneControl(turnAngle); //1�Ŷ������
	}
	else if( steerNo== ROBORT_STEER2)
	{
		SteerTwoControl(turnAngle); //2�Ŷ������
	}
	else if( steerNo== ROBORT_STEER3)
	{
		SteerThreeControl(turnAngle); //3�Ŷ������
	}
	else if( steerNo== ROBORT_STEER4)
	{
		SteerFourControl(turnAngle); //4�Ŷ������
	}
	else if( steerNo== ROBORT_STEER5)
	{
		SteerFiveControl(turnAngle); //5�Ŷ������
	}
	else if( steerNo== ROBORT_STEER6)
	{
		SteerSixControl(turnAngle); //6�Ŷ������
	}
	else if( steerNo== ROBORT_STEER7)
	{
		SteerSevenControl(turnAngle); //7�Ŷ������
	}
	else if( steerNo== ROBORT_STEER8)
	{
		SteerEightControl(turnAngle); //8�Ŷ������
	}
	return 0;
}

/*************************************************************************************
*��  ��   ����Robort_Steer_Init
*�� �� �� �ܣ������ʼ��
*��       ������
*��  ��   ֵ����
*ʱ       �䣺2018.8.3
*��       ע��Z Rui
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