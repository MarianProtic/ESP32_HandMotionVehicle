#include"initialization.h"
#include"timer.h"
#include"motors.h"
#include"uart.h"
#include"uartQueue.h"

void Initialization(void)
{
	Timer1Init();
	UartInit();
	motorsInit();
	sei();
}
