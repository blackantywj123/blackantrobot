/**********************************************************************************************************
**** FileName：App_Protocol.c
**** 
**** FileDescription：Terminal & robort protocol parse
****		
**** ProtocolDescription:
****
****  ------------------------------------------------------------------------------------------------------------------     
****  |    Name            Header      SlaveAddr    MsgLSB(1B)    MsgMSB(1B)      Cmd      Data     CrcLSB     CrcMSB  |
****  |    STOP            0xff        0x01                                       0x00                                 |
****  |    FORWARD         0xff        0x01                                       0x01                                 |
****  |    BACK            0xff        0x01                                       0x02                                 |
****  |    LEFT            0xff        0x01                                       0x03                                 |
****  |    RIGHT           0xff        0x01                                       0x04                                 |
****  |    STEER1~STEER8   0xff        0x01                                       0x05                                 |
****  |    SPEED_L         0xff        0x01                                       0x06                                 |
****  |    SPEED_R         0xff        0x01                                       0x07                                 |
****  |    LIGHT_ON        0xff        0x01                                       0x08                                 |
****  |    LIGHT_OFF       0xff        0x01                                       0x09                                 |
****  |    ENERGY          0xff        0x02                                       0x0a                                 |
****  |    HUMIDITY        0xff        0x02                                       0x0b                                 |
****  |    TEMPERATURE     0xff        0x02                                       0x0c                                 |
****  |    POSITION        0xff        0x02                                       0x0d                                 |
****  |                                                                                                                |
****  ------------------------------------------------------------------------------------------------------------------
****
**** Author：Sandy
**** 
**** Version：1.0
**** 
**** Revision：
**** 
**** 
***********************************************************************************************************/

#include "App_Protocol.h"




/* Get msg length */ 
static unsigned short Assemble_Message_Length(App_Robort_Dat_Tag *pdata)
{
	unsigned short len;
	len = (pdata->Msg_H << 8) | (pdata->Msg_L);
	
	return len;
}

/* Compare local addr & slave addr */ 
static uint8 Robort_Adress_Veritfy(App_Robort_Dat_Tag *pdata)
{
	return (pdata->Addr == ROBORT_LOCAL_ADDRESS) ;
}
/**************************************************************
*****
*****电机控制
*****
*****
**************************************************************/
/* Dirction(stop, forward, back, left, right) cmd process */ 
uint8 Process_Robort_Direction_Cmd(App_Robort_Dat_Tag *pdata)
{
	unsigned short msg_len = 0;
    uint8 EN = 0;
		uint8 ctl_l = 1;
		uint8 ctl_r = 1;
    uint8 speed_l = 0;
		uint8 speed_r = 0;
	if (pdata == NULL)
		return 0;
	
	msg_len = Assemble_Message_Length(pdata);
	if (msg_len == 4)
	{
		EN = ROBORT_SPEED_VARIBLE; //ENABLE
		ctl_l   = pdata->Data[0];
		ctl_r   = pdata->Data[1];
		speed_l = pdata->Data[2];  // speed_left data	
		speed_r = pdata->Data[3];  // speed_right data			
	}
	else
	{
		EN = ROBORT_SPEED_DEFAULT;
	}
//	if (pdata->Cmd == APP_ROBORT_CMD_STOP)
//	{
	Robort_Direction_Control(EN, ctl_l, ctl_r, speed_l , speed_r);
//	Robort_Direction_Control(EN, ctl_l, ctl_r, speed_l, speed_r);
//	}
//	else if (pdata->Cmd == APP_ROBORT_CMD_FORWARD)
//	{
//		Robort_Direction_Control(speed, speed_l, speed_r);
//	}
//	else if (pdata->Cmd == APP_ROBORT_CMD_BACK)
//	{
//		Robort_Direction_Control(ROBORT_BACK, speed, speed_l, speed_r);
//	}
//	else if (pdata->Cmd == APP_ROBORT_CMD_LEFT)
//	{
//		Robort_Direction_Control(ROBORT_LEFT, speed, speed_l, speed_r);
//	}
//	else if (pdata->Cmd == APP_ROBORT_CMD_RIGHT)
//	{
//		Robort_Direction_Control(ROBORT_RIGHT, speed, speed_l, speed_r);
//	}
	
	
	return 1;
}



/* Steer turned  cmd process */ 
static uint8 Process_Robort_Steer_Cmd(App_Robort_Dat_Tag *pdata)
{
	unsigned short msg_len = 0;
	uint8 angle = 0;
	
	if (pdata == NULL)
		return 0;
	
	msg_len = Assemble_Message_Length(pdata);
	if (msg_len == 1)
	{
		 angle = pdata->Data[0]; //angle data
	}
	else
	{
		 angle = ANGLE_DEFAULT;
	}
	
	if (pdata->Cmd == APP_ROBORT_CMD_STEER1)
	{
		Robort_Steer_Control(ROBORT_STEER1, angle);  //for cloud stage steer1
	}
	else if (pdata->Cmd == APP_ROBORT_CMD_STEER2)
	{
		Robort_Steer_Control(ROBORT_STEER2, angle);  //for cloud stage steer2
	}
	else if (pdata->Cmd == APP_ROBORT_CMD_STEER3)
	{
		Robort_Steer_Control(ROBORT_STEER3, angle);
	}
	else if (pdata->Cmd == APP_ROBORT_CMD_STEER4)
	{
		Robort_Steer_Control(ROBORT_STEER4, angle);
	}
	else if (pdata->Cmd == APP_ROBORT_CMD_STEER5)
	{
		Robort_Steer_Control(ROBORT_STEER5, angle);
	}
	else if (pdata->Cmd == APP_ROBORT_CMD_STEER6)
	{
		Robort_Steer_Control(ROBORT_STEER6, angle);
	}
	else if (pdata->Cmd == APP_ROBORT_CMD_STEER7)
	{
		Robort_Steer_Control(ROBORT_STEER7, angle);
	}
	else if (pdata->Cmd == APP_ROBORT_CMD_STEER8)
	{
		Robort_Steer_Control(ROBORT_STEER8, angle);
	}
	
	return 1;
}

/* Robort speed(left speed or right speed) cmd process */ 
static uint8 Process_Robort_Speed_Cmd(App_Robort_Dat_Tag *pdata)
{
	uint8 speed = 0;
	unsigned short msg_len = 0;
	
	if (pdata == NULL)
	return 0;

	msg_len = Assemble_Message_Length(pdata);// 报文 01 00
	if (msg_len == 1)													 //报文验证正确
	{
		speed = pdata->Data[0]; //speed data 
		speed = 70;
	}

	if (speed>100)
	{
			return 0;
	}
    
	if (pdata->Cmd == APP_ROBORT_CMD_SPEED_L)	//下行指令是left
	{
		Robort_Speed_Control(ROBORT_SPEED_L, speed);
	}
	else if (pdata->Cmd == APP_ROBORT_CMD_SPEED_R)//下行指令是right
	{
		Robort_Speed_Control(ROBORT_SPEED_R, speed);
	}
    
    return 1;
}

 /* Light(on or off) cmd process */ 
static uint8 Process_Robort_Light_Cmd(App_Robort_Dat_Tag *pdata)
{
    uint8 lightNum = 0;
    unsigned short msg_len = 0;   

    
	if (pdata == NULL)
		return 0;
      
    msg_len = Assemble_Message_Length(pdata);
	if (msg_len == 1)
	{
		 lightNum = pdata->Data[0]; //angle data
	}
    else
    {
        return 0;
    }

    
    if (lightNum ==  ROBORT_LIGHT_ONE)
    {
        if (pdata->Cmd == APP_ROBORT_CMD_LIGHT_ON)
        {
            Robort_Light_Control(ROBORT_LIGHT_ONE, ROBORT_LIGHT_ON);
        }
        else
        {
            Robort_Light_Control(ROBORT_LIGHT_ONE, ROBORT_LIGHT_OFF);
        }
    }
	else if (lightNum ==  ROBORT_LIGHT_TWO)
    {
        if (pdata->Cmd == APP_ROBORT_CMD_LIGHT_ON)
        {
            Robort_Light_Control(ROBORT_LIGHT_TWO, ROBORT_LIGHT_ON);
        }
        else
        {
            Robort_Light_Control(ROBORT_LIGHT_TWO, ROBORT_LIGHT_OFF);
        }
    }
    
    
	return 1;
}

/* Beep(on or off) cmd process */ 
static uint8 Process_Robort_Beep_Cmd(App_Robort_Dat_Tag *pdata)
{
	if (pdata == NULL)
		return 0;
	
	if (pdata->Cmd == APP_ROBORT_CMD_BEEP_ON)
	{
		Robort_Beep_Control(ROBORT_BEEP_ON);
	}
    else
    {
        Robort_Beep_Control(ROBORT_BEEP_OFF);
    }
	
	return 1;
}


/* sensor(distance, temperature, humidity, energy, position...) cmd process */ 
static uint8 Process_Robort_Sensor_Cmd(App_Robort_Dat_Tag *pdata)
{
    uSensorInfoTag SensorEntry = {0};
    App_Robort_Dat_Tag send_protocol = {0};
    unsigned short crc = 0;
    uint8 msg_len = 0;
    APP_SENSOR_TYPE_TAG fSensorType; 

	if (pdata == NULL)
		return 0;
      

    msg_len = Assemble_Message_Length(pdata);
	if (msg_len == 1)
	{
		 fSensorType = pdata->Data[0]; //angle data
	}
    else
    {
        return 0;
    }

    if (fSensorType ==  APP_DISTANCE_TYPE)
    {
        Robort_Get_Sensor_Data(DISTANCE_TYPE, &SensorEntry);
        send_protocol.Cmd = ROBORT_APP_CMD_DISTANCE;
    }
    else if (fSensorType == APP_TEMPERATURE_TYPE)
    {
        Robort_Get_Sensor_Data(TEMPERATURE_TYPE, &SensorEntry);
        send_protocol.Cmd = ROBORT_APP_CMD_TEMPERATURE;
    }
    else if (fSensorType == APP_HUMIDITY_TYPE)
    {
        Robort_Get_Sensor_Data(HUMIDITY_TYPE, &SensorEntry);
        send_protocol.Cmd = ROBORT_APP_CMD_HUMIDITY;
    }
    else if (fSensorType ==  APP_POSITION_TYPE)
    {
        Robort_Get_Sensor_Data(POSITION_TYPE, &SensorEntry);
        send_protocol.Cmd = ROBORT_APP_CMD_POSITION;
    }
    else if (fSensorType ==  APP_ENERGY_TYPE)
    {
        Robort_Get_Sensor_Data(POSITION_TYPE, &SensorEntry);
        send_protocol.Cmd = ROBORT_APP_CMD_ENERGY;
    }
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    send_protocol.Frame_Head = 0xff;
    send_protocol.Addr = APP_TERMINAL_ADDRESS;
    send_protocol.Msg_L = SensorEntry.Data_Length % 256;
    send_protocol.Msg_H = SensorEntry.Data_Length / 256;
    send_protocol.Cmd   = pdata->Cmd;
    
    memcpy(send_protocol.Data,  &SensorEntry.sensor, SensorEntry.Data_Length); //
       
    crc = Crc16((uint8 *)&send_protocol, sizeof(send_protocol)-2); //exclude Crc_L & Crc_H
    
    send_protocol.Crc_L = crc & 0xff;
    send_protocol.Crc_H = (crc>>8) & 0xff;
    
    Robort_Send_Frame((void *)&send_protocol);

    return 1;
}

static unsigned short Crc16(uint8 *ptr, int count)
{
	int crc, i;
	crc = 0;
	while (--count >= 0)
    {
		crc = (crc ^ (((int)*ptr++) << 8));
		for (i = 0; i < 8; ++i)
        {
            if (crc & 0x8000)
                crc = ((crc << 1) ^ 0x1021);
            else
                crc = crc << 1;
        }
	}
    
	return (crc & 0xFFFF);
}


/*************************************************************************************************
*****
***** function：uint8  Protocol_Parser()
*****
***** input：pdata -- pointer to protocol command received
*****        
*****
***** output: none
*****
***** description：parse the protocol based on APP_ROBORT_CMD_TYPE
*****
*****
***** call：Process_Robort_Direction_Cmd() -- parse robort's direction cmd 
*****       Process_Robort_Steer_Cmd() -- parse robort's steer cmd
*****       Process_Robort_Light_Cmd() -- parse robort's light cmd
*****       Process_Robort_Beep_Cmd()  --  parse robort's beep cmd
*****       Process_Robort_Sensor_Cmd() -- parse robort's sensor cmd
*****       
*****   
*****
***** return:   0 -- error
*****           1 -- ok
*****
***** author：Sandy
*****				
**************************************************************************************************/
uint8 Protocol_Parser(App_Robort_Dat_Tag *pdata)
{
	if (0 == Robort_Adress_Veritfy(pdata))
	{
		return 0;
	}
	
	switch (pdata->Cmd)
	{
		case APP_ROBORT_CMD_STOP:
		case APP_ROBORT_CMD_FORWARD:
		case APP_ROBORT_CMD_BACK:
		case APP_ROBORT_CMD_LEFT:
		case APP_ROBORT_CMD_RIGHT:
			Process_Robort_Direction_Cmd(pdata);
			break;
		case APP_ROBORT_CMD_STEER1:
		case APP_ROBORT_CMD_STEER2:
		case APP_ROBORT_CMD_STEER3:
		case APP_ROBORT_CMD_STEER4:
		case APP_ROBORT_CMD_STEER5:
		case APP_ROBORT_CMD_STEER6:
		case APP_ROBORT_CMD_STEER7:
		case APP_ROBORT_CMD_STEER8:
			Process_Robort_Steer_Cmd(pdata);
			break;
		case APP_ROBORT_CMD_SPEED_L:// ---- speed
		case APP_ROBORT_CMD_SPEED_R:
			Process_Robort_Speed_Cmd(pdata);
			break;
		case APP_ROBORT_CMD_LIGHT_ON:
		case APP_ROBORT_CMD_LIGHT_OFF:
			Process_Robort_Light_Cmd(pdata);
			break;
        case APP_ROBORT_CMD_BEEP_ON:
		case APP_ROBORT_CMD_BEEP_OFF:
			Process_Robort_Beep_Cmd(pdata);
			break;
		case APP_ROBORT_CMD_SENSOR:
			Process_Robort_Sensor_Cmd(pdata);
			break;
		default: return 0;
	}
	
	return 1;
}

void Steer_Test( void)
{
	short angle1=70,angle2=90;
	Robort_Steer_Control(ROBORT_STEER3, angle1);
	Robort_Steer_Control(ROBORT_STEER4, angle2);
	Robort_Steer_Control(ROBORT_STEER5, angle2);
}


