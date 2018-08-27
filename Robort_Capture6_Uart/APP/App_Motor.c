#include "App_Motor.h"

static uint8 gRobortSpeed_L = 0;  //����ٶ�
static uint8 gRobortSpeed_R = 0;  //�Ҳ��ٶ�
static uint8 gFlagSpeedRset = 0;  //��־Ϊ0������ͻ���û���趨�ٶȣ���־Ϊ1������ͻ����趨���ٶ�

/*************************************************************************************
*��  ��   ����Robort_Direction_Control
*�� �� �� �ܣ����ҵ���ٶȿ���    �ܿ���
*��       ����EN ʹ�ܵ���ٶȿ���  
              ctl_l �����Ƿ�����ȫ���٣�ǰ���ٶ�Ϊ0��
              ctl_r �ҵ���Ƿ�����ȫ���٣�ǰ���ٶ�Ϊ0��
              speed_l  ������ٶ�   speed_r  �Ҳ����ٶ�
*��  ��   ֵ����
*ʹ��   �ȼ���һ��ʹ��
*ʱ       �䣺2018.8.3
*��       ע��Z Rui
*************************************************************************************/
void Robort_Direction_Control(uint8 EN,uint8 ctl_l, uint8 ctl_r,uint8 speed_l,uint8 speed_r)
{
	Robort_Speed_Control(ROBORT_SPEED_L, speed_l); //�������speed_l�ٶ�����
	Robort_Speed_Control(ROBORT_SPEED_R, speed_r);  //�Ҳ�����speed_r�ٶ�����
	
	//���ٱ����趨
	uint8 sysSpeed_L = (gFlagSpeedRset==0) ? 30 : gRobortSpeed_L;  //����ͻ���û���趨�ٶȣ��������ٶ�=30;��������ͻ����趨���ٶȣ����趨���ٶ���
	uint8 sysSpeed_R = (gFlagSpeedRset==0) ? 30 : gRobortSpeed_R;  //����ͻ���û���趨�ٶȣ��������ٶ�=30;��������ͻ����趨���ٶȣ����趨���ٶ�
	if(ctl_l >= 2)  sysSpeed_L = 0; //���ctl>2�����ٵ�ʱ��ǰ���ٶ�Ϊ0
	if(ctl_r >= 2)  sysSpeed_R = 0;	
	
	if(EN)           //EN=1������ǰ���ֲ��٣�EN=0��ǰ���ֲ����Բ���
	{		
		Robort_Speed_Control(ROBORT_SPEED_L, speed_l); //�������speed_l�ٶ�����
		Robort_Speed_Control(ROBORT_SPEED_R, speed_r); //�Ҳ�����speed_r�ٶ�����
		
		//������ת  ��ǰ�����ٶȱ���
		Set_Motor_Direction(MOTOR_LT, ctl_l); //��ת
		Set_Motor_Speed( MOTOR_LT,  sysSpeed_L); //��ǰ��� ����sysSpeed_L�ٶ�����
		
		//������ת ��ǰ�����ٶȱ���
		Set_Motor_Direction(MOTOR_RT, ctl_r);  //��ת
		Set_Motor_Speed( MOTOR_RT,  sysSpeed_R);//��ǰ��� ����sysSpeed_L�ٶ�����
	}
}

/*************************************************************************************
*��  ��   ����Robort_Speed_Control
*�� �� �� ��: �������ٶȿ���
*��       ����  robort_dir �����/�Ҳ� ������� ��     robort_speed���ٶ�
*��  ��   ֵ����
*ʹ��   �ȼ�������ʹ��  �� Robort_Direction_Control ����
*ʱ       �䣺2018.8.3
*��       ע��Z Rui
*************************************************************************************/
void  Robort_Speed_Control(ROBORT_SPEED_TAG robort_dir, unsigned char robort_speed)
{
    gFlagSpeedRset = 1;   //��־Ϊ1������ͻ����趨���ٶ�
    
    switch (robort_dir)
    {
     case ROBORT_SPEED_L:   //�趨����������ٶ�
     {
          gRobortSpeed_L =  robort_speed;
          break;
     }
     case ROBORT_SPEED_R:   //�趨�����Ҳ����ٶ�
     {
          gRobortSpeed_R =  robort_speed;
          break;
     }  
     default: break;
    }      
}


/*************************************************************************************
*��  ��   ����Set_Motor_Direction
*�� �� �� �ܣ���NO�ŵ���������  
*��       ����  motorNo  ��NO�ŵ��   dir  ���������߷���
*��  ��   ֵ����
*ʹ��   �ȼ�������ʹ��  �� Robort_Direction_Control ����
*ʱ       �䣺2018.8.3
*��       ע��Z Rui
*************************************************************************************/
static void Set_Motor_Direction(MOTOR_TAG motorNo, DIRECTION_TAG  dir)
{
		//��ʼ״̬����
    static MOTOR_TAG preMotorNo = MOTOR_NONE; //�����̬�������������۱仯  �״γ�ʼ״̬���������������κ�һ�����
    static DIRECTION_TAG preDir = MOTOR_NONE; //�������̬�������������۷���仯  ���״γ�ʼ״̬  ��������������ת�����ⷽ��
    
    if (preMotorNo==motorNo && preDir==dir)//��һ�κ���һ�ε�������ͬ����û�ж���
    {
        return ;
    }
		
    
    preMotorNo = motorNo;
    preDir = dir;
    
		//���Ƶĵ��
    switch (motorNo)
    {
     case MOTOR_LT://���ϵ������
     {
        if (dir == DIR_POSITIVE)
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_9, 1);   // DIR1=HIGH
        }
        else
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_9, 0);  // DIR1=LOW
        }
     }
     break;
     
     case MOTOR_RT: //���ϵ������
        if (dir == DIR_POSITIVE)
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_8, 1);   // DIR2=HIGH
        }
        else
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_8, 0);    // DIR2=LOW
        }
     break;
     case MOTOR_LB://���������
        if (dir == DIR_POSITIVE)
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_7, 1);   // DIR3=HIGH
        }
        else
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_7, 0);    // DIR3=LOW
        }
     break;
     case MOTOR_RB://�Һ�������
        if (dir == DIR_POSITIVE)
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_6, 1);   // DIR4=HIGH
        }
        else
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_6, 0);   // DIR4=LOW
        }
     break;  

     default: break;
    }    
    return ;
}

/*************************************************************************************
*��  ��   ����Set_Motor_Speed
*�� �� �� �ܣ���������ٶȿ���   
*��       ����motorNo ��no�ŵ��-> MOTOR_LT,MOTOR_RT,MOTOR_LB,MOTOR_RB,MOTOR_NONE
              speed  ����ٶ�
*��  ��   ֵ��DutyCycle  ռ�ձ� 
*ʹ��   �ȼ�������ʹ�� �� Robort_Direction_Control ����
*ʱ       �䣺2018.8.3
*��       ע��Z Rui
*************************************************************************************/
static void Set_Motor_Speed(MOTOR_TAG motorNo, uint8 speed)
{
    static MOTOR_TAG preMotorNo;   //��̬�������״γ�ʼ״̬�����������κ�һ�����
    static uint8    preSpeed;				//��̬�������״γ�ʼ״̬������ת������Ƕ�
    uint16 ratio = 0;								//ռ�ձ�
    
    if (preMotorNo==motorNo && preSpeed==speed)  
    {
        return ;										// ������ϴ����ָ��û�иı�һ�����Ͳ����κζ���
    }
    
    preMotorNo = motorNo;						// �ṩ��һ�ε� preMotorNo
    preSpeed = speed;								// �ṩ��һ�ε� preSpeed
    
    ratio =  CalPWMDutyCycle(speed);// ��Ҫ����ٶ����ռ�ձ�
    
    switch (motorNo)
    {
     case MOTOR_LT:  //���ϵ������  �ٶ�
        STM32_TIMER2_PWMControl(TIM2_PWM1, MOTOR_PERIOD_DEFAULT, ratio);
     break;
     case MOTOR_RT:  //���ϵ������  �ٶ�
        STM32_TIMER2_PWMControl(TIM2_PWM2, MOTOR_PERIOD_DEFAULT, ratio);
     break;
     case MOTOR_LB://���µ������  �ٶ�
        STM32_TIMER2_PWMControl(TIM2_PWM3, MOTOR_PERIOD_DEFAULT, ratio);
     break;
     case MOTOR_RB://�Һ�������  �ٶ�
        STM32_TIMER2_PWMControl(TIM2_PWM4, MOTOR_PERIOD_DEFAULT, ratio);
     break;  

     default: break;
    }    
    return ;
}






/*************************************************************************************
*��  ��   ����CalPWMDutyCycle
*�� �� �� �ܣ���Speed �� Duty �ı���ת��   
*��       ����speed  ����ٶ�
*��  ��   ֵ��DutyCycle  ռ�ձ�
*ʹ��   �ȼ�������ʹ�� �� Set_Motor_Speed ����
*ʱ       �䣺2018.8.3
*��       ע��Z Rui
*************************************************************************************/
static uint16  CalPWMDutyCycle(unsigned char speed)
{
	uint16 DutyCycle = 0;

	DutyCycle = (uint16) (speed*1.0 / 100 * 1000);
	return DutyCycle;
}


//�����ʼ��
void Robort_Motor_Init(void)
{   
   GPIO_InitTypeDef GPIO_InitStructure;  
 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 			
   /* Pin9-DIR1, Pin8-DIR2, Pin7-DIR3, Pin6-DIR4 */
   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 |GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);  
    
   STM32_GPIO_SetPin(GPIOC, GPIO_Pin_6, 1);
   STM32_GPIO_SetPin(GPIOC, GPIO_Pin_7, 1);
   STM32_GPIO_SetPin(GPIOC, GPIO_Pin_8, 1);
   STM32_GPIO_SetPin(GPIOC, GPIO_Pin_9, 1);
     
   TIM2_PWM_Init(MOTOR_PERIOD_DEFAULT,71);
   STM32_TIMER2_PWMControl(TIM2_PWM1, MOTOR_PERIOD_DEFAULT, 0); // PWM out low 
   STM32_TIMER2_PWMControl(TIM2_PWM2, MOTOR_PERIOD_DEFAULT, 0); // PWM out low 
   STM32_TIMER2_PWMControl(TIM2_PWM3, MOTOR_PERIOD_DEFAULT, 0); // PWM out low 
//   STM32_TIMER2_PWMControl(TIM2_PWM4, MOTOR_PERIOD_DEFAULT, 0); // PWM out low     
   
}
