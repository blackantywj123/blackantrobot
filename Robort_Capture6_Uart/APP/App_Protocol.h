#ifndef _APP_PROTOCOL_H
#define _APP_PROTOCOL_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "App_Motor.h"
#include "App_Steer.h"
#include "App_Light.h"
#include "App_Beep.h"
#include "App_Message.h"
#include "App_Sensor.h"

#include "stm32f10x.h"



#define ROBORT_LOCAL_ADDRESS 0X01
#define APP_TERMINAL_ADDRESS 0X02




typedef enum
{
	APP_ROBORT_CMD_STOP   = 0, //direction
	APP_ROBORT_CMD_FORWARD,
	APP_ROBORT_CMD_BACK,
	APP_ROBORT_CMD_LEFT,
	APP_ROBORT_CMD_RIGHT,
	APP_ROBORT_CMD_STEER1, //steer engine
	APP_ROBORT_CMD_STEER2, //steer engine
	APP_ROBORT_CMD_STEER3, //steer engine
	APP_ROBORT_CMD_STEER4, //steer engine
	APP_ROBORT_CMD_STEER5, //steer engine
	APP_ROBORT_CMD_STEER6, //steer engine
	APP_ROBORT_CMD_STEER7, //steer engine
    APP_ROBORT_CMD_STEER8, //steer engine
	APP_ROBORT_CMD_SPEED_L,  //speed---------13---0d
	APP_ROBORT_CMD_SPEED_R,  //speed
	APP_ROBORT_CMD_LIGHT_ON, //light 
	APP_ROBORT_CMD_LIGHT_OFF,
    APP_ROBORT_CMD_BEEP_ON,  //beep
    APP_ROBORT_CMD_BEEP_OFF,
    APP_ROBORT_CMD_SENSOR,
	APP_ROBORT_END
}APP_ROBORT_CMD_TYPE;


typedef enum
{
    ROBORT_APP_CMD_ENERGY = 0,
    ROBORT_APP_CMD_HUMIDITY,
    ROBORT_APP_CMD_TEMPERATURE,
    ROBORT_APP_CMD_POSITION,
    ROBORT_APP_CMD_DISTANCE,
    ROBORT_APP_END
}ROBORT_APP_CMD_TYPE;


typedef enum 
{
    APP_DISTANCE_TYPE = 0,
    APP_TEMPERATURE_TYPE,
    APP_HUMIDITY_TYPE,
    APP_POSITION_TYPE,
    APP_ENERGY_TYPE
}APP_SENSOR_TYPE_TAG;




uint8 Protocol_Parser(App_Robort_Dat_Tag *pdata);  //extern interface

static uint8 Process_Robort_Sensor_Cmd(App_Robort_Dat_Tag *pdata);
static uint8 Process_Robort_Light_Cmd(App_Robort_Dat_Tag *pdata);
static uint8 Process_Robort_Beep_Cmd(App_Robort_Dat_Tag *pdata);
static uint8 Process_Robort_Speed_Cmd(App_Robort_Dat_Tag *pdata);
static uint8 Process_Robort_Steer_Cmd(App_Robort_Dat_Tag *pdata);
static uint8 Process_Robort_Direction_Cmd(App_Robort_Dat_Tag *pdata);

static uint8 Robort_Adress_Veritfy(App_Robort_Dat_Tag *pdata);
static unsigned short Assemble_Message_Length(App_Robort_Dat_Tag *pdata);
static unsigned short Crc16(uint8 *ptr, int count);
void Steer_Test( void);
#endif













