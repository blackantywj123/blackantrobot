
#include "stm32f10x.h"
#include "BSP_Uart.h"	
#include "nmea/nmea.h"
#include "BSP_GPS.h"

nmeaINFO info;          //GPS�����õ�����Ϣ
nmeaPARSER parser;      //����ʱʹ�õ����ݽṹ 
nmeaINFO GPS_Info;

/**
  * @brief  nmea_decode_test ����GPSģ����Ϣ
  * @param  ��
  * @retval ��
  */
int nmea_decode_test(void)
{

    nmeaINFO info;          //GPS�����õ�����Ϣ
    nmeaPARSER parser;      //����ʱʹ�õ����ݽṹ  
    uint8_t new_parse=0;    //�Ƿ����µĽ������ݱ�־
		uint32_t lat;
  
    nmeaTIME beiJingTime;    //����ʱ�� 

    /* �����������������Ϣ�ĺ��� */
    nmea_property()->trace_func = &trace;
    nmea_property()->error_func = &error;

    /* ��ʼ��GPS���ݽṹ */
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
   
    while(1)
    {
      if(GPS_HalfTransferEnd)     /* ���յ�GPS_RBUFF_SIZEһ������� */
      {
//        /* ����nmea��ʽ���� */
        nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);
				 STM32_UART1_Write((u8*)&(info.satinfo.inuse),1);
//        
        GPS_HalfTransferEnd = 0;   //��ձ�־λ
        new_parse = 1;             //���ý�����Ϣ��־ 
				
      }
      else if(GPS_TransferEnd)    /* ���յ���һ������ */
      {

        nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);
       
        GPS_TransferEnd = 0;
        new_parse =1;
				
      }
      
      if(new_parse )                //���µĽ�����Ϣ   
      { 
				
        /* �Խ�����ʱ�����ת����ת���ɱ���ʱ�� */
//        GMTconvert(&info.utc,&beiJingTime,8,1);
				
        lat=info.lat*1000000;
				printf("\r\nγ�ȣ�%d\r\n",lat);
        printf("\r\nʱ��%d,%d,%d,%d,%d,%d\r\n", beiJingTime.year+1900, beiJingTime.mon+1,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
        printf("\r\nγ�ȣ�%f,����%f\r\n",info.lat,info.lon);
        printf("\r\n����ʹ�õ����ǣ�%d,�ɼ����ǣ�%d",info.satinfo.inuse,info.satinfo.inview);
        printf("\r\n���θ߶ȣ�%f �� ", info.elv);
        printf("\r\n�ٶȣ�%f km/h ", info.speed);
        printf("\r\n����%f ��", info.direction);
       STM32_UART1_Write((u8*)&(info.satinfo.inuse),1);
        new_parse = 0;
      }
	
	}

    /* �ͷ�GPS���ݽṹ */
    // nmea_parser_destroy(&parser);

    
    //  return 0;
}







/**************************************************end of file****************************************/

