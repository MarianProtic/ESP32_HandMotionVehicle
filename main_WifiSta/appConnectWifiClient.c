#include<math.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "AltimuV3.h"
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

#include"ProcesareDateSenzor.h"

#define PI 3.1415

/* The examples use simple WiFi configuration that you can set via
 'make menuconfig'.

 If you'd rather not, just change the below entries to strings with
 the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
 */
#define EXAMPLE_ESP_WIFI_SSID      "MarPro"
#define EXAMPLE_ESP_WIFI_PASS      "123454321"

int clientSocket;
struct sockaddr_in serverAddr;
socklen_t addr_size;
int err;
const char *TAG = "simple wifiSTA";

int period = 10 / portTICK_PERIOD_MS;								//5ms



//**
/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
 but we only care about one event - are we connected
 to the AP with an IP? */
const int WIFI_CONNECTED_BIT = BIT0;



static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	switch (event->event_id)
	{
	case SYSTEM_EVENT_STA_START:
		esp_wifi_connect();
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
		break;
	case SYSTEM_EVENT_STA_GOT_IP:
		ESP_LOGI(TAG, "got ip:%s",
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

void wifi_init_sta()
{
	tcpip_adapter_init();
	wifi_event_group = xEventGroupCreate();

	printf("tcpip adapter init\n");
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	//ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	wifi_config_t wifi_config =
	{ .sta =
	{ .ssid = EXAMPLE_ESP_WIFI_SSID, .password = EXAMPLE_ESP_WIFI_PASS }, };

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGI(TAG, "wifi_init_sta finished.");
	ESP_LOGI(TAG, "connect to ap SSID:%s password:%s", EXAMPLE_ESP_WIFI_SSID,
			EXAMPLE_ESP_WIFI_PASS);
}

void appSocket(void)
{

	/*---- Configure settings of the server address struct ----*/
	/* Address family = Internet */
	serverAddr.sin_family = AF_INET;
	/* Set IP address to localhost */
	serverAddr.sin_addr.s_addr = inet_addr("192.168.4.1");
	/* Set port number, using htons function to use proper byte order */
	serverAddr.sin_port = htons(8080);

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(clientSocket < 0)
	{
		ESP_LOGE(TAG, "Unable to create socket");
	}
	/*---- Connect the socket to the server using the address struct ----*/
	err = connect(clientSocket, (struct sockaddr*)(&serverAddr),sizeof(serverAddr));
	if (err != 0)
	{
		ESP_LOGE(TAG, "Socket unable to connect !!!");
	}

	while(1)
	{
		/*	//Citire date senzor		//update Roll and Pitch*/
		RollAndPitch();

		if (err < 0)
		{
			ESP_LOGE(TAG, "Error on sending data !!!");

		}
		else
		{
			err = write(clientSocket, TxDataBuffer, 2);
			//ESP_LOGI(TAG, "Data sent !");
		}

		//closesocket(clientSocket);
		//clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		//connect(clientSocket, (struct sockaddr*)(&serverAddr),sizeof(serverAddr));
		vTaskDelay(period);
	}
}

void NVS_init(void)
{
	//Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES
			|| ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
}
void wait_for_ip(void)
{
	uint32_t bits = WIFI_CONNECTED_BIT;

	ESP_LOGI(TAG, "Waiting for AP connection...");
	xEventGroupWaitBits(wifi_event_group, bits, false, true, portMAX_DELAY);
	ESP_LOGI(TAG, "Connected to AP");
}

