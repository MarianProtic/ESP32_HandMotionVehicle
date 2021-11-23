/*
 * AltimuV3.c
 *
 *  Created on: Oct 11, 2019
 *      Author: uic25511
 */
#include"freertos/FreeRTOS.h"
#include "freertos/task.h"
#include"driver/i2c.h"
#include"driver/gpio.h"
#include"esp_attr.h"
#include "esp_err.h"
#include "esp_log.h"

#include"AltimuV3.h"

#define tag "AltImuV3"

#define SCL_PIN 22
#define SDA_PIN 21

#define LSM303D			0x3A		//Acc and Magn addres			//LSM303D			0x48

/* Declare accelerometer axes */
int16_t Ax,Ay,Az;

uint8_t AccDataBuffer[6];

static esp_err_t AltIMU_WriteRegister(uint8_t SlaveAddress,uint8_t RegisterAddress, uint8_t RegisterData);

void AltIMU_Init(void)
{
	esp_err_t espRC;

	i2c_config_t i2c_config = {
			.mode = I2C_MODE_MASTER,
			.sda_io_num = SDA_PIN,
			.scl_io_num = SCL_PIN,
			.sda_pullup_en = GPIO_PULLUP_ENABLE,
			.scl_pullup_en = GPIO_PULLUP_ENABLE,
			.master.clk_speed = 100000									 //100 kH
	};
	i2c_param_config(I2C_NUM_0, &i2c_config);
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0,0);

	//Confuguration Acc sensor
	espRC = AltIMU_WriteRegister(LSM303D,0x1F,0b10000000);					//1:reboot memory content
	vTaskDelay(100);
	espRC = AltIMU_WriteRegister(LSM303D,0x1F,0b00000010);					//CTRL0 //INT1 filter - enable	//Reboot memory content. Default value: 0 (0: normal mode; 1: reboot memory content)
	vTaskDelay(300);
	espRC = AltIMU_WriteRegister(LSM303D,0x20,0b01110111);			//new val --> 200Hz 		//CTRL1	//Acc rate - 6.25Hz		//BDU (Block Data Update)	 //Acc axes enable	//0b01100111
	espRC = AltIMU_WriteRegister(LSM303D,0x21,0b00011000);					//CTRL2 //Anti-alias filter		//Acceleration full-scale +-8g		//Self test - disable	//SPI mode
	espRC = AltIMU_WriteRegister(LSM303D,0x22,0b11111110);					//CTRL3	//Boot om INT1 - disable	//INT1	//Interrupt - disable	//FIFO - disable
	espRC = AltIMU_WriteRegister(LSM303D,0x23,0b00000000);					//CTRL4	//INT2	//Interrupt - disable	//FIFO - disable
	espRC = AltIMU_WriteRegister(LSM303D,0x24,0b00000101);					//CTRL5	//Temperature sensor - disable	//Magnetic resolution selection - low resolution	//Magnetic data rate selection - 6.25Hz	//interrupt request not latched INT1 and INT2
	espRC = AltIMU_WriteRegister(LSM303D,0x25,0b01000000);					//CTRL6 //Magnetic full-scale selection+-8g
	espRC = AltIMU_WriteRegister(LSM303D,0x26,0b00000000);					//CTRL7	// High-pass filter mode selection for acceleration data - Normal mode (reset X, Y and Z-axis reading)	//Magnetic sensor mode - normal mode
	espRC = AltIMU_WriteRegister(LSM303D,0x07,0b10000000);					//STATUS REG
	vTaskDelay(300);
}

static esp_err_t AltIMU_WriteRegister(uint8_t SlaveAddress,uint8_t RegisterAddress, uint8_t RegisterData)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,SlaveAddress,1);
	i2c_master_write_byte(cmd,RegisterAddress,1);
	i2c_master_write_byte(cmd,RegisterData,1);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0,cmd,1);
	i2c_cmd_link_delete(cmd);
	return 0;
}

void AltIMU_ReadData(void)
{
	i2c_cmd_handle_t cmd;
	/*Acc sensor read*/
	cmd=i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,LSM303D,1);
	i2c_master_write_byte(cmd,0xA8,1);
	i2c_master_stop(cmd);

	i2c_master_cmd_begin(I2C_NUM_0,cmd,1/portTICK_PERIOD_MS);

	i2c_cmd_link_delete(cmd);
	cmd=i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,LSM303D|0x01,1);
	i2c_master_read(cmd,&AccDataBuffer[0],6,0);
	i2c_master_stop(cmd);

	i2c_master_cmd_begin(I2C_NUM_0,cmd,1/portTICK_PERIOD_MS);

	i2c_cmd_link_delete(cmd);
	Ax = (int16_t)(((uint16_t)AccDataBuffer[1] << 8)| AccDataBuffer[0]);
	Ay = (int16_t)(((uint16_t)AccDataBuffer[3] << 8)| AccDataBuffer[2]);
	Az = (int16_t)(((uint16_t)AccDataBuffer[5] << 8)| AccDataBuffer[4]);
}



