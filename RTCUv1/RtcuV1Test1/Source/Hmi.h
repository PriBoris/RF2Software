

#ifndef HMI_H
#define HMI_H


extern void hmiInit();
extern void hmiProcess();


#define HMI_RX_BUFFER_LENGTH	(2048)
extern uint8_t hmiRxBuffer[HMI_RX_BUFFER_LENGTH];
#define HMI_TX_BUFFER_LENGTH	(4096)
extern uint8_t hmiTxBuffer[HMI_TX_BUFFER_LENGTH];



inline void hmiTxEnableAssert()
{
		GPIOE->BSRRL = (1<<6);
};
inline void hmiTxEnableDeassert()
{
		GPIOE->BSRRH = (1<<6);
};


#define HMI_RX_MESSAGE_MAX_VALUE_LEN	(512)
class HmiRxMessage
{
public:	
	uint8_t tag;
	uint16_t valueLen;
	uint32_t id;
	uint8_t value[HMI_RX_MESSAGE_MAX_VALUE_LEN];
};

#define HMI_RX_MESSAGE_QUEUE_LEN	(32)
extern HmiRxMessage hmiRxMessageQueue[HMI_RX_MESSAGE_QUEUE_LEN];
extern uint16_t hmiRxMessageQueuePtrReceived;
extern uint16_t hmiRxMessageQueuePtrProcessed;

extern void flushHmiRxMessageQueue(void);
extern bool isHmiRxMessageQueueNotEmpty(void);
extern HmiRxMessage* popHmiRxMessage(void);







#endif
