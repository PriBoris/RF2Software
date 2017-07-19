
#ifndef NFC_H
#define NFC_H

#include <stdint.h>


extern void nfcInit(void);
extern void nfcTxProcess(void);
extern bool nfcRxProcess(void);
extern void nfcPushByte(uint8_t txByte);

#define NFC_RX_BUFFER_LENGTH	(1024)
extern uint8_t nfcRxBuffer[NFC_RX_BUFFER_LENGTH];
#define NFC_TX_BUFFER_LENGTH	(1024)
extern uint8_t nfcTxBuffer[NFC_TX_BUFFER_LENGTH];





extern uint8_t nfcUIDLength;
extern uint8_t nfcUID[16];



#endif
