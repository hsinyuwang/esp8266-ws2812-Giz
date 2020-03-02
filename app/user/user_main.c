
#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "gagent_soc.h"
#include "user_devicefind.h"
#include "user_webserver.h"
#include "gizwits_product.h"
#include "driver/ws2812.h"
#include "user_esp_platform.h"


#ifdef SERVER_SSL_ENABLE
#include "ssl/cert.h"
#include "ssl/private_key.h"
#else
#ifdef CLIENT_SSL_ENABLE
unsigned char *default_certificate;
unsigned int default_certificate_len = 0;
unsigned char *default_private_key;
unsigned int default_private_key_len = 0;
#endif
#endif

uint32_t ICACHE_FLASH_ATTR user_rf_cal_sector_set()
{
    return 636;
}


void ICACHE_FLASH_ATTR user_init(void)
{
    uint32_t system_free_size = 0;
    wifi_station_set_auto_connect(1);
    wifi_set_sleep_type(NONE_SLEEP_T);//set none sleep mode
    espconn_tcp_set_max_con(10);
    GIZWITS_LOG( "---------------SDK version:%s--------------\n", system_get_sdk_version());
    WS2812_Init();
    gizwitsInit();
}
