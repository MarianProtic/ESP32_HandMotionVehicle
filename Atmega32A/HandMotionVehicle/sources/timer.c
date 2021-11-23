
#include"os.h"
#include"timer.h"
#include"initialization.h"
#include"motors.h"
#include"uart.h"
#include"uartQueue.h"
#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdint.h>
#include <stdlib.h>

/*initialize timer3 for os*/
void Timer3Init(void)
{
	TCNT3 = 0x00;
	TCCR3A = 0x00;
	TCCR3B = 0x05; // prescaler 1024
	TCCR3C = 0x00;
}

/* initialize timer1 i n PWM, Phase and Frequency Correct ....250us*/
void Timer1Init(void)
{
	/*set period*/
	ICR1 = 2000;
	/* set duty cicle until the program will start*/
	OCR1A = 1999;
	OCR1B = 1999;

	TIMSK1 |= 0x01; // Interrupt Enable
	TCCR1A = 0xF0; // Set OCRA/OCRB on compare match
	TCCR1B = 0x11; // No prescaling
	TCCR1C = 0x00;
}

/*interrupt function for PWM1 and PWM2*/
ISR(TIMER1_OVF_vect)
{
		OCR1B = PWM1;					//PWM1
		OCR1A = PWM2;					//PWM2
		PORTD &= 0b11011111;			//led green on
}


