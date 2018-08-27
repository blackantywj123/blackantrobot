#include "App_Motor.h"

static uint8 gRobortSpeed_L = 0;  //左侧速度
static uint8 gRobortSpeed_R = 0;  //右侧速度
static uint8 gFlagSpeedRset = 0;  //标志为0，代表客户端没有设定速度；标志为1，代表客户端设定了速度

/*************************************************************************************
*函  数   名：Robort_Direction_Control
*函 数 功 能：左右电机速度控制    总控制
*参       数：EN 使能电机速度控制  
              ctl_l 左电机是否是完全差速（前轮速度为0）
              ctl_r 右电机是否是完全差速（前轮速度为0）
              speed_l  左侧电机速度   speed_r  右侧电机速度
*返  回   值：无
*使用   等级：一级使用
*时       间：2018.8.3
*备       注：Z Rui
*************************************************************************************/
void Robort_Direction_Control(uint8 EN,uint8 ctl_l, uint8 ctl_r,uint8 speed_l,uint8 speed_r)
{
	Robort_Speed_Control(ROBORT_SPEED_L, speed_l); //左侧电机以speed_l速度运行
	Robort_Speed_Control(ROBORT_SPEED_R, speed_r);  //右侧电机以speed_r速度运行
	
	//差速变量设定
	uint8 sysSpeed_L = (gFlagSpeedRset==0) ? 30 : gRobortSpeed_L;  //如果客户端没有设定速度，那左轮速度=30;否则，如果客户端设定了速度，按设定的速度来
	uint8 sysSpeed_R = (gFlagSpeedRset==0) ? 30 : gRobortSpeed_R;  //如果客户端没有设定速度，那右轮速度=30;否则，如果客户端设定了速度，按设定的速度
	if(ctl_l >= 2)  sysSpeed_L = 0; //如果ctl>2，差速的时候前轮速度为0
	if(ctl_r >= 2)  sysSpeed_R = 0;	
	
	if(EN)           //EN=1，可以前后轮差速；EN=0，前后轮不可以差速
	{		
		Robort_Speed_Control(ROBORT_SPEED_L, speed_l); //左侧电机以speed_l速度运行
		Robort_Speed_Control(ROBORT_SPEED_R, speed_r); //右侧电机以speed_r速度运行
		
		//差速左转  左前轮子速度变慢
		Set_Motor_Direction(MOTOR_LT, ctl_l); //左转
		Set_Motor_Speed( MOTOR_LT,  sysSpeed_L); //左前电机 ，以sysSpeed_L速度运行
		
		//差速右转 右前轮子速度变慢
		Set_Motor_Direction(MOTOR_RT, ctl_r);  //右转
		Set_Motor_Speed( MOTOR_RT,  sysSpeed_R);//右前电机 ，以sysSpeed_L速度运行
	}
}

/*************************************************************************************
*函  数   名：Robort_Speed_Control
*函 数 功 能: 单侧电机速度控制
*参       数：  robort_dir ：左侧/右侧 电机控制 ；     robort_speed：速度
*返  回   值：无
*使用   等级：二级使用  被 Robort_Direction_Control 调用
*时       间：2018.8.3
*备       注：Z Rui
*************************************************************************************/
void  Robort_Speed_Control(ROBORT_SPEED_TAG robort_dir, unsigned char robort_speed)
{
    gFlagSpeedRset = 1;   //标志为1，代表客户端设定了速度
    
    switch (robort_dir)
    {
     case ROBORT_SPEED_L:   //设定的是左侧轮速度
     {
          gRobortSpeed_L =  robort_speed;
          break;
     }
     case ROBORT_SPEED_R:   //设定的是右测轮速度
     {
          gRobortSpeed_R =  robort_speed;
          break;
     }  
     default: break;
    }      
}


/*************************************************************************************
*函  数   名：Set_Motor_Direction
*函 数 功 能：第NO号电机方向控制  
*参       数：  motorNo  第NO号电机   dir  机器人行走方向
*返  回   值：无
*使用   等级：二级使用  被 Robort_Direction_Control 调用
*时       间：2018.8.3
*备       注：Z Rui
*************************************************************************************/
static void Set_Motor_Direction(MOTOR_TAG motorNo, DIRECTION_TAG  dir)
{
		//初始状态定义
    static MOTOR_TAG preMotorNo = MOTOR_NONE; //电机静态变量，持续积累变化  首次初始状态————不动用任何一个电机
    static DIRECTION_TAG preDir = MOTOR_NONE; //电机方向静态变量，持续积累方向变化  ，首次初始状态  ——————不转动任意方向
    
    if (preMotorNo==motorNo && preDir==dir)//上一次和这一次的数据相同，则没有动作
    {
        return ;
    }
		
    
    preMotorNo = motorNo;
    preDir = dir;
    
		//控制的电机
    switch (motorNo)
    {
     case MOTOR_LT://左上电机控制
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
     
     case MOTOR_RT: //右上电机控制
        if (dir == DIR_POSITIVE)
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_8, 1);   // DIR2=HIGH
        }
        else
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_8, 0);    // DIR2=LOW
        }
     break;
     case MOTOR_LB://左后电机控制
        if (dir == DIR_POSITIVE)
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_7, 1);   // DIR3=HIGH
        }
        else
        {
             STM32_GPIO_SetPin(GPIOC, GPIO_Pin_7, 0);    // DIR3=LOW
        }
     break;
     case MOTOR_RB://右后电机控制
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
*函  数   名：Set_Motor_Speed
*函 数 功 能：单个电机速度控制   
*参       数：motorNo 第no号电机-> MOTOR_LT,MOTOR_RT,MOTOR_LB,MOTOR_RB,MOTOR_NONE
              speed  电机速度
*返  回   值：DutyCycle  占空比 
*使用   等级：二级使用 被 Robort_Direction_Control 调用
*时       间：2018.8.3
*备       注：Z Rui
*************************************************************************************/
static void Set_Motor_Speed(MOTOR_TAG motorNo, uint8 speed)
{
    static MOTOR_TAG preMotorNo;   //静态变量，首次初始状态——不动用任何一个舵机
    static uint8    preSpeed;				//静态变量，首次初始状态——不转动任意角度
    uint16 ratio = 0;								//占空比
    
    if (preMotorNo==motorNo && preSpeed==speed)  
    {
        return ;										// 如果和上次相比指令没有改变一样，就不作任何动作
    }
    
    preMotorNo = motorNo;						// 提供下一次的 preMotorNo
    preSpeed = speed;								// 提供下一次的 preSpeed
    
    ratio =  CalPWMDutyCycle(speed);// 由要求的速度算出占空比
    
    switch (motorNo)
    {
     case MOTOR_LT:  //左上电机控制  速度
        STM32_TIMER2_PWMControl(TIM2_PWM1, MOTOR_PERIOD_DEFAULT, ratio);
     break;
     case MOTOR_RT:  //右上电机控制  速度
        STM32_TIMER2_PWMControl(TIM2_PWM2, MOTOR_PERIOD_DEFAULT, ratio);
     break;
     case MOTOR_LB://左下电机控制  速度
        STM32_TIMER2_PWMControl(TIM2_PWM3, MOTOR_PERIOD_DEFAULT, ratio);
     break;
     case MOTOR_RB://右后电机控制  速度
        STM32_TIMER2_PWMControl(TIM2_PWM4, MOTOR_PERIOD_DEFAULT, ratio);
     break;  

     default: break;
    }    
    return ;
}






/*************************************************************************************
*函  数   名：CalPWMDutyCycle
*函 数 功 能：：Speed 和 Duty 的比例转换   
*参       数：speed  电机速度
*返  回   值：DutyCycle  占空比
*使用   等级：三级使用 被 Set_Motor_Speed 调用
*时       间：2018.8.3
*备       注：Z Rui
*************************************************************************************/
static uint16  CalPWMDutyCycle(unsigned char speed)
{
	uint16 DutyCycle = 0;

	DutyCycle = (uint16) (speed*1.0 / 100 * 1000);
	return DutyCycle;
}


//电机初始化
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
