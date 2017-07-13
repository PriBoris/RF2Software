

#include "RtcuV1Test1.h"

uint8_t hmiRxBuffer[HMI_RX_BUFFER_LENGTH];
uint8_t hmiTxBuffer[HMI_TX_BUFFER_LENGTH];

Protocol hmiProtocol;

HmiRxMessage hmiRxMessageQueue[HMI_RX_MESSAGE_QUEUE_LEN];
uint16_t hmiRxMessageQueuePtrReceived;
uint16_t hmiRxMessageQueuePtrProcessed;


//==============================================================================================
void hmiInit()
{
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2, ENABLE);

	hmiProtocol.init(
		hmiTxBuffer,
		HMI_TX_BUFFER_LENGTH,
		hmiRxBuffer,
		HMI_RX_BUFFER_LENGTH
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

	hmiTxEnableAssert();

	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //Received Data Ready to be Read
	
	
	
	
}
//==============================================================================================
extern "C" {void USART2_IRQHandler()
{
	
	if (USART2->SR & USART_IT_RXNE)
	{
		hmiProtocol.receiveByte(USART2->DR);
	}

	
}}
//==============================================================================================
void hmiProcess()
{

	if (
		(hmiProtocol.bytesPending()==true) && 
		(USART2->SR & USART_FLAG_TXE)
	)
	{
			USART2->DR = hmiProtocol.popTxByte();
	}
	
	if (hmiProtocol.processRx()==true)
	{
		HmiRxMessage *message = (HmiRxMessage*)&hmiRxMessageQueue[hmiRxMessageQueuePtrReceived];
		if (hmiProtocol.getRxDataLen()<=HMI_RX_MESSAGE_MAX_VALUE_LEN)
		{
				message->id = hmiProtocol.getRxID();
				message->tag = hmiProtocol.getRxTag();
				message->valueLen = hmiProtocol.getRxDataLen();
				hmiProtocol.getRxValue((uint8_t*)&message->value);
				hmiRxMessageQueuePtrReceived = (hmiRxMessageQueuePtrReceived+1)&(HMI_RX_MESSAGE_QUEUE_LEN-1);
		}

		
		
	}
	

}
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





