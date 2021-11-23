#ifndef INCLUDES_MOTORS_H_
#define INCLUDES_MOTORS_H_

#include <avr/io.h>

extern uint16_t speedMotorLeft;
extern uint16_t speedMotorRight;



extern void motorsInit(void);
extern void SetSpeed(int16_t PWM1, int16_t PWM2);
extern void SetDirection(int16_t LPWM1, int16_t RPWM2);
extern uint16_t PWM1;
extern uint16_t PWM2;
#endif /* INCLUDES_MOTORS_H_ */
