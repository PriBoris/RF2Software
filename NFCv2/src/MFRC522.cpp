
#include "MFRC522.h"

#include "Ports.h"
#include "stm32f0xx.h"

//=================================================================================================
void MFRC522::start(){
	
	spiCsDeassert();
	
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	
	SPI1->CR2 = 
		//(1<<6)+//1: RXNE interrupt not masked. Used to generate an interrupt request when the RXNE flag is set.
		0;
	SPI1->CR1 = 
		(0<<15)+//0: 2-line unidirectional data mode selected
		(0<<13)+//0: CRC calculation disabled
		(0<<10)+//0: Full duplex (Transmit and receive)
		(1<<9)+//1: Software slave management enabled    !!! no idea why it is necessary !!!
		(1<<8)+// SSI: Internal slave select    !!! no idea why it is necessary !!!
		(0<<7)+//0: MSB transmitted first
		(1<<6)+//1: Peripheral enabled
		(2<<3)+//010: fPCLK/8 = 24/8=3MHz
		(1<<2)+//1: Master configuration
		(0<<1)+//0: CK to 0 when idle
		(0<<0)+//0: The first clock transition is the first data capture edge
		0;
	 //NVIC_EnableIRQ(SPI1_IRQn);

	
}
//=================================================================================================
void MFRC522::spiCsAssert(){
	Ports::gpioPinReset(GPIOA,4);
}
//=================================================================================================
void MFRC522::spiCsDeassert(){
	Ports::gpioPinSet(GPIOA,4);
}
//=================================================================================================
extern "C"{
void SPI1_IRQHandler(){
	MFRC522::spiInterrupt();
}}
void MFRC522::spiInterrupt(){
	volatile uint8_t rxByte = SPI1->DR;

}
//=================================================================================================
uint8_t MFRC522::spiRxTxByte(uint8_t tx){
	SPI1->DR = tx;
	while((SPI1->SR&SPI_SR_RXNE)==0);
	return SPI1->DR;
}
//=================================================================================================
uint8_t MFRC522::spiReadRegister(uint8_t reg){

	uint8_t txByte0 = READ_COMMAND + (reg<<1);
	uint8_t txByte1 = 0x00;
	spiCsAssert();
	spiRxTxByte(txByte0);
	uint8_t rxByte1 = spiRxTxByte(txByte1);
	spiCsDeassert();
	return rxByte1;
	
}
//=================================================================================================
void MFRC522::spiWriteRegister(uint8_t reg,uint8_t val){

	uint8_t txByte0 = WRITE_COMMAND + (reg<<1);
	uint8_t txByte1 = val;
	spiCsAssert();
	spiRxTxByte(txByte0);
	spiRxTxByte(txByte1);
	spiCsDeassert();

}
//=================================================================================================


