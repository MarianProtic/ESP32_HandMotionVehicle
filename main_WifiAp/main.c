/* Simple WiFi Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "uart_rxtx.h"
#include "appConnectWifiServer.h"

void app_main()
{
	NVS_init();
	wifi_init_softap();
	INIT_UART();
	xTaskCreate(&HandleSocket, "HandleSocket", 2048, NULL, 5, NULL);
	xTaskCreate(&tx_task, "uart_tx_task", 2048, NULL,4, NULL);
	xTaskCreate(&DataToDisplay, "Data to displayu",2048,NULL,1,NULL);			//minim number is low priority
}
