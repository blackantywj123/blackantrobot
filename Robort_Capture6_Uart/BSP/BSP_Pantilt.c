#include "Common.h" 

uchar Horizontal_Angle=0; //��ǰ��̨ˮƽ�Ƕ�
uchar Vertical_Angle=0;   //��ǰ��̨��ֱ�Ƕ�

void H_ACT_Ctrl(uchar Angle)
{
	
	TIM_SetCompare1(TIM1, Angle);
}






















/*********************************************END OF FILE**********************/
