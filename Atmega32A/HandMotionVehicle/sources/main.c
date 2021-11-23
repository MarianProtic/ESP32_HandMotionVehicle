#include"os.h"
#include"initialization.h"
#include<avr/io.h>
#include<stdint.h>

int main(void)
{
	DDRD |= 0b00100000;	//PD5 green led
	DDRB |= 0b00000001;	//PB0 red led

	Initialization();
	OsInit();
	OsStart();

	while(1)
	{


	}
	return 0;
}

