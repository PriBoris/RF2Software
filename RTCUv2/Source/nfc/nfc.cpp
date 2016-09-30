
#include "nfc.h"

#include <string.h>

#include "stm32f4xx_conf.h"
#include "hmi/RxMessageQueue.h"


uint8_t NFC::rxBuffer[RX_BUFFER_LENGTH];
uint8_t NFC::txBuffer[TX_BUFFER_LENGTH];
uint8_t NFC::rxValue[RX_VALUE_LENGTH];
Protocol NFC::protocol;

//==============================================================================================
void NFC::init(){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	protocol.init(
		txBuffer,
		TX_BUFFER_LENGTH,
		rxBuffer,
		RX_BUFFER_LENGTH,
		rxValue,
		RX_VALUE_LENGTH		
		);	


	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART6, ENABLE);
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); //Received Data Ready to be Read
	
	
	
}
//==============================================================================================
extern "C" {void USART6_IRQHandler(){
	if (USART6->SR & USART_IT_RXNE){
		NFC::protocol.receiveByte(USART6->DR);
	}
}}
//==============================================================================================
void NFC::processTx(){

	if (
		(protocol.bytesPending()==true) && 
		(USART6->SR & USART_FLAG_TXE)
	){
		USART6->DR = protocol.popTxByte();
	}

}
//==============================================================================================




