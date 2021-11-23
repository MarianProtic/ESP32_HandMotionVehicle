

#ifdef MAIN_MPU9250_H_
#define MAIN_MPU9250_H_


#endif /* MAIN_MPU9250_H_ */
#include<STDINT.H>
#include"ESP_ERR.H"
#include"DRIVER/I2C.H"

/* DECLARE ACCELEROMETER AXES */
extern int16_t Ax,Ay,Az;



extern void AltIMU_Init(void);
extern void AltIMU_ReadData(void);
