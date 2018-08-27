#ifndef _APP_MESSAGE_H
#define _APP_MESSAGE_H

#include "stm32f10x.h"
#include "BSP_Uart.h"

#define uint8 unsigned char
#define MB_SIZE 20


typedef enum
{
    RET_OK=0,
    RET_ERROR,
}RET_TAG;

/* The definition of protocol */
typedef struct
{
	uint8 Frame_Head;       //0xff
	uint8 Addr;             //robort local address
	uint8 Msg_L;            //Data[] length LSB
	uint8 Msg_H;            //Data[] length MSB
	uint8 Cmd;              //Command type
	uint8 Data[MB_SIZE];    //msg buffer size
	uint8 Crc_L;            //crc check LSB
	uint8 Crc_H;            //crc check MSB
}App_Robort_Dat_Tag;			// 对应下行协议的顺序


void GPS_Message_Upload( void);
void Robort_Send_Frame(App_Robort_Dat_Tag *pdata) ;
int Robort_Receive_Frame(App_Robort_Dat_Tag *pdata);
void Robort_Uart_Init(void);

#endif
