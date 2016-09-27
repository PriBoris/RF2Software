
#include "hmi.h"

#include "stm32f4xx_conf.h"
#include "protocol.h"
#include "RxMessageQueue.h"

uint8_t HMI::rxBuffer[RX_BUFFER_LENGTH];
uint8_t HMI::txBuffer[TX_BUFFER_LENGTH];
uint8_t HMI::rxValue[HMI::RX_MAX_VALUE_LENGTH];
Protocol HMI::protocol;


void HMI::powerOn(){
	GPIOA->BSRRL = (1<<8);
}

void HMI::powerOff(){
	GPIOA->BSRRH = (1<<8);
}	

void HMI::powerKeyAssert(){
	GPIOA->BSRRL = (1<<11);
}

void HMI::powerKeyDeassert(){
	GPIOA->BSRRH = (1<<11);
}

void HMI::init(){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

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
	USART_Init(USART1, &USART_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //Received Data Ready to be Read
	
	powerOff();
	powerKeyDeassert();


	
}
//==============================================================================================
extern "C" {void USART1_IRQHandler(){
	if (USART1->SR & USART_IT_RXNE){
		HMI::protocol.receiveByte(USART1->DR);
	}
}}
//==============================================================================================
void HMI::process(){

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
			true,
			protocol.rxValue
		);	
	}



}
//==============================================================================================
bool HMI::stopButtonPressed(){
//	GPIOB->IDR & (1<<7)
	
	return true;
	
}	
//==============================================================================================


