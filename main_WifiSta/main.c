/*
 * main.c
 *
 *  Created on: Oct 11, 2019
 *      Author: uic25511
 */
#include "appConnectWifiClient.h"
#include "AltimuV3.h"
#include "ProcesareDateSenzor.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <string.h>
#include <stdio.h>




void app_main()
{
	AltIMU_Init();														//Initializare senzor Acc + Magn + Gyro
	NVS_init();															//ESP_ERROR_CHECK( NVS_init() );
	wifi_init_sta();													//vTaskDelay(1500);
	wait_for_ip();
	xTaskCreate(&appSocket, "appSocket",2048, NULL,5,NULL);				//
	xTaskCreate(&DataToDysplay,"Data to display",2048,NULL,1,NULL);		//Low priority numbers denote low priority tasks

}
