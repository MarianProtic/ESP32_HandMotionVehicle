/*
 * uart_rxtx.c
 *
 *  Created on: Oct 30, 2019
 *      Author: uic25511
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "string.h"

#include "appConnectWifiServer.h"

static const int RX_BUF_SIZE = 1024;
int j = 0;
//#define TXD_PIN (GPIO_NUM_1)
//#define RXD_PIN (GPIO_NUM_3)

//**
/*
 * - Port: UART1
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below
 */


#define ECHO_TEST_TXD  (GPIO_NUM_33)
#define ECHO_TEST_RXD  (GPIO_NUM_32)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

static const char *TAG1 = "WifiAP";
//#define BUF_SIZE (1024)

void INIT_UART()
{
	/* Configure parameters of an UART driver,
	 * communication pins and install the driver */
	uart_config_t uart_config = {
			.baud_rate = 9600,
			.data_bits = UART_DATA_8_BITS,
			.parity    = UART_PARITY_DISABLE,
			.stop_bits = UART_STOP_BITS_1,
			.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};
	uart_param_config(UART_NUM_1, &uart_config);
	uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
	uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);



}

int sendData(const char* logName, const char* data)
{
	const int len = strlen(data);
	const int txBytes = uart_write_bytes(UART_NUM_1, data, len);	//			//const int txBytes = uart_tx_chars(UART_NUM_1, data, len);
	//   ESP_LOGI(logName, "Wrote %d bytes", txBytes);
	return txBytes;
}



void tx_task(void)
{

	static const char *TX_TASK_TAG = "TX_TASK";
	esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
	while (1)
	{
		uart_write_bytes(UART_NUM_1,(const char*)UartTxBuffer,2);

		vTaskDelay(15 / portTICK_PERIOD_MS);
	}
}

void DataToDisplay(void)
{
	while(1)
	{
		ESP_LOGI(TAG1,"PWM1 = %d \t PWM2 = %d",UartTxBuffer[0],UartTxBuffer[1]);
		vTaskDelay(15/portTICK_PERIOD_MS);
	}
}
//void rx_task()
//{
//    static const char *RX_TASK_TAG = "RX_TASK";
//    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
//    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
//    while (1)
//    {
//    	const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
//    	if (rxBytes > 0) {
//    		data[rxBytes] = 0;
//    		ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
//    		ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
//    	}
//    	//vTaskDelay(1000 / portTICK_PERIOD_MS);
//    }
//    free(data);
//}


