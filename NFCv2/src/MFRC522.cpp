
#include "MFRC522.h"

#include "Ports.h"
#include "Clock.h"
#include "stm32f0xx.h"

uint8_t MFRC522::chipSoftwareVersion;


//=================================================================================================
void MFRC522::start(){
	
	SPI::csDeassert();
	
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	
	SPI1->CR2 = 
		(1<<12)+//1: RXNE event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)
		(7<<8)+//0111: 8-bit
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

	reset();

	// When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
	Register::write(Register::TMode, 0x80);			// TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
	Register::write(Register::TPrescaler, 0xA9);		// TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25?s.
	Register::write(Register::TReloadH, 0x03);		// Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
	Register::write(Register::TReloadL, 0xE8);
	Register::write(Register::TxASK, 0x40);		// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	Register::write(Register::Mode, 0x3D);		// Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
	antennaOn();

	
}
//=================================================================================================
void MFRC522::SPI::csAssert(){
	Ports::gpioPinReset(GPIOA,4);
}
//=================================================================================================
void MFRC522::SPI::csDeassert(){
	Ports::gpioPinSet(GPIOA,4);
}
//=================================================================================================
/*extern "C"{
void SPI1_IRQHandler(){
	MFRC522::spiInterrupt();
}}
void MFRC522::spiInterrupt(){
	volatile uint8_t rxuint8_t = SPI1->DR;

}*/
//=================================================================================================
uint8_t MFRC522::SPI::rxTxByte(uint8_t tx){

	*(volatile uint8_t*)0x4001300C = tx;
	while((SPI1->SR&SPI_SR_RXNE)==0);
	return *(volatile uint8_t*)0x4001300C;
}
//=================================================================================================
uint8_t MFRC522::Register::read(uint8_t reg){

	uint8_t txByte0 = READ_COMMAND + (reg<<1);
	uint8_t txByte1 = 0x00;
	SPI::csAssert();
	SPI::rxTxByte(txByte0);
	uint8_t rxByte1 = SPI::rxTxByte(txByte1);
	SPI::csDeassert();
	return rxByte1;
	
}
//=================================================================================================
void MFRC522::Register::write(uint8_t reg,uint8_t val){

	uint8_t txByte0 = WRITE_COMMAND + (reg<<1);
	uint8_t txByte1 = val;
	SPI::csAssert();
	SPI::rxTxByte(txByte0);
	SPI::rxTxByte(txByte1);
	SPI::csDeassert();

}
//=================================================================================================
void MFRC522::Register::setBitMask(uint8_t reg,uint8_t mask){ 
	uint8_t tmp;
	tmp = read(reg);
	write(reg, tmp | mask);			// set bit mask
}
//=================================================================================================
void MFRC522::Register::clearBitMask(uint8_t reg,uint8_t mask){
	uint8_t tmp;
	tmp = read(reg);
	write(reg, tmp & (~mask));		// clear bit mask
}
//=================================================================================================
void MFRC522::reset(){

	Register::write(Register::Command,Command::SoftReset);
	Clock::pause(50);
	
	while(true){
		uint8_t commandReg = Register::read(Register::Command);
		if ((commandReg&(1<<4))==0){
			break;
		}
	}

	
	while(true){
		chipSoftwareVersion = Register::read(Register::Version);
		if((0xF0&chipSoftwareVersion)==0x90){
			break;
		}

	}


}
//=================================================================================================
void MFRC522::antennaOn(void){

	uint8_t value = Register::read(Register::TxControl);
	if ((value & 0x03) != 0x03) {
		Register::write(Register::TxControl, value | 0x03);
	}

}
//=================================================================================================
bool MFRC522::isNewCardPresent(){
	uint8_t bufferATQA[2];
	uint8_t bufferSize = sizeof(bufferATQA);
	StatusCode result = requestA(bufferATQA, &bufferSize);
	return (result == STATUS_OK || result == STATUS_COLLISION);
}
//=================================================================================================
/**
 * Transmits a REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
 * Beware: When two PICCs are in the field at the same time I often get STATUS_TIMEOUT - probably due do bad antenna design.
 * 
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522::StatusCode MFRC522::requestA(	uint8_t *bufferATQA,	///< The buffer to store the ATQA (Answer to request) in
											uint8_t *bufferSize	///< Buffer size, at least two bytes. Also number of bytes returned if STATUS_OK.
										) {
	return REQA_or_WUPA(PICC_CMD_REQA, bufferATQA, bufferSize);
} 
//=================================================================================================
/**
 * Transmits REQA or WUPA commands.
 * Beware: When two PICCs are in the field at the same time I often get STATUS_TIMEOUT - probably due do bad antenna design.
 * 
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */ 
MFRC522::StatusCode MFRC522::REQA_or_WUPA(	uint8_t command, 		///< The command to send - PICC_CMD_REQA or PICC_CMD_WUPA
												uint8_t *bufferATQA,	///< The buffer to store the ATQA (Answer to request) in
												uint8_t *bufferSize	///< Buffer size, at least two bytes. Also number of bytes returned if STATUS_OK.
											) {
	uint8_t validBits;
	MFRC522::StatusCode status;
	
	if (bufferATQA == NULL || *bufferSize < 2) {	// The ATQA response is 2 bytes long.
		return STATUS_NO_ROOM;
	}
	Register::clearBitMask(CollReg, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.
	validBits = 7;									// For REQA and WUPA we need the short frame format - transmit only 7 bits of the last (and only) byte. TxLastBits = BitFramingReg[2..0]
	status = transceiveData(&command, 1, bufferATQA, bufferSize, &validBits);
	if (status != STATUS_OK) {
		return status;
	}
	if (*bufferSize != 2 || validBits != 0) {		// ATQA must be exactly 16 bits.
		return STATUS_ERROR;
	}
	return STATUS_OK;
}
//=================================================================================================
/**
 * Executes the Transceive command.
 * CRC validation can only be done if backData and backLen are specified.
 * 
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522::StatusCode MFRC522::transceiveData(	uint8_t *sendData,		///< Pointer to the data to transfer to the FIFO.
													uint8_t sendLen,		///< Number of bytes to transfer to the FIFO.
													uint8_t *backData,		///< NULL or pointer to buffer if data should be read back after executing the command.
													uint8_t *backLen,		///< In: Max number of bytes to write to *backData. Out: The number of bytes returned.
													uint8_t *validBits,	///< In/Out: The number of valid bits in the last byte. 0 for 8 valid bits. Default NULL.
													uint8_t rxAlign,		///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
													bool checkCRC		///< In: True => The last two bytes of the response is assumed to be a CRC_A that must be validated.
								 ) {
	uint8_t waitIRq = 0x30;		// RxIRq and IdleIRq
	return communicate(PCD_Transceive, waitIRq, sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC);
}
/**
 * Transfers data to the MFRC522 FIFO, executes a command, waits for completion and transfers data back from the FIFO.
 * CRC validation can only be done if backData and backLen are specified.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522::StatusCode MFRC522::communicate(	uint8_t command,		///< The command to execute. One of the PCD_Command enums.
														uint8_t waitIRq,		///< The bits in the ComIrqReg register that signals successful completion of the command.
														uint8_t *sendData,		///< Pointer to the data to transfer to the FIFO.
														uint8_t sendLen,		///< Number of bytes to transfer to the FIFO.
														uint8_t *backData,		///< NULL or pointer to buffer if data should be read back after executing the command.
														uint8_t *backLen,		///< In: Max number of bytes to write to *backData. Out: The number of bytes returned.
														uint8_t *validBits,	///< In/Out: The number of valid bits in the last byte. 0 for 8 valid bits.
														uint8_t rxAlign,		///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
														bool checkCRC		///< In: True => The last two bytes of the response is assumed to be a CRC_A that must be validated.
									 ) {
	uint8_t n, _validBits;
	unsigned int i;
	
	// Prepare values for BitFramingReg
	uint8_t txLastBits = validBits ? *validBits : 0;
	uint8_t bitFraming = (rxAlign << 4) + txLastBits;		// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]
	
	PCD_WriteRegister(CommandReg, PCD_Idle);			// Stop any active command.
	PCD_WriteRegister(ComIrqReg, 0x7F);					// Clear all seven interrupt request bits
	PCD_SetRegisterBitMask(FIFOLevelReg, 0x80);			// FlushBuffer = 1, FIFO initialization
	PCD_WriteRegister(FIFODataReg, sendLen, sendData);	// Write sendData to the FIFO
	PCD_WriteRegister(BitFramingReg, bitFraming);		// Bit adjustments
	PCD_WriteRegister(CommandReg, command);				// Execute the command
	if (command == PCD_Transceive) {
		PCD_SetRegisterBitMask(BitFramingReg, 0x80);	// StartSend=1, transmission of data starts
	}
	
	// Wait for the command to complete.
	// In PCD_Init() we set the TAuto flag in TModeReg. This means the timer automatically starts when the PCD stops transmitting.
	// Each iteration of the do-while-loop takes 17.86�s.
	i = 2000;
	while (1) {
		n = PCD_ReadRegister(ComIrqReg);	// ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
		if (n & waitIRq) {					// One of the interrupts that signal success has been set.
			break;
		}
		if (n & 0x01) {						// Timer interrupt - nothing received in 25ms
			return STATUS_TIMEOUT;
		}
		if (--i == 0) {						// The emergency break. If all other conditions fail we will eventually terminate on this one after 35.7ms. Communication with the MFRC522 might be down.
			return STATUS_TIMEOUT;
		}
	}
	
	// Stop now if any errors except collisions were detected.
	uint8_t errorRegValue = PCD_ReadRegister(ErrorReg); // ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
	if (errorRegValue & 0x13) {	 // BufferOvfl ParityErr ProtocolErr
		return STATUS_ERROR;
	}	

	// If the caller wants data back, get it from the MFRC522.
	if (backData && backLen) {
		n = PCD_ReadRegister(FIFOLevelReg);			// Number of bytes in the FIFO
		if (n > *backLen) {
			return STATUS_NO_ROOM;
		}
		*backLen = n;											// Number of bytes returned
		PCD_ReadRegister(FIFODataReg, n, backData, rxAlign);	// Get received data from FIFO
		_validBits = PCD_ReadRegister(ControlReg) & 0x07;		// RxLastBits[2:0] indicates the number of valid bits in the last received byte. If this value is 000b, the whole uint8_t is valid.
		if (validBits) {
			*validBits = _validBits;
		}
	}
	
	// Tell about collisions
	if (errorRegValue & 0x08) {		// CollErr
		return STATUS_COLLISION;
	}
	
	// Perform CRC_A validation if requested.
	if (backData && backLen && checkCRC) {
		// In this case a MIFARE Classic NAK is not OK.
		if (*backLen == 1 && _validBits == 4) {
			return STATUS_MIFARE_NACK;
		}
		// We need at least the CRC_A value and all 8 bits of the last uint8_t must be received.
		if (*backLen < 2 || _validBits != 0) {
			return STATUS_CRC_WRONG;
		}
		// Verify CRC_A - do our own calculation and store the control in controlBuffer.
		uint8_t controlBuffer[2];
		MFRC522::StatusCode status = PCD_CalculateCRC(&backData[0], *backLen - 2, &controlBuffer[0]);
		if (status != STATUS_OK) {
			return status;
		}
		if ((backData[*backLen - 2] != controlBuffer[0]) || (backData[*backLen - 1] != controlBuffer[1])) {
			return STATUS_CRC_WRONG;
		}
	}
	
	return STATUS_OK;
}
//=================================================================================================

