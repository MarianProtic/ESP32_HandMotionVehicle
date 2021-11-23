/*
 * ConnectWifi.c
 *
 *  Created on: Oct 8, 2019
 *      Author: uic25511
 */
/* Simple WiFi Example

 This example code is in the Public Domain (or CC0 licensed, at your option.)

 Unless required by applicable law or agreed to in writing, this
 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 CONDITIONS OF ANY KIND, either express or implied.
 */

#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<math.h>

#include "lwip/err.h"
#include "lwip/sys.h"
#include "uart_rxtx.h"


/* The examples use simple WiFi configuration that you can set via
 'make menuconfig'.

 If you'd rather not, just change the below entries to strings with
 the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
 */
#define EXAMPLE_ESP_WIFI_SSID      "MarPro"
#define EXAMPLE_ESP_WIFI_PASS      "123454321"
#define EXAMPLE_MAX_STA_CONN		5

int EventIsConected = 0;

int welcomeSocket, newSocket;
char buffer[1024];
int err;
int clientSock;
struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size;

int8_t RxDataBuffer[2];
int8_t UartTxBuffer[2];


/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
 but we only care about one event - are we connected
 to the AP with an IP? */
const int WIFI_CONNECTED_BIT = BIT0;

static const char *TAG = "WifiAP";

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	switch (event->event_id)
	{
	case SYSTEM_EVENT_STA_START:
		esp_wifi_connect();
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
		break;
	case SYSTEM_EVENT_STA_GOT_IP:
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP ip:%s",
				ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
		xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
		break;

	case SYSTEM_EVENT_STA_DISCONNECTED:
		/* This is a workaround as ESP32 WiFi libs don't currently auto-reassociate. */
		esp_wifi_connect();
		xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
		break;
	default:
		break;
	}
	return ESP_OK;
}

void wifi_init_softap()
{
	wifi_event_group = xEventGroupCreate();

	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	//    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );    				//+++

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT()
																																											;
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	wifi_config_t wifi_config =
	{ .ap =
	{ .ssid = EXAMPLE_ESP_WIFI_SSID, .ssid_len = strlen(
			EXAMPLE_ESP_WIFI_SSID), .password = EXAMPLE_ESP_WIFI_PASS, .max_connection =
					EXAMPLE_MAX_STA_CONN, .authmode = WIFI_AUTH_WPA_WPA2_PSK }, };
	if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0)
	{
		wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}
	ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGI(TAG, "wifi_init_softap finished.SSID:%s password:%s",
			EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
}

void HandleSocket(void)
{

	int16_t Length=-1;
	welcomeSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (welcomeSocket < 0)
	{
		ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
	}
	else
		ESP_LOGI(TAG, "Socket created");
	/*---- Configure settings of the server address struct ----*/
	/* Address family = Internet */
	serverAddr.sin_family = AF_INET;
	/* Set IP address to localhost */
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/* Set port number, using htons function to use proper byte order */
	serverAddr.sin_port = htons(8080);
	/*---- Bind the address struct to the socket ----*/
	err = bind(welcomeSocket, (struct sockaddr *)(&serverAddr),sizeof(serverAddr));
	if (err != 0)
	{
		ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);

	}
	ESP_LOGI(TAG, "Socket bound, port %d", 8080);

	/*---- Listen on the socket, with 5 max connection requests queued ----*/
	err = listen(welcomeSocket, 1);
	if (err != 0)
	{
		ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
	}
	else
	{
		ESP_LOGI(TAG, "Socket listening");
	}

	/*---- Accept call creates a new socket for the incoming connection ----*/
	uint addr_len = sizeof(serverAddr);
	clientSock = accept(welcomeSocket, (struct sockaddr *)&serverAddr,  &addr_len);
	if (clientSock >=0 )
	{
		ESP_LOGI(TAG, "Connection accepted");
	}

	while(1)
	{
		Length = read(clientSock, RxDataBuffer, 2);
		if(Length>0)
		{
			UartTxBuffer[0]=RxDataBuffer[0];
			UartTxBuffer[1]=RxDataBuffer[1];
			//ESP_LOGI(TAG,"%d %d",(int8_t)RxDataBuffer[0],(int8_t)RxDataBuffer[1]);
		}
		else
		{
			ESP_LOGE(TAG, "No data receive!");
			closesocket(clientSock);

		}
		vTaskDelay(5/portTICK_RATE_MS);
	}
}

void NVS_init(void)
{
	//Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
}




