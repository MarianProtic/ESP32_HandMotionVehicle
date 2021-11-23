#include"uartQueue.h"
#include"uart.h"
#include"motors.h"
#include<avr/io.h>
#include<stdint.h>
#include<stdio.h>
#include<avr/interrupt.h>
#include<string.h>
#include <stdlib.h>

static 	unsigned char buffer[BUFFER_SIZE];
uint8_t ReceiverBuffer=0;								//uint16_t ReceiveBuffer;
volatile uint8_t comTrigger = 0;

int16_t uartPWM1;
int16_t uartPWM2;
uint8_t FlagReceiver = 0;

void UartInit(void)
{
	UBRR1=0;
	UCSR1B|= (1<<TXCIE1) | (1<<RXCIE1) | (1<<TXEN1) | (1<<RXEN1); // enable receiver and transmitter and interrupts
	UBRR1H=(uint8_t)(CBR>>8);
	UBRR1L=(uint8_t) CBR;        //set baud rate

	///***
	// UCSR1C = (1<<USBS1)|(3<<UCSZ10);		/* Set frame format: 8data, 2 stop bit */
}

unsigned char uartReceive(void)
{
	/*astepta ca datele sa fie primite*/
	while (!(UCSR1A & (1<<RXC1)));		//(RXC1=7) //1<<7 = 128 = 0b10000000
 	return UDR1;
}

ISR(USART1_RX_vect)
{
	/*if flag == 0,
	 * receive PWM1
	 * else
	 * receive PWM2
	 */
	if(FlagReceiver == 0)
	{
		uartPWM1 = uartReceive();
		if(uartPWM1 > 100)
			uartPWM1 = uartPWM1 - 256;

		/*sned PWM1 receiver to PC (UART)*/
				uartSendMsg("\nPWM1=");
				uartSendNr(uartPWM1);
		FlagReceiver++;
	}
	else
	{
		uartPWM2 = uartReceive();
		if(uartPWM2 > 100)
			uartPWM2 = uartPWM2 - 256;

		/*sned PWM2 receiver to PC (UART)*/
				uartSendMsg("\tPWM2=");
				uartSendNr(uartPWM2);
		FlagReceiver = 0;

		/*motor PWM init*/
		SetSpeed(uartPWM1, uartPWM2);
	}
}

ISR(USART1_TX_vect)
{
	uint8_t val;

	if(QueuePop(buffer,&val) == 1)
	{
		UDR1 = val;
	}
	else
	{
		comTrigger = 0; //activez trigger
	}
}

void uartTxHandler(void)
{
	uint8_t val;
	if(comTrigger == 0)
	{
		if(QueuePop(buffer,&val) == 1)
		{
			comTrigger = 1;
			UDR1 = val;
		}
	}
}

void uartSendMsg(char *msg)
{
	while(*msg != '\0')
	{
		QueuePush(buffer,*msg++);

	}
}

void uartSendNr(int32_t value)
{
	uint8_t i = 0;
	unsigned char text[16];
	IntegerToAscii(value,10,7,text);


	while(text[i] != '\0')
	{
		QueuePush(buffer,text[i]);
		i++;

	}
}

void uartReceiveFromESP(void)
{


}


