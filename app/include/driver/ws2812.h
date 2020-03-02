#ifndef __WS2812_H__
#define __WS2812_H__

#include "c_types.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
/***********************************************************************************/
//                 WS2812����
/***********************************************************************************/
#define WS2812_PIN 12
#define WS2812_FUN FUNC_GPIO12
#define WS2812_MUX PERIPHS_IO_MUX_MTDI_U

#define PIXEL_MAX   24

//����ˢ����С���
#define WS2812_DELAY()  os_delay_us(500);


/***********************************************************************************/
//                 WS2812���ݽṹ
/***********************************************************************************/
typedef struct
{
    uint8 b;    //��
    uint8 g;    //��
    uint8 r;    //��
    uint8 w;    //�Ҷ�(������)
}WRGB;

//������
typedef union
{
    uint32  c;      //32bit ��ɫֵ
    WRGB    wrgb;   //wrgb�ṹ��
}COLOR_DATA;

typedef struct
{
    COLOR_DATA at[PIXEL_MAX];
}COLOR_BUFF;



/***********************************************************************************/
//                 ����API
/***********************************************************************************/
#define IFA     ICACHE_FLASH_ATTR

#define IRA     ICACHE_RODATA_ATTR


#define COLOR(r,g,b)    (uint32)(((uint32_t)r << 16)|((uint32_t)g <<8)|b)

void    IFA WS2812_Init( void );
void    IFA WS2812_RefreshShow( void );
void      WS2812_Send24bit( uint32 color );
uint8   IFA WS2812_SetPixelColor( uint32 color , uint16 n );
void    IFA WS2812_SetFillColor(int r, int g, int b) ;

void IFA WS2812_GetColorBuff( COLOR_BUFF *colorBuff );
void IFA WS2812_SetColorBuff( const COLOR_BUFF *colorBuff );

#endif

