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

Task Tasks[OS_NUMBER_OF_TASKS];

void Task1ms(void);
void Task10ms(void);
void Task100ms(void);
void Task1000ms(void);

static void OsConfigureTask(uint8_t TaskNo, uint16_t period, void *ptrtfunc, uint16_t offset)
{
	Tasks[TaskNo].function=ptrtfunc;
	Tasks[TaskNo].period=period;
	Tasks[TaskNo].offset=offset;
	Tasks[TaskNo].last_tick=0;
	Tasks[TaskNo].state=OS_TASK_SUSPENDED;
}

void OsInit(void)
{
	OsConfigureTask(OS_TASK_1MS,OS_MS_TO_TICKS(1),&Task1ms,0);
	OsConfigureTask(OS_TASK_10MS,OS_MS_TO_TICKS(10),&Task10ms,19);
	OsConfigureTask(OS_TASK_100MS,OS_MS_TO_TICKS(100),&Task100ms,37);
	OsConfigureTask(OS_TASK_1000MS,OS_MS_TO_TICKS(1000),&Task1000ms,56);
}

void OsStart(void)
{
	uint8_t TaskIndex;
	Timer3Init();
	sei();

	while(1)
	{
		for(TaskIndex=0; TaskIndex<OS_NUMBER_OF_TASKS; TaskIndex++)
		{
			if(Tasks[TaskIndex].state == OS_TASK_SUSPENDED)
			{
				if(Tasks[TaskIndex].offset<=TCNT3)
				{
					Tasks[TaskIndex].last_tick = TCNT3;
					Tasks[TaskIndex].state = OS_TASK_READY;
				}
			}
			else
			{
				if((TCNT3 - Tasks[TaskIndex].last_tick) >= Tasks[TaskIndex].period)
				{
					Tasks[TaskIndex].last_tick = TCNT3;
					Tasks[TaskIndex].function();
				}
			}
		}
	}
}

void Task1ms(void)
{
	uartTxHandler();
}

void Task10ms(void)
{





	//uartSendMsg(uartReceive());

}

void Task100ms(void)
{



}

void Task1000ms(void)
{

	//PORTD &= 0b11011111;	//led green on


	//	char a;
	//	a = UDR1;
	//	ReceiveBuffer = atoi(a);

	//	LED(ReceiveBuffer);
	//PORTD &= 0b11011111;	//led green on
	//	uartSendMsg("Buna ziua!");
	//	uartSendMsg("\n");
	//	uartSendMsg(uartReceive());
	//

	//	uartReceive();
	//
	//	if(UDR1 != 0x00)
	//	{
	//		PORTC |= 0x80;
	//	}

}
