
#ifndef BLACK_BOX_H
#define BLACK_BOX_H

extern void blackBoxInit(void);
extern void blackBoxProcess(void);
extern void blackBoxPushByte(uint8_t byte);
extern void blackBoxResponse(uint8_t tag,uint8_t* data,uint32_t dataLength);


#define BLACKBOX_RX_BUFFER_LENGTH	(512)
extern uint8_t blackBoxRxBuffer[BLACKBOX_RX_BUFFER_LENGTH];
extern uint32_t blackBoxRxBufferReceived;
extern uint32_t blackBoxRxBufferProcessed;

#define BLACKBOX_TX_BUFFER_LENGTH	(4096)
extern uint8_t blackBoxTxBuffer[BLACKBOX_TX_BUFFER_LENGTH];
extern uint32_t blackBoxTxBufferPending;
extern uint32_t blackBoxTxBufferTransmitted;






extern TStuffState stuffState;
extern TProtocolState protocolState;



typedef struct
{
		uint32_t dataCount;
		int32_t dataValue;
		uint32_t fault;
		uint32_t overflow;
	
	
} TBlackBoxStrainGauge;
extern TBlackBoxStrainGauge blackBoxStrainGauge;


#endif


