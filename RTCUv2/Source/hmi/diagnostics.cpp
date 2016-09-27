
#include "diagnostics.h"

#include "stm32f4xx_conf.h"
#include "protocol.h"
#include "RxMessageQueue.h"


uint8_t Diagnostics::rxBuffer[RX_BUFFER_LENGTH];
uint8_t Diagnostics::txBuffer[TX_BUFFER_LENGTH];
uint8_t Diagnostics::rxValue[HMI::RX_MAX_VALUE_LENGTH];
Protocol Diagnostics::protocol;




void Diagnostics::init(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	protocol.init(
		txBuffer,
		TX_BUFFER_LENGTH,
		rxBuffer,
		RX_BUFFER_LENGTH,
		rxValue,
		HMI::RX_MAX_VALUE_LENGTH
		);	
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //Received Data Ready to be Read
	
	
}
//==============================================================================================
extern "C" {void USART3_IRQHandler()
{
	
	if (USART3->SR & USART_IT_RXNE)
	{
		Diagnostics::protocol.receiveByte(USART3->DR);
	}

	
}}
//==============================================================================================
void Diagnostics::process(){

	if (
		(protocol.bytesPending()==true) && 
		(USART3->SR & USART_FLAG_TXE)
	){
		USART3->DR = protocol.popTxByte();
	}

	if (protocol.processRx()==true){	
		RxMessageQueue::push(
			protocol.rxTag,
			protocol.rxDataLen,
			protocol.rxID,
			false,
			protocol.rxValue
		);	
	}

	
}
//==============================================================================================
