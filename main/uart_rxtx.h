/*
 * uart_rxtx.h
 *
 *  Created on: Oct 30, 2019
 *      Author: uic25511
 */

#ifndef MAIN_UART_RXTX_H_
#define MAIN_UART_RXTX_H_


extern void init(void);
extern void rx_task(void);
extern void tx_task(void);
extern void INIT_UART(void);
extern void DataToDisplay(void);
#endif /* MAIN_UART_RXTX_H_ */
