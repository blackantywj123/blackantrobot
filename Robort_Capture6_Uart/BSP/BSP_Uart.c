#include "Common.h" 

uchar Task_Uart1_Rx =0;
uchar Tx_Buffer[Tx_Array_Size];			              //���ͻ�����
uchar Rx_Buffer[Rx_Array_Size];			              //���ܻ�����
uchar Rx_Process_Buffer[Rx_Array_Size];	          //���ܴ�������
uchar Tx_Ptr,Rx_Ptr; 	 		               	        // ���ͽ���ָ��
uchar Tx_Tail,Rx_Tail;    			   	              //���ͽ���βָ��
uchar Rx_Step=0;							                      // ���ղ���
uchar Tx_Count,Rx_Count;				                  // ���ͽ��ռ�ʱ
uchar Temp=0;
uchar i=0;
uchar DMA1_Uart_Status=0;

/***********************************************************************************
*��������UART2_Init
*�������ܣ� ����2��ʼ��
*������������
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
void UART2_Init(u32 bound)
	{
		//GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		 
	  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		//USART1_TX   GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
		 
		//USART1_RX	  GPIOA.10��ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

		//Usart1 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
		//USART ��ʼ������
		USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

		USART_Init(USART2, &USART_InitStructure); //��ʼ������1
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
		USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 
}
	
/***********************************************************************************
*��������UART3_Init
*�������ܣ� ����3��ʼ��
*������������
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
void UART3_Init(ulong bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* USART2 GPIO config */
  /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = bound;                //GPSģ��Ĭ��ʹ�ò����ʣ�9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure); 

	USART_Cmd(USART3, ENABLE);
}

/***********************************************************************************
*��������UART1_Init
*�������ܣ� ����1��ʼ��
*������������
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*************************************************************************************/
void UART1_Init( ulong bound)
{

    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	 
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	  USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	 USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	 USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	 USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	 USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

   USART_Init(USART1, &USART_InitStructure); //��ʼ������
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
   USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}

/***********************************************************************************
*��������UART_Init
*�������ܣ� ���ڲ���������
*��������
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*************************************************************************************/

void UART_Init(void)
{
	UART1_Init(115200);
	UART2_Init(500000);
	UART3_Init(38400);//38400
	DMA_Uart1_Init();
}

/***********************************************************************************
*��������USART1_IRQHandler
*�������ܣ� ���ڽ��մ�������֡ ��1��
*��������
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
************************************************************************************/

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Rx_Buffer[Rx_Ptr] =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		if(Rx_Ptr>(Rx_Array_Size-1)) Rx_Ptr=0;//size=50,
		Rx_Ptr++;
		switch (Rx_Step)										//��ʼ��־
		{
			case 0x00:											  //У��֡ͷ0xff
				Rx_Count=0;
				if(Rx_Buffer[0]==0xFF) 
					{
					Rx_Step++; 
					}
   				else Rx_Ptr=0;
				break;
			case 0x01:											//����ģ��ţ�0x01
				Rx_Count=0;
				if(Rx_Buffer[1]==0x01)
					{
					Rx_Step++;
					}	
				else{           
						Rx_Ptr=0;
						Rx_Step=0;
						}
				break ;
        case 0x02:											//����ʶ��
			  Rx_Count=0;
				if(Rx_Ptr==4)
				{
					Temp=Rx_Buffer[3];
					Rx_Tail= ((u16)Rx_Buffer[2] | Temp<<8) +7;//�˴ε����ݳ���(������CRC��Щ)	
			    if (Rx_Tail>(Rx_Array_Size-1))			// ������Ч 
					  {
						Rx_Ptr=0;
						Rx_Step=0;
						}
					else Rx_Step++;
				}
				break;
			  case 0x03:											        //��ȡ����
				Rx_Count=0;
				if(Rx_Ptr==Rx_Tail)
				{
					for ( i=0;i<Rx_Tail;i++){
						Rx_Process_Buffer[i]=Rx_Buffer[i]; 
						}
					Rx_Ptr=0;
					Rx_Tail=0;
					Rx_Step=0;
					Task_Uart1_Rx=1;
				}
				break;
			default:;  											// 
			}
	}
		
}

/*********************************************************************
***** ����2
***** 
*********************************************************************/
#define Tx_Array_Size 			 	 50
#define Rx_Array_Size 			 	 50
u8 Task_Uart2_Rx =0;														//���ݸ��³ɹ���־λ��û��clear�����ⲿclear��
u8 Tx_Buffer2[Tx_Array_Size];			              //���ͻ�����
u8 Rx_Buffer2[Rx_Array_Size];			              //����ʵʱ������
u8 Rx_Process_Buffer2[Rx_Array_Size];	          //���ճɹ�������
u8 Tx_Ptr2,Rx_Ptr2; 	 		               	      // ��־���ڽ��յڼ�λ����
u8 Tx_Tail2,Rx_Tail2;//6 + 6 + 1 = 6 + 7   // ���ݳ���
u8 Rx_Step2=0;							                    // У�鲽��
u8 Tx_Count2,Rx_Count2;				                  // ���ͽ��ռ�ʱ
u8 Temp2=0;
u8 i2=0;
/***********************************************************************************
*��������USART2_IRQHandler
*�������ܣ� ����2���մ�������֡ ��2��
*��������
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*************************************************************************************/
void USART2_IRQHandler(void)                	//����2�ж�
{
	Rx_Tail2 = 13;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  // �����ж�
	{
		Rx_Buffer2[Rx_Ptr2] =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���ݵ�Rx_Buffer2
		if(Rx_Ptr2>(Rx_Array_Size-1)) Rx_Ptr2=0;	 // ��ֹ�������
		Rx_Ptr2++;																 // ��־��һλ���յ�λ��
		switch (Rx_Step2)										       // ��ʼ��־λ�ļ��FE 0A 80 81 01 00 _ _ _ _ - - FF
		{
			case 0x00:											        
				Rx_Count2=0;
				if(Rx_Buffer2[0]==0xFE) 
					{
					Rx_Step2++; 
					}
   				else Rx_Ptr2=0;
				break;
			case 0x01:											        
				Rx_Count2=0;
				if(Rx_Buffer2[1]==0x0A)
					{
					Rx_Step2++;
					}	
				else {           
						Rx_Ptr2=0;
						Rx_Step2=0;
						}
				break;
			case 0x02: 
				Rx_Count2=0;
				if(Rx_Buffer2[2]==0x80)
					{
					Rx_Step2++;
					}	
				else {           
						Rx_Ptr2=0;
						Rx_Step2=0;
						}
				break;
			case 0x03:
				Rx_Count2=0;
				if(Rx_Buffer2[3]==0x81)
					{
					Rx_Step2++;
					}	
				else {           
						Rx_Ptr2=0;
						Rx_Step2=0;
						}
				break;
			case 0x04: 
				Rx_Count2=0;
				if(Rx_Buffer2[4]==0x01)
					{
					Rx_Step2++;
					}	
				else {           
						Rx_Ptr2=0;
						Rx_Step2=0;
						}
				break;
			case 0x05: 
				Rx_Count2=0;
				if(Rx_Buffer2[5]==0x00)
					{
					Rx_Step2++;
					}	
				else {           
						Rx_Ptr2=0;
						Rx_Step2=0;
						}
				break;
			case 0x06:											        
				Rx_Count2=0;
			if(Rx_Ptr2==Rx_Tail2)
				{
					for ( i2=0;i2<Rx_Tail2;i2++){
						Rx_Process_Buffer2[i2]=Rx_Buffer2[i2]; 
						}
					Rx_Ptr2=0;
					Rx_Tail2=0;
					Rx_Step2=0;
					Task_Uart2_Rx=1;
				}
				break;
			default:;  											
			}
	}		
}

/***********************************************************************************
*��������crc16
*�������ܣ� ѭ������У�飬��������֡�Ƿ����
*������ptr��count
*����ֵ��crc&0xFFFF
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/

int Crc16(uchar *ptr, int count)
{
	int crc, i;
	crc = 0;
	while (--count >= 0){
		crc = (crc ^ (((int)*ptr++) << 8));
		for (i = 0; i < 8; ++i)
		if (crc & 0x8000)
		crc = ((crc << 1) ^ 0x1021);
		else
		crc = crc << 1;
	}
	return (crc & 0xFFFF);
}

/***********************************************************************************
*��������STM32_UART_Write
*�������ܣ� �������յ�DR�Ĵ�����
*������uartNo   *src   len �����ں� srcָ�� ���ݳ��ȣ�
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
void STM32_UART_Write(u8 uartNo, void *src, u16 len)
{
	u8 t=0;
	if( 1 ==uartNo){
		for(t=0;t<len;t++)
			{
				USART1->DR =*((u8*)src+t);
				while((USART1->SR&0X40)==0);//��ѯ�����Ƿ����
			}
		}
	else if( 2==uartNo){
		for(t=0;t<len;t++)
			{
				USART2->DR =*((u8*)src+t);
				while((USART2->SR&0X40)==0);//��ѯ�����Ƿ����
			}
		}
}
/***********************************************************************************
*��������fputc
*�������ܣ� �ض���c�⺯��printf��USART1
*������ch   *f(һ�ֽ����� ָ��)
*����ֵ��ch��һ�ֽ����ݣ�
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

/***********************************************************************************
*��������fgetc
*�������ܣ� �ض���c�⺯��scanf��USART1
*������ *f(ָ��)
*����ֵ������һ�ӵ������ݣ�int��
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
/***********************************************************************************
*��������STM32_UART_Read
*�������ܣ� ���ݼ��
*������ uartNo dst len(���ں� dstָ�� ���ݳ���)
*����ֵ��0 or 1
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
 u8 STM32_UART_Read(u8 uartNo, void *dst, u16 len)
 {
	 u8 i=0;
	 u16 dat_len = 0;
	 u16 crc_code = 0;
	 
	if( Task_Uart1_Rx)
	{
		dat_len = ((u16)Rx_Process_Buffer[2]<<8 | Rx_Process_Buffer[3])+7;
		
		crc_code=Crc16(&Rx_Process_Buffer[0],dat_len-2); //CRC code calculation
		
		//CRC check
		if ((crc_code&0xff == Rx_Process_Buffer[dat_len-1]) && ((crc_code>>8)&0xff == Rx_Process_Buffer[dat_len-2]))
		{
			//assert(0);
			return 0;
		}		
		
		for(i=0;i<len;i++ )
			*((u8*)dst+i) =Rx_Process_Buffer[i];
		
		Task_Uart1_Rx=0;
		return 1;
	}	
	return 0;
 }
 
 
 
 /***********************************************************************************
*��������DMA_Uart1_Init
*�������ܣ� ����һDMA��ʼ��
*������ ��
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
 void DMA_Uart1_Init( void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	DMA_DeInit(DMA1_Channel4);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Tx_Buffer;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = 10;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	// DMA2 Channel Interrupt ENABLE
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�

	/*ʹ��DMA*/
	DMA_Cmd (DMA1_Channel4,ENABLE);		
    
  /* ���ô��� �� DMA����TX���� */
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

/***********************************************************************************
*��������DMA1_DATA_Trans_Task_ON()
*�������ܣ� ����һDMA״̬
*������ ��
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
void DMA1_DATA_Trans_Task_ON()
{
	DMA1_Uart_Status=1;
}
/***********************************************************************************
*��������DMA1_DATA_Trans_Task_OFF
*�������ܣ�  DMA1����
*������ ��
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
void DMA1_DATA_Trans_Task_OFF()
{
	DMA1_Uart_Status=0;
}
u8 DMA1_Uart1_Status_Get( void)
{
	return DMA1_Uart_Status;
}
/***********************************************************************************
*��������STM32_UART1_Write
*�������ܣ� ����һDMA��д����
*������ ��
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
void STM32_UART1_Write(u8* Tx_Buff,u8 Data_Len)
{
	DMA1_Channel4->CCR&=~(1<<0);
	DMA1_Channel4->CMAR=(u32)Tx_Buff;
	DMA1_Channel4->CNDTR=Data_Len;
	DMA1_Channel4->CCR|=(1<<0);
	DMA1_DATA_Trans_Task_ON();
}
/***********************************************************************************
*��������Upload_ProcessDMAIRQ
*�������ܣ� ����ֱ�Ӵ���
*������ ��
*����ֵ����
*ʱ�䣺2018.8.13
*��ע��wenjie.Y
*********************************************************************/
void Upload_ProcessDMAIRQ( void)
{
	if( DMA_GetITStatus(DMA1_IT_TC4))
	{
		DMA1_DATA_Trans_Task_OFF();
		DMA_ClearFlag(DMA1_FLAG_TC4);//���ͨ��4������ɱ�־
	}
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

/*********************************************END OF FILE**********************/
