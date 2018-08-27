#include "Common.h" 

uchar Horizontal_Angle=0; //当前云台水平角度
uchar Vertical_Angle=0;   //当前云台垂直角度

void H_ACT_Ctrl(uchar Angle)
{
	
	TIM_SetCompare1(TIM1, Angle);
}






















/*********************************************END OF FILE**********************/
