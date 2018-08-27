#include "APP_GPS.H"
#include "BSP_Uart.h"	
#include "nmea/nmea.h"

struct GPS_Info GPS_Info_Data;
nmeaINFO info;          //GPS解码后得到的信息
nmeaPARSER parser;      //解码时使用的数据结构  
uint8_t new_parse=0;    //是否有新的解码数据标志
void GPS_DATA_Init( void)
{
	  nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
}


static u8 STM32_GET_GPS_HalfTransferEnd_FLAG()
{
	if( GPS_HalfTransferEnd)
	{
		return 1;
	}
	return 0;

}
static u8 STM32_GET_GPS_TransferEnd_FLAG()
{
	if( GPS_TransferEnd)
	{
		return 1;
	}
	return 0;
}
static void GPS_HalfTransferEnd_Nema()
{
//	u8 temp[4]={};
//	if (!parser.top_node)	//List have no data entry
//	{
		nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);  /* 进行nmea格式解码 */       
		GPS_HalfTransferEnd = 0;   //清空标志位
		GPS_Info_Data.lat.fLat= info.lat*100000;
		GPS_Info_Data.lon.fLon=info.lon*100000;
		GPS_Info_Data.direction.fDir=info.direction*100000;
		GPS_Info_Data.speed.fSpeed=info.speed*100000;
		GPS_Info_Data.sAtinfo_iNuse=info.satinfo.inuse;
		GPS_Info_Data.sAtinfo_iNview=info.satinfo.inview;
	  
//			printf("\r\n纬度：%f\r\n",info.lat);
//	    printf("\r\n纬度：%d\r\n",GPS_Info_Data.lat.fLat);
//	    printf("\r\n纬度:%d %d %d %d\r\n",GPS_Info_Data.lat.uLat[0],GPS_Info_Data.lat.uLat[1],GPS_Info_Data.lat.uLat[2],GPS_Info_Data.lat.uLat[3]);

//		printf("\r\n正在使用的卫星：%d,可见卫星：%d",info.satinfo.inuse,info.satinfo.inview);
////		printf("\r\n海拔高度：%f 米 ", info.elv);
//		printf("\r\n速度：%f km/h ", info.speed);
//		printf("\r\n航向：%f 度", info.direction);
//	}
//	else
//	{
//		nmea_parse(&parser, (const char*)&gps_rbuff[0], 0, &info);	//Only get, don't parse
//	}
//	nmea_parser_destroy(&parser);
}

static void GPS_TransferEnd_Nema()
{

//	if (!parser.top_node)	//List have no data entry
//	{
		nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);
    GPS_TransferEnd = 0;
    GPS_Info_Data.lat.fLat= info.lat*100000;
		GPS_Info_Data.lon.fLon=info.lon*100000;
		GPS_Info_Data.direction.fDir=info.direction*100000;
		GPS_Info_Data.speed.fSpeed=info.speed*100000;
		GPS_Info_Data.sAtinfo_iNuse=info.satinfo.inuse;
		GPS_Info_Data.sAtinfo_iNview=info.satinfo.inview;
	
//    printf("\r\n纬度：%f,经度%f\r\n",info.lat,info.lon);
//    printf("\r\n正在使用的卫星：%d,可见卫星：%d",info.satinfo.inuse,info.satinfo.inview);
//    printf("\r\n速度：%f km/h ", info.speed);
//    printf("\r\n航向：%f 度", info.direction);
//	}
//	else
//	{
//		nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], 0, &info);	//Only get, don't parse
//	}

			
}
void Robort_Get_Nema(void)
{
    if(STM32_GET_GPS_HalfTransferEnd_FLAG())     /* 接收到GPS_RBUFF_SIZE一半的数据 */
    {     
        GPS_HalfTransferEnd_Nema();
				
      }
     else if(STM32_GET_GPS_TransferEnd_FLAG())    /* 接收到另一半数据 */
     {

        GPS_TransferEnd_Nema();
      }
      
      
}












/**************************************************end of file****************************************/

