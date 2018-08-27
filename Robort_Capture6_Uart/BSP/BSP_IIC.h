#ifndef __IIC_H
#define __IIC_H
#include "Common.h"
#include "stm32f10x.h"
#include "sys.h"

#define IIC_LINE      GPIOC  // ------------------------
#define IIC_RCC_LINE  RCC_APB2Periph_GPIOC  // ---------
#define SCL           GPIO_Pin_4  // -------------------
#define SDA           GPIO_Pin_5  // -------------------
#define SCL_num       4           // -------------------
#define SDA_num       5           // -------------------
//#define SDA_IN()  {IIC_LINE->CRL &= 0XFF0FFFFF;IIC_LINE->CRL|=(u32)8<< 20;}//0��λ��,4*x
//#define SDA_OUT() {IIC_LINE->CRL &= 0XFF0FFFFF;IIC_LINE->CRL|=(u32)3<< 20;}
#define SDA_IN()  {IIC_LINE->CRL &= ~(0x0000000F<<(4*SDA_num));IIC_LINE->CRL|=(u32)8<<(4*SDA_num);}
#define SDA_OUT() {IIC_LINE->CRL &= ~(0x0000000F<<(4*SDA_num));IIC_LINE->CRL|=(u32)3<<(4*SDA_num);}


//IO��������	 
#define IIC_SCL    PCout(SCL_num) // SCL PA4 -----------------
#define IIC_SDA    PCout(SDA_num) // SDA	PA5 ----------------
#define READ_SDA   PCin(SDA_num)  // ����SDA ----------------- 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	

#endif
