#ifndef INCLUDES_UART_H_
#define INCLUDES_UART_H_

#define F_CPU 			16000000
#define BAUDRATE 		9600
#define CBR 		((F_CPU / (BAUDRATE * 16UL)) - 1) //'16UL' an unsigned long (Asynchronous Normal mode (U2Xn = 0))

extern void UartInit(void);
extern void uartSendNr(int32_t value);
extern void uartSendMsg(char* msg);
extern unsigned char uartReceive(void);
extern void uartTxHandler(void);
extern void uartReceiveFromESP(void);



#endif /* INCLUDES_UART_H_ */
