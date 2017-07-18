

#include "diagnostics.h"

#include "stm32f2xx_conf.h"
#include "RxMessageQueue.h"


Protocol Diagnostics::protocol;

uint8_t Diagnostics::rxBuffer[Diagnostics::RX_BUFFER_LENGTH];
uint8_t Diagnostics::txBuffer[Diagnostics::TX_BUFFER_LENGTH];
uint8_t Diagnostics::rxValue[Diagnostics::RX_MAX_VALUE_LENGTH];



//=================================================================================================
void Diagnostics::init(){

	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
	
	GPIOA->BSRRH = (1<<12); //assert RTS

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
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //Received Data Ready to be Read



}
//=================================================================================================
void Diagnostics::process(){

	if (
		(protocol.bytesPending()==true) && 
		(USART1->SR & USART_FLAG_TXE)
	){
		USART1->DR = protocol.popTxByte();
	}

	if (protocol.processRx()==true){	
		RxMessageQueue::push(
			protocol.rxTag,
			protocol.rxDataLen,
			protocol.rxID,
			false,
			rxValue
		);	
	}	

	
}
//=================================================================================================
extern "C" {void USART1_IRQHandler(){
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET){
		Diagnostics::protocol.receiveByte(USART1->DR);
	}
}}
//=================================================================================================
