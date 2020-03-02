
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "driver/hal_key.h"
#include "driver/ws2812.h"
#include "os_type.h"
/** User area The current device state structure */
dataPoint_t currentDataPoint;
LOCAL os_timer_t timerSmartConfig;

int8_t ICACHE_FLASH_ATTR gizwitsEventProcess(eventInfo_t *info, uint8_t *data, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t * dataPointPtr = (dataPoint_t *)data;
    moduleStatusInfo_t * wifiData = (moduleStatusInfo_t *)data;

    if((NULL == info) || (NULL == data))
    {
        GIZWITS_LOG("!!! gizwitsEventProcess Error \n");
        return -1;
    }

    for(i = 0; i < info->num; i++)
    {
        switch(info->event[i])
        {
        case EVENT_power :
        	currentDataPoint.valueLED_Color = 3 ;
            currentDataPoint.valuepower = dataPointPtr->valuepower;
			GIZWITS_LOG("Evt: EVENT_power %d \n", currentDataPoint.valuepower);

			if (0x01 == currentDataPoint.valuepower) {
				currentDataPoint.valueLED_R = 0;
				currentDataPoint.valueLED_G = 250;
				currentDataPoint.valueLED_B = 0;
				//user handle
				WS2812_SetFillColor(currentDataPoint.valueLED_R,
						currentDataPoint.valueLED_G,
						currentDataPoint.valueLED_B);
			} else {
				currentDataPoint.valueLED_R = 0;
				currentDataPoint.valueLED_G = 0;
				currentDataPoint.valueLED_B = 0;
				//user handle
				WS2812_SetFillColor(currentDataPoint.valueLED_R,
						currentDataPoint.valueLED_G,
						currentDataPoint.valueLED_B);
			}
			break;

        case EVENT_LED_Color:
			currentDataPoint.valueLED_Color = dataPointPtr->valueLED_Color;
			GIZWITS_LOG("Evt: EVENT_LED_Color %d\n", currentDataPoint.valueLED_Color)
			;
			switch (currentDataPoint.valueLED_Color) {

			case LED_Color_VALUE0:

				currentDataPoint.valueLED_R = 254;
				currentDataPoint.valueLED_G = 251;
				currentDataPoint.valueLED_B = 0;
				//user handle
				WS2812_SetFillColor(currentDataPoint.valueLED_R,
						currentDataPoint.valueLED_G,
						currentDataPoint.valueLED_B);

				break;
			case LED_Color_VALUE1:
				//user handle

				currentDataPoint.valueLED_R = 167;
				currentDataPoint.valueLED_G = 0;
				currentDataPoint.valueLED_B = 254;
				//user handle
				WS2812_SetFillColor(currentDataPoint.valueLED_R,
						currentDataPoint.valueLED_G,
						currentDataPoint.valueLED_B);

				break;
			case LED_Color_VALUE2:
				//user handle
				currentDataPoint.valueLED_R = 254;
				currentDataPoint.valueLED_G = 0;
				currentDataPoint.valueLED_B = 57;
				//user handle
				WS2812_SetFillColor(currentDataPoint.valueLED_R,
						currentDataPoint.valueLED_G,
						currentDataPoint.valueLED_B);
				break;
			case LED_Color_VALUE3:
				break;
			}
            break;

        case EVENT_LED_R:
        	currentDataPoint.valueLED_Color = 3 ;
            currentDataPoint.valueLED_R= dataPointPtr->valueLED_R;
            GIZWITS_LOG("Evt:EVENT_LED_R %d\n",currentDataPoint.valueLED_R);
            //user handle
            WS2812_SetFillColor(currentDataPoint.valueLED_R,currentDataPoint.valueLED_G,currentDataPoint.valueLED_B);
            break;
        case EVENT_LED_G:
        	currentDataPoint.valueLED_Color = 3 ;
            currentDataPoint.valueLED_G= dataPointPtr->valueLED_G;
            GIZWITS_LOG("Evt:EVENT_LED_G %d\n",currentDataPoint.valueLED_G);
            //user handle
            WS2812_SetFillColor(currentDataPoint.valueLED_R,currentDataPoint.valueLED_G,currentDataPoint.valueLED_B);
            break;
        case EVENT_LED_B:
        	currentDataPoint.valueLED_Color = 3 ;
            currentDataPoint.valueLED_B= dataPointPtr->valueLED_B;
            GIZWITS_LOG("Evt:EVENT_LED_B %d\n",currentDataPoint.valueLED_B);
            //user handle
            WS2812_SetFillColor(currentDataPoint.valueLED_R,currentDataPoint.valueLED_G,currentDataPoint.valueLED_B);
            break;

        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
            GIZWITS_LOG("@@@@ connected router\n");

            break;
        case WIFI_DISCON_ROUTER:
            GIZWITS_LOG("@@@@ disconnected router\n");
 
            break;
        case WIFI_CON_M2M:
            GIZWITS_LOG("@@@@ connected m2m\n");
			setConnectM2MStatus(0x01);
 
            break;
        case WIFI_DISCON_M2M:
            GIZWITS_LOG("@@@@ disconnected m2m\n");
			setConnectM2MStatus(0x00);
 
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("@@@@ RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            //user handle , Fetch data from [data] , size is [len]
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
            break;
            
        default:
            break;
        }
    }
    system_os_post(USER_TASK_PRIO_2, SIG_UPGRADE_DATA, 0);
    
    return 0; 
}


void ICACHE_FLASH_ATTR userHandle(void)
{

    system_os_post(USER_TASK_PRIO_2, SIG_UPGRADE_DATA, 0);
}



#define GPIO_KEY_NUM                            1                           ///< Defines the total number of key members
#define KEY_0_IO_MUX                            PERIPHS_IO_MUX_GPIO0_U      ///< ESP8266 GPIO function
#define KEY_0_IO_NUM                            0                           ///< ESP8266 GPIO number
#define KEY_0_IO_FUNC                           FUNC_GPIO0                 ///< ESP8266 GPIO name
LOCAL key_typedef_t * singleKey[GPIO_KEY_NUM];                              ///< Defines a single key member array pointer
LOCAL keys_typedef_t keys;                                                  ///< Defines the overall key module structure pointer

LOCAL void ICACHE_FLASH_ATTR keyShortPress(void) {
	GIZWITS_LOG("#### key2 short press \n")
	;
	currentDataPoint.valueLED_Color++;
	if (currentDataPoint.valueLED_Color > 3) {
		currentDataPoint.valueLED_Color = 0;
	}

	switch (currentDataPoint.valueLED_Color) {
	case LED_Color_VALUE0:
		currentDataPoint.valueLED_R = 254;
		currentDataPoint.valueLED_G = 251;
		currentDataPoint.valueLED_B = 0;
		//user handle
		WS2812_SetFillColor(currentDataPoint.valueLED_R,
				currentDataPoint.valueLED_G, currentDataPoint.valueLED_B);

		break;
	case LED_Color_VALUE1:
		//user handle

		currentDataPoint.valueLED_R = 167;
		currentDataPoint.valueLED_G = 0;
		currentDataPoint.valueLED_B = 254;
		//user handle
		WS2812_SetFillColor(currentDataPoint.valueLED_R,
				currentDataPoint.valueLED_G, currentDataPoint.valueLED_B);

		break;
	case LED_Color_VALUE2:
		//user handle
		currentDataPoint.valueLED_R = 254;
		currentDataPoint.valueLED_G = 0;
		currentDataPoint.valueLED_B = 57;
		//user handle
		WS2812_SetFillColor(currentDataPoint.valueLED_R,
				currentDataPoint.valueLED_G, currentDataPoint.valueLED_B);
		break;
	case LED_Color_VALUE3:
		currentDataPoint.valueLED_R = 0;
		currentDataPoint.valueLED_G = 0;
		currentDataPoint.valueLED_B = 0;
		//user handle
		WS2812_SetFillColor(currentDataPoint.valueLED_R,currentDataPoint.valueLED_G, currentDataPoint.valueLED_B);
		break;
	}
	userHandle();
}

int flag = 0;
void timerSmartConfig_callback(void) {
	flag++;
	switch (flag) {
	//切换为蓝色
	case 1:
		currentDataPoint.valueLED_R = 0;
		currentDataPoint.valueLED_G = 0;
		currentDataPoint.valueLED_B = 254;
		//user handle
		WS2812_SetFillColor(currentDataPoint.valueLED_R,
				currentDataPoint.valueLED_G, currentDataPoint.valueLED_B);
		break;
	//切换为红色
	case 2:
		currentDataPoint.valueLED_R = 254;
		currentDataPoint.valueLED_G = 0;
		currentDataPoint.valueLED_B = 0;
		//user handle
		WS2812_SetFillColor(currentDataPoint.valueLED_R,
				currentDataPoint.valueLED_G, currentDataPoint.valueLED_B);
		break;
	//切换为绿色
	case 3:
		currentDataPoint.valueLED_R = 0;
		currentDataPoint.valueLED_G = 254;
		currentDataPoint.valueLED_B = 0;
		//user handle
		WS2812_SetFillColor(currentDataPoint.valueLED_R,
				currentDataPoint.valueLED_G, currentDataPoint.valueLED_B);
		break;
	case 4:
		gizwitsSetMode(WIFI_AIRLINK_MODE);
		flag = 0;
		break;
	}

}

LOCAL void ICACHE_FLASH_ATTR keyLongPress(void) {
	currentDataPoint.valueLED_R = 0;
	currentDataPoint.valueLED_G = 254;
	currentDataPoint.valueLED_B = 0;
	//user handle
	WS2812_SetFillColor(currentDataPoint.valueLED_R,currentDataPoint.valueLED_G, currentDataPoint.valueLED_B);
	gizwitsSetMode(WIFI_AIRLINK_MODE);
	GIZWITS_LOG("#### key2 long press, airlink mode\n");
}

/**
* Key to initialize
* @param none
* @return none
*/
LOCAL void ICACHE_FLASH_ATTR keyInit(void)
{
    singleKey[0] = keyInitOne(KEY_0_IO_NUM, KEY_0_IO_MUX, KEY_0_IO_FUNC,
                                keyLongPress, keyShortPress);
    keys.singleKey = singleKey;
    keyParaInit(&keys);
}

void ICACHE_FLASH_ATTR userInit(void) {
	keyInit();
	gizMemset((uint8_t *) &currentDataPoint, 0, sizeof(dataPoint_t));

	/** Warning !!!  **/

	currentDataPoint.valuepower = 1;
	currentDataPoint.valueLED_Color = 4;
	currentDataPoint.valueLED_R = 0;
	currentDataPoint.valueLED_G = 254;
	currentDataPoint.valueLED_B = 0;

}


