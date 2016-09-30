
#include "RTCU.h"

#include "stm32f0xx.h"
#include "crc32.h"


uint8_t RTCU::rxBuffer[RTCU::RX_BUFFER_LENGTH];
uint8_t RTCU::txBuffer[RTCU::TX_BUFFER_LENGTH];
uint8_t RTCU::rxValue[RTCU::RX_MAX_VALUE_LENGTH];

Protocol RTCU::protocol;

uint32_t RTCU::requestCounter = 0;


void RTCU::init(){

	Crc32::init();
	
	{
		uint32_t crcTemp = 0;
		Crc32::appendByte(0xA5,crcTemp);
	}

	RCC->CFGR3 &= ~RCC_CFGR3_USART1SW;
	RCC->CFGR3 |= RCC_CFGR3_USART1SW_PCLK;//00: PCLK selected as USART1 clock source (default)
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	protocol.init(
		txBuffer,
		TX_BUFFER_LENGTH,
		rxBuffer,
		RX_BUFFER_LENGTH,
		rxValue,
		RX_MAX_VALUE_LENGTH		
	);	
		

	USART1->CR2 = 0;
	USART1->CR3 = 0;
	USART1->BRR = 208;
	USART1->CR1 = 
		(0<<28)+//M1
		(0<<12)+//M0 M = 00: 1 Start bit, 8 data bits, n stop bits
		(0<<27)+// End of Block interrupt is inhibited
		(0<<26)+// Receiver timeout interrupt 
		(0<<15)+//0: Oversampling by 16
		(0<<10)+//0: Parity control disabled
		(1<<5)+// RXNEIE: RXNE interrupt enable
		(1<<3)+// TE: Transmitter enable
		(1<<2)+// RE: Receiver enable
		(1<<0)+//1: USART enabled
		0;
		
	NVIC_EnableIRQ(USART1_IRQn);


}

extern "C" {void USART1_IRQHandler(){
	if (USART1->ISR & (1<<5)){
		RTCU::protocol.receiveByte(USART1->RDR);
	}
}}

void RTCU::processTx(){
	if (
		(protocol.bytesPending()==true) && 
		(USART1->ISR & (1<<7))// TXE: Transmit data register empty
	){
		USART1->TDR = protocol.popTxByte();
	}
}

bool RTCU::processRx(){

	if (protocol.processRx()==true){	
		if (protocol.rxTag==Protocol::TAG_CheckRfidProximity){
			requestCounter++;
			return true;
		}
	}	
	return false;
	
	
}



