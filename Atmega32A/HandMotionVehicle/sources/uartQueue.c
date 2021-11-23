#include"uartQueue.h"
#include"uart.h"
#include<stdio.h>
#include<stdlib.h>
#include<util/delay.h>
#include<avr/io.h>
#include<stdint.h>

static	uint8_t head = 0;
static	uint8_t tail = 0;

uint8_t QueuePush(unsigned char buffer[], uint8_t value)
{
	uint8_t status =0; //verificare adaugare in coada
	uint8_t next;
	next = head+1;
	if(next != tail)
	{
		if(next == BUFFER_SIZE)
		{
			next = 0;
		}
		buffer[head] = value;
		head = next;
		status = 1; //adaugare reusita in coada
	}
	return status;
}

uint8_t QueuePop(unsigned char buffer[], uint8_t *value)
{
	uint8_t status = 0; //verificare existenta element in coada
	uint8_t next;
	next = tail+1;
	if(tail != head)
	{
		if(next == BUFFER_SIZE)
		{
			next = 0;
		}
		*value = buffer[tail];
		tail = next;
		status = 1;
	}
	return status;
}

void IntegerToAscii(int32_t Value, uint8_t Base, uint8_t Digits,unsigned char *Text)
{
	static uint8_t Chars[16]="0123456789ABCDEF";
	uint8_t TextIndex;
	uint8_t Sign = 0;
	Text[Digits]='\0';
	if(Value < 0)
	{
		Sign = 1 ;
	}
	for(TextIndex = 0; TextIndex < Digits; TextIndex++)
	{
		if((Value == 0) && (TextIndex > 0))
		{
			Text[Digits-TextIndex-1]=' ';
		}
		else
		{
			if(Value >= 0)
			{
				Text[Digits-TextIndex-1] = Chars[Value%Base];
			}
			else
			{
				Text[Digits-TextIndex-1] = Chars[(~Value+1)%Base];
			}
		}
		Value = Value/Base;
		if((Value ==0 ) && (Sign == 1) && (Base == 10))
		{
			Sign = 0;
			TextIndex++;
			Text[Digits-TextIndex-1]='-';
		}
	}
}


