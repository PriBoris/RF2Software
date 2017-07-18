

#include "hmi.h"

#include "stm32f2xx_conf.h"
#include "RxMessageQueue.h"


Protocol HMI::protocol;

uint8_t HMI::rxBuffer[HMI::RX_BUFFER_LENGTH];
uint8_t HMI::txBuffer[HMI::TX_BUFFER_LENGTH];
uint8_t HMI::rxValue[HMI::RX_MAX_VALUE_LENGTH];



//=================================================================================================
void HMI::init(){

	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2, ENABLE);

	protocol.init(
		txBuffer,
		TX_BUFFER_LENGTH,
		rxBuffer,
		RX_BUFFER_LENGTH,
		rxValue,
		RX_MAX_VALUE_LENGTH
		);	


	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	txEnableAssert();

	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //Received Data Ready to be Read
	
	


}



//=================================================================================================
void HMI::process(){

	if (
		(protocol.bytesPending()==true) && 
		(USART2->SR & USART_FLAG_TXE)
	){
		USART2->DR = protocol.popTxByte();
	}

/*	if (protocol.processRx()==true){
		
		HmiRxMessage *message = (HmiRxMessage*)&hmiRxMessageQueue[hmiRxMessageQueuePtrReceived];
		if (protocol.getRxDataLen()<=RX_MAX_VALUE_LENGTH){
			
				message->id = protocol.getRxID();
				message->tag = protocol.getRxTag();
				message->valueLen = protocol.getRxDataLen();
				protocol.getRxValue((uint8_t*)&message->value);
				hmiRxMessageQueuePtrReceived = (hmiRxMessageQueuePtrReceived+1)&(HMI_RX_MESSAGE_QUEUE_LEN-1);
		}
	}
*/	


	if (protocol.processRx()==true){	
		RxMessageQueue::push(
			protocol.rxTag,
			protocol.rxDataLen,
			protocol.rxID,
			true,
			rxValue
		);	
	}	
}
//=================================================================================================
void HMI::txEnableAssert(){
	GPIOE->BSRRL = (1<<6);
}
//=================================================================================================
void HMI::txEnableDeassert(){
	GPIOE->BSRRH = (1<<6);
}
//=================================================================================================
extern "C" {void USART2_IRQHandler(){
	
	if (USART2->SR & USART_IT_RXNE){
		HMI::protocol.receiveByte(USART2->DR);
	}
}}
//==============================================================================================



/*

HmiRxMessage hmiRxMessageQueue[HMI_RX_MESSAGE_QUEUE_LEN];
uint16_t hmiRxMessageQueuePtrReceived;
uint16_t hmiRxMessageQueuePtrProcessed;


//==============================================================================================
void flushHmiRxMessageQueue(void)
{
		hmiRxMessageQueuePtrProcessed = hmiRxMessageQueuePtrReceived;
}
//==============================================================================================
bool isHmiRxMessageQueueNotEmpty(void)
{
	return (hmiRxMessageQueuePtrProcessed==hmiRxMessageQueuePtrReceived)?(false):(true);
}
//==============================================================================================
HmiRxMessage* popHmiRxMessage(void)
{
		uint16_t ptrProcessed = hmiRxMessageQueuePtrProcessed;
		hmiRxMessageQueuePtrProcessed = (hmiRxMessageQueuePtrProcessed+1)&(HMI_RX_MESSAGE_QUEUE_LEN-1);
		return (HmiRxMessage*)&hmiRxMessageQueue[ptrProcessed];
}
//==============================================================================================





*/
