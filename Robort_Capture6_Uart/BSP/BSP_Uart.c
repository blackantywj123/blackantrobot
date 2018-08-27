#include "Common.h" 

uchar Task_Uart1_Rx =0;
uchar Tx_Buffer[Tx_Array_Size];			              //发送缓存区
uchar Rx_Buffer[Rx_Array_Size];			              //接受缓存区
uchar Rx_Process_Buffer[Rx_Array_Size];	          //接受处理缓存区
uchar Tx_Ptr,Rx_Ptr; 	 		               	        // 发送接收指针
uchar Tx_Tail,Rx_Tail;    			   	              //发送接收尾指针
uchar Rx_Step=0;							                      // 接收步骤
uchar Tx_Count,Rx_Count;				                  // 发送接收计时
uchar Temp=0;
uchar i=0;
uchar DMA1_Uart_Status=0;

/***********************************************************************************
*函数名：UART2_Init
*函数功能： 串口2初始化
*参数：波特率
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
*********************************************************************/
void UART2_Init(u32 bound)
	{
		//GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		 
	  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		//USART1_TX   GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
		 
		//USART1_RX	  GPIOA.10初始化
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

		//Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
		
		//USART 初始化设置
		USART_InitStructure.USART_BaudRate = bound;//串口波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		USART_Init(USART2, &USART_InitStructure); //初始化串口1
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_Cmd(USART2, ENABLE);                    //使能串口1 
}
	
/***********************************************************************************
*函数名：UART3_Init
*函数功能： 串口3初始化
*参数：波特率
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
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
	USART_InitStructure.USART_BaudRate = bound;                //GPS模块默认使用波特率：9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure); 

	USART_Cmd(USART3, ENABLE);
}

/***********************************************************************************
*函数名：UART1_Init
*函数功能： 串口1初始化
*参数：波特率
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
*************************************************************************************/
void UART1_Init( ulong bound)
{

    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	 
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	  USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	 USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	 USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	 USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	 USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

   USART_Init(USART1, &USART_InitStructure); //初始化串口
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
   USART_Cmd(USART1, ENABLE);                    //使能串口 
}

/***********************************************************************************
*函数名：UART_Init
*函数功能： 串口波特率输入
*参数：无
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
*************************************************************************************/

void UART_Init(void)
{
	UART1_Init(115200);
	UART2_Init(500000);
	UART3_Init(38400);//38400
	DMA_Uart1_Init();
}

/***********************************************************************************
*函数名：USART1_IRQHandler
*函数功能： 串口接收处理数据帧 （1）
*参数：无
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
************************************************************************************/

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		Rx_Buffer[Rx_Ptr] =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		if(Rx_Ptr>(Rx_Array_Size-1)) Rx_Ptr=0;//size=50,
		Rx_Ptr++;
		switch (Rx_Step)										//开始标志
		{
			case 0x00:											  //校验帧头0xff
				Rx_Count=0;
				if(Rx_Buffer[0]==0xFF) 
					{
					Rx_Step++; 
					}
   				else Rx_Ptr=0;
				break;
			case 0x01:											//本机模块号：0x01
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
        case 0x02:											//长度识别
			  Rx_Count=0;
				if(Rx_Ptr==4)
				{
					Temp=Rx_Buffer[3];
					Rx_Tail= ((u16)Rx_Buffer[2] | Temp<<8) +7;//此次的数据长度(包括了CRC这些)	
			    if (Rx_Tail>(Rx_Array_Size-1))			// 长度无效 
					  {
						Rx_Ptr=0;
						Rx_Step=0;
						}
					else Rx_Step++;
				}
				break;
			  case 0x03:											        //提取数据
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
***** 串口2
***** 
*********************************************************************/
#define Tx_Array_Size 			 	 50
#define Rx_Array_Size 			 	 50
u8 Task_Uart2_Rx =0;														//数据更新成功标志位（没有clear，需外部clear）
u8 Tx_Buffer2[Tx_Array_Size];			              //发送缓存区
u8 Rx_Buffer2[Rx_Array_Size];			              //接收实时缓存区
u8 Rx_Process_Buffer2[Rx_Array_Size];	          //接收成功缓存区
u8 Tx_Ptr2,Rx_Ptr2; 	 		               	      // 标志正在接收第几位数据
u8 Tx_Tail2,Rx_Tail2;//6 + 6 + 1 = 6 + 7   // 数据长度
u8 Rx_Step2=0;							                    // 校验步骤
u8 Tx_Count2,Rx_Count2;				                  // 发送接收计时
u8 Temp2=0;
u8 i2=0;
/***********************************************************************************
*函数名：USART2_IRQHandler
*函数功能： 串口2接收处理数据帧 （2）
*参数：无
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
*************************************************************************************/
void USART2_IRQHandler(void)                	//串口2中断
{
	Rx_Tail2 = 13;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  // 接收中断
	{
		Rx_Buffer2[Rx_Ptr2] =USART_ReceiveData(USART2);//(USART1->DR);	//读取数据到Rx_Buffer2
		if(Rx_Ptr2>(Rx_Array_Size-1)) Rx_Ptr2=0;	 // 防止溢出数组
		Rx_Ptr2++;																 // 标志下一位接收的位置
		switch (Rx_Step2)										       // 开始标志位的检测FE 0A 80 81 01 00 _ _ _ _ - - FF
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
*函数名：crc16
*函数功能： 循环冗余校验，检验数据帧是否出错
*参数：ptr，count
*返回值：crc&0xFFFF
*时间：2018.8.13
*备注：wenjie.Y
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
*函数名：STM32_UART_Write
*函数功能： 把数据收到DR寄存器里
*参数：uartNo   *src   len （串口号 src指针 数据长度）
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
*********************************************************************/
void STM32_UART_Write(u8 uartNo, void *src, u16 len)
{
	u8 t=0;
	if( 1 ==uartNo){
		for(t=0;t<len;t++)
			{
				USART1->DR =*((u8*)src+t);
				while((USART1->SR&0X40)==0);//查询发送是否结束
			}
		}
	else if( 2==uartNo){
		for(t=0;t<len;t++)
			{
				USART2->DR =*((u8*)src+t);
				while((USART2->SR&0X40)==0);//查询发送是否结束
			}
		}
}
/***********************************************************************************
*函数名：fputc
*函数功能： 重定向c库函数printf到USART1
*参数：ch   *f(一字节数据 指针)
*返回值：ch（一字节数据）
*时间：2018.8.13
*备注：wenjie.Y
*********************************************************************/
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

/***********************************************************************************
*函数名：fgetc
*函数功能： 重定向c库函数scanf到USART1
*参数： *f(指针)
*返回值：串口一接到的数据（int）
*时间：2018.8.13
*备注：wenjie.Y
*********************************************************************/
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
/***********************************************************************************
*函数名：STM32_UART_Read
*函数功能： 数据检测
*参数： uartNo dst len(串口号 dst指针 数据长度)
*返回值：0 or 1
*时间：2018.8.13
*备注：wenjie.Y
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
*函数名：DMA_Uart1_Init
*函数功能： 串口一DMA初始化
*参数： 无
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
*********************************************************************/
 void DMA_Uart1_Init( void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	DMA_DeInit(DMA1_Channel4);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Tx_Buffer;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = 10;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	// DMA2 Channel Interrupt ENABLE
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断

	/*使能DMA*/
	DMA_Cmd (DMA1_Channel4,ENABLE);		
    
  /* 配置串口 向 DMA发出TX请求 */
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

/***********************************************************************************
*函数名：DMA1_DATA_Trans_Task_ON()
*函数功能： 串口一DMA状态
*参数： 无
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
*********************************************************************/
void DMA1_DATA_Trans_Task_ON()
{
	DMA1_Uart_Status=1;
}
/***********************************************************************************
*函数名：DMA1_DATA_Trans_Task_OFF
*函数功能：  DMA1开关
*参数： 无
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
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
*函数名：STM32_UART1_Write
*函数功能： 串口一DMA读写命令
*参数： 无
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
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
*函数名：Upload_ProcessDMAIRQ
*函数功能： 数据直接传送
*参数： 无
*返回值：无
*时间：2018.8.13
*备注：wenjie.Y
*********************************************************************/
void Upload_ProcessDMAIRQ( void)
{
	if( DMA_GetITStatus(DMA1_IT_TC4))
	{
		DMA1_DATA_Trans_Task_OFF();
		DMA_ClearFlag(DMA1_FLAG_TC4);//清除通道4传输完成标志
	}
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

/*********************************************END OF FILE**********************/
