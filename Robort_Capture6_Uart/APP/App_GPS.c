#include "APP_GPS.H"
#include "BSP_Uart.h"	
#include "nmea/nmea.h"

struct GPS_Info GPS_Info_Data;
nmeaINFO info;          //GPS�����õ�����Ϣ
nmeaPARSER parser;      //����ʱʹ�õ����ݽṹ  
uint8_t new_parse=0;    //�Ƿ����µĽ������ݱ�־
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
		nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);  /* ����nmea��ʽ���� */       
		GPS_HalfTransferEnd = 0;   //��ձ�־λ
		GPS_Info_Data.lat.fLat= info.lat*100000;
		GPS_Info_Data.lon.fLon=info.lon*100000;
		GPS_Info_Data.direction.fDir=info.direction*100000;
		GPS_Info_Data.speed.fSpeed=info.speed*100000;
		GPS_Info_Data.sAtinfo_iNuse=info.satinfo.inuse;
		GPS_Info_Data.sAtinfo_iNview=info.satinfo.inview;
	  
//			printf("\r\nγ�ȣ�%f\r\n",info.lat);
//	    printf("\r\nγ�ȣ�%d\r\n",GPS_Info_Data.lat.fLat);
//	    printf("\r\nγ��:%d %d %d %d\r\n",GPS_Info_Data.lat.uLat[0],GPS_Info_Data.lat.uLat[1],GPS_Info_Data.lat.uLat[2],GPS_Info_Data.lat.uLat[3]);

//		printf("\r\n����ʹ�õ����ǣ�%d,�ɼ����ǣ�%d",info.satinfo.inuse,info.satinfo.inview);
////		printf("\r\n���θ߶ȣ�%f �� ", info.elv);
//		printf("\r\n�ٶȣ�%f km/h ", info.speed);
//		printf("\r\n����%f ��", info.direction);
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
	
//    printf("\r\nγ�ȣ�%f,����%f\r\n",info.lat,info.lon);
//    printf("\r\n����ʹ�õ����ǣ�%d,�ɼ����ǣ�%d",info.satinfo.inuse,info.satinfo.inview);
//    printf("\r\n�ٶȣ�%f km/h ", info.speed);
//    printf("\r\n����%f ��", info.direction);
//	}
//	else
//	{
//		nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], 0, &info);	//Only get, don't parse
//	}

			
}
void Robort_Get_Nema(void)
{
    if(STM32_GET_GPS_HalfTransferEnd_FLAG())     /* ���յ�GPS_RBUFF_SIZEһ������� */
    {     
        GPS_HalfTransferEnd_Nema();
				
      }
     else if(STM32_GET_GPS_TransferEnd_FLAG())    /* ���յ���һ������ */
     {

        GPS_TransferEnd_Nema();
      }
      
      
}












/**************************************************end of file****************************************/

