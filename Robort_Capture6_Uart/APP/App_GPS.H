#ifndef _APP_GPS_H
#define _APP_GPS_H
#include "BSP_GPS.h"
#include "stm32f10x.h"

struct GPS_Info{
	//存储经度信息
	union{
		uint32_t fLat;
		u8   uLat[4];
	}lat;
	//存储维度信息
	union {
		uint32_t fLon;
		u8    uLon[4];   
	}lon;
	//存储航向信息
	union{
		uint32_t fDir;
		u8    uDir[4];
	}direction;
	u8 sAtinfo_iNuse;
	u8 sAtinfo_iNview;
	union{
		uint32_t fSpeed;
		u8    uSpeed[4]; 
	}speed;	
};
extern struct GPS_Info GPS_Info_Data;
void Robort_Get_Nema(void);
void GPS_DATA_Init( void);
#endif
