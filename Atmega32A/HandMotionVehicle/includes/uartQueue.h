#ifndef INCLUDES_UARTQUEUE_H_
#define INCLUDES_UARTQUEUE_H_
#include <stdio.h>
#define BUFFER_SIZE 100

uint8_t QueuePop(unsigned char buffer[], uint8_t *value);
uint8_t QueuePush(unsigned char buffer[], uint8_t value);
void IntegerToAscii(int32_t Value, uint8_t Base, uint8_t Digits,unsigned char *Text);

#endif /* INCLUDES_UARTQUEUE_H_ */
