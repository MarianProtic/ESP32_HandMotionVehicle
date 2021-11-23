/*
 * math.c
 *
 *  Created on: Oct 16, 2019
 *      Author: uic25511
 */

#include <string.h>
#include <stdio.h>
#include<math.h>

#include "appConnectWifiClient.h"
#include "AltimuV3.h"
#include"ProcesareDateSenzor.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#define PI 3.1415

const char *TAG1 = "WifiSTA";
int16_t pitch, roll;
int16_t PWM1, PWM2;
int8_t TxDataBuffer[2];


void RollAndPitch()
{

	AltIMU_ReadData();				//read data sensor acc
	Ax = ((Ax/1)*0.244);			//g
	Ay = ((Ay/1)*0.244);
	Az = ((Az/1)*0.244);

	roll  = (atan2(-Ay, Az)*180.0)/PI;
	pitch = 180 * atan2(Ax, sqrt(Ay*Ay + Az*Az))/PI;

	if(pitch > 15 || pitch < -15)
	{
		if(pitch < 0)
			pitch = pitch + 15;
		else
			pitch = pitch -15;
		TxDataBuffer[0] = 1.11*pitch;											//( 200/180) = 1.11
		TxDataBuffer[1] = 1.11*pitch;
	}
	else
	{
		TxDataBuffer[0] = 0;												//( 200/180) = 1.11
		TxDataBuffer[1] = 0;
	}
	if((roll > 30) && (roll < 90))											//dreapta	 //PWM+MotorR-- 	//(PWM2--)
	{
		if((TxDataBuffer[1] >= 0) && (TxDataBuffer[1] < 90))
		{
			TxDataBuffer[1] = TxDataBuffer[1] - (roll-30);
			if(TxDataBuffer[1] < 0)
				TxDataBuffer[1] = 0;
		}
		else
		{
			TxDataBuffer[1] = TxDataBuffer[1] + (roll-30);
			if(TxDataBuffer[1] > 0)
				TxDataBuffer[1] = 0;
		}
	}
	if((roll < -30) && (roll > -90)) 											//stanga	 //PWM_MototrL--	//(PWM1--)
	{
		if((TxDataBuffer[0] >= 0) && (TxDataBuffer[1] < 90))
		{
			TxDataBuffer[0] = TxDataBuffer[0] + (roll+30);
			if(TxDataBuffer[0] < 0)
				TxDataBuffer[0] = 0;
		}
		else
		{
			TxDataBuffer[0] = TxDataBuffer[0] - (roll+30);
			if(TxDataBuffer[0] > 0)
				TxDataBuffer[0] = 0;
		}

	}
}

void DataToDysplay(void)
{
	while(1)
	{
		ESP_LOGI(TAG1, "Ax = %d \t Ay = %d \t Az = %d \t roll = %d \t pitch = %d \t PWM1 = %d \t PWM2 = %d ",Ax,Ay,Az,roll,pitch,TxDataBuffer[0],TxDataBuffer[1]);
		vTaskDelay(10/portTICK_PERIOD_MS);	//10
	}

}




