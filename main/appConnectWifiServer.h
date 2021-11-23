/*
 * appConnectWifiServer.h
 *
 *  Created on: Oct 28, 2019
 *      Author: uic25511
 */

#ifndef MAIN_APPCONNECTWIFISERVER_H_
#define MAIN_APPCONNECTWIFISERVER_H_

extern int8_t UartTxBuffer[2];

extern void HandleSocket(void);
extern void wifi_init_softap(void);
extern void NVS_init(void);


#endif /* MAIN_APPCONNECTWIFISERVER_H_ */
