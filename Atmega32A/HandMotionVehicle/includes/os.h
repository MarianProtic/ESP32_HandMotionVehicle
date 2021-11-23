#ifndef INCLUDES_OS_H_
#define INCLUDES_OS_H_
#include<stdint.h>

#define OS_NUMBER_OF_TASKS 	4

#define OS_TASK_1MS	    0
#define OS_TASK_10MS	1
#define OS_TASK_100MS	2
#define OS_TASK_1000MS	3

#define OS_MS_TO_TICKS(ms) (ms/0.064)


#define OS_TASK_READY 		1
#define OS_TASK_SUSPENDED 	0


typedef struct task
{
	void (*function)(void);
	uint16_t period;
	uint16_t last_tick;
	uint16_t offset;
	uint8_t state;
}Task;

extern void OsInit(void);
extern void OsStart(void);


#endif /* INCLUDES_OS_H_ */
