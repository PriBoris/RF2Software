
#include "MFRC522.h"

#include "Ports.h"
#include "Clock.h"
#include "stm32f0xx.h"


uint8_t MFRC522::chipSoftwareVersion;
MFRC522::Uid MFRC522::uid;	

uint32_t MFRC522::Tag::detectionCounter;
uint32_t MFRC522::Tag::readSerialCounter;


//=================================================================================================
void MFRC522::start(){
	
	SPI::init();

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

	Tag::detectionCounter = 0;
	Tag::readSerialCounter = 0;
}
//=================================================================================================
void MFRC522::SPI::init(){

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

}
/*extern "C"{
void SPI1_IRQHandler(){
	MFRC522::spiInterrupt();
}}
void MFRC522::spiInterrupt(){
	volatile uint8_t rxuint8_t = SPI1->DR;
}*/
//=================================================================================================
void MFRC522::SPI::csAssert(){
	Ports::gpioPinReset(GPIOA,4);
}
//=================================================================================================
void MFRC522::SPI::csDeassert(){
	Ports::gpioPinSet(GPIOA,4);
}
//=================================================================================================
uint8_t MFRC522::SPI::rxTxByte(uint8_t tx){

	*(volatile uint8_t*)0x4001300C = tx;
	while((SPI1->SR&SPI_SR_RXNE)==0);
	return *(volatile uint8_t*)0x4001300C;
}
//=================================================================================================
uint8_t MFRC522::Register::read(uint8_t reg){

	uint8_t txByteAddress = READ_COMMAND + (reg<<1);
	SPI::csAssert();
	SPI::rxTxByte(txByteAddress);
	uint8_t rxByteData = SPI::rxTxByte(0x00);
	SPI::csDeassert();
	return rxByteData;
	
}
//=================================================================================================
void MFRC522::Register::read(
	uint8_t reg,		///< The register to read from. One of the PCD_Register enums. 
	uint8_t count,		///< The number of bytes to read 
	uint8_t *values,	///< Byte array to store the values in. 
	uint8_t rxAlign	///< Only bit positions rxAlign..7 in values[0] are updated.
	){

	uint8_t txByteAddress = READ_COMMAND + (reg<<1);
	SPI::csAssert();

	SPI::rxTxByte(txByteAddress);
	uint8_t index = 0;
	while (index < count){

		if (index == 0 && rxAlign) {		// Only update bit positions rxAlign..7 in values[0]
			// Create bit mask for bit positions rxAlign..7
			uint8_t mask = 0;
			for (uint8_t i = rxAlign; i <= 7; i++) {
				mask |= (1 << i);
			}
			// Read value and tell that we want to read the same address again.
			uint8_t value = SPI::rxTxByte(txByteAddress);
			// Apply mask to both current value of values[0] and the new data in value.
			values[0] = (values[index] & ~mask) | (value & mask);
		}
		else { // Normal case
			values[index] = SPI::rxTxByte(txByteAddress);
		}
		index++;
	}
	values[index] = SPI::rxTxByte(0x00);

	SPI::csDeassert();

}
//=================================================================================================
void MFRC522::Register::write(uint8_t reg,uint8_t val){

	SPI::csAssert();
	SPI::rxTxByte(WRITE_COMMAND + (reg<<1));
	SPI::rxTxByte(val);
	SPI::csDeassert();

}
//=================================================================================================
void MFRC522::Register::write(uint8_t reg, uint8_t count, uint8_t *values){

	SPI::csAssert();

	SPI::rxTxByte(WRITE_COMMAND + (reg<<1));

	for (uint8_t index = 0; index < count; index++) {
		SPI::rxTxByte(values[index]);
	}

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

	Register::write(Register::Command,ReaderCommand::SoftReset);
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
/**
 * Transmits a REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
 * Beware: When two PICCs are in the field at the same time I often get STATUS_TIMEOUT - probably due do bad antenna design.
 * 
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522::StatusCode MFRC522::requestA(	uint8_t *bufferATQA,	///< The buffer to store the ATQA (Answer to request) in
											uint8_t *bufferSize	///< Buffer size, at least two bytes. Also number of bytes returned if STATUS_OK.
										) {
	return REQA_or_WUPA(TAG_CMD_REQA, bufferATQA, bufferSize);
} 
//=================================================================================================
/**
 * Transmits REQA or WUPA commands.
 * Beware: When two PICCs are in the field at the same time I often get STATUS_TIMEOUT - probably due do bad antenna design.
 * 
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */ 
MFRC522::StatusCode MFRC522::REQA_or_WUPA(	uint8_t command, 		///< The command to send - TAG_CMD_REQA or TAG_CMD_WUPA
												uint8_t *bufferATQA,	///< The buffer to store the ATQA (Answer to request) in
												uint8_t *bufferSize	///< Buffer size, at least two bytes. Also number of bytes returned if STATUS_OK.
											) {
	uint8_t validBits;
	MFRC522::StatusCode status;
	
	if (bufferATQA == 0 || *bufferSize < 2) {	// The ATQA response is 2 bytes long.
		return STATUS_NO_ROOM;
	}
	Register::clearBitMask(Register::Coll, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.
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
	return communicate(ReaderCommand::Transceive, waitIRq, sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC);
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
	
	Register::write(Register::Command, ReaderCommand::Idle);			// Stop any active command.
	Register::write(Register::ComIrq, 0x7F);					// Clear all seven interrupt request bits
	Register::setBitMask(Register::FIFOLevel, 0x80);			// FlushBuffer = 1, FIFO initialization
	Register::write(Register::FIFOData, sendLen, sendData);	// Write sendData to the FIFO
	Register::write(Register::BitFraming, bitFraming);		// Bit adjustments
	Register::write(Register::Command, command);				// Execute the command
	if (command == ReaderCommand::Transceive) {
		Register::setBitMask(Register::BitFraming, 0x80);	// StartSend=1, transmission of data starts
	}
	
	// Wait for the command to complete.
	// In PCD_Init() we set the TAuto flag in TModeReg. This means the timer automatically starts when the PCD stops transmitting.
	// Each iteration of the do-while-loop takes 17.86�s.
	i = 2000;
	while (1) {
		n = Register::read(Register::ComIrq);	// ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
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
	uint8_t errorRegValue = Register::read(Register::Error); // ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
	if (errorRegValue & 0x13) {	 // BufferOvfl ParityErr ProtocolErr
		return STATUS_ERROR;
	}	

	// If the caller wants data back, get it from the MFRC522.
	if (backData && backLen) {
		n = Register::read(Register::FIFOLevel);			// Number of bytes in the FIFO
		if (n > *backLen) {
			return STATUS_NO_ROOM;
		}
		*backLen = n;											// Number of bytes returned
		Register::read(Register::FIFOData, n, backData, rxAlign);	// Get received data from FIFO
		_validBits = Register::read(Register::Control) & 0x07;		// RxLastBits[2:0] indicates the number of valid bits in the last received byte. If this value is 000b, the whole uint8_t is valid.
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
		MFRC522::StatusCode status = calculateCRC(&backData[0], *backLen - 2, &controlBuffer[0]);
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
MFRC522::StatusCode MFRC522::calculateCRC(uint8_t *data, uint8_t length, uint8_t *result){

	Register::write(Register::Command, ReaderCommand::Idle);		// Stop any active command.
	Register::write(Register::DivIrq, 0x04);				// Clear the CRCIRq interrupt request bit
	Register::setBitMask(Register::FIFOLevel, 0x80);		// FlushBuffer = 1, FIFO initialization
	Register::write(Register::FIFOData, length, data);	// Write data to the FIFO
	Register::write(Register::Command, ReaderCommand::CalcCRC);		// Start the calculation
	
	// Wait for the CRC calculation to complete. Each iteration of the while-loop takes 17.73�s.
	uint16_t i = 5000;
	uint8_t n;
	while (1) {
		n = Register::read(Register::DivIrq);	// DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
		if (n & 0x04) {						// CRCIRq bit set - calculation done
			break;
		}
		if (--i == 0) {						// The emergency break. We will eventually terminate on this one after 89ms. Communication with the MFRC522 might be down.
			return STATUS_TIMEOUT;
		}
	}
	Register::write(Register::Command, ReaderCommand::Idle);		// Stop calculating CRC for new content in the FIFO.
	
	// Transfer the result from the registers to the result buffer
	result[0] = Register::read(Register::CRCResultL);
	result[1] = Register::read(Register::CRCResultH);
	return STATUS_OK;

}
//=================================================================================================
MFRC522::StatusCode MFRC522::select(Uid *uid, uint8_t validBits){

	bool uidComplete;
	bool selectDone;
	bool useCascadeTag;
	uint8_t cascadeLevel = 1;
	MFRC522::StatusCode result;
	uint8_t count;
	uint8_t index;
	uint8_t uidIndex;					// The first index in uid->uidByte[] that is used in the current Cascade Level.
	int8_t currentLevelKnownBits;		// The number of known UID bits in the current Cascade Level.
	uint8_t buffer[9];					// The SELECT/ANTICOLLISION commands uses a 7 byte standard frame + 2 bytes CRC_A
	uint8_t bufferUsed;				// The number of bytes used in the buffer, ie the number of bytes to transfer to the FIFO.
	uint8_t rxAlign;					// Used in BitFramingReg. Defines the bit position for the first bit received.
	uint8_t txLastBits;				// Used in BitFramingReg. The number of valid bits in the last transmitted byte. 
	uint8_t *responseBuffer;
	uint8_t responseLength;
	
	// Description of buffer structure:
	//		Byte 0: SEL 				Indicates the Cascade Level: PICC_CMD_SEL_CL1, PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
	//		Byte 1: NVB					Number of Valid Bits (in complete command, not just the UID): High nibble: complete bytes, Low nibble: Extra bits. 
	//		Byte 2: UID-data or CT		See explanation below. CT means Cascade Tag.
	//		Byte 3: UID-data
	//		Byte 4: UID-data
	//		Byte 5: UID-data
	//		Byte 6: BCC					Block Check Character - XOR of bytes 2-5
	//		Byte 7: CRC_A
	//		Byte 8: CRC_A
	// The BCC and CRC_A are only transmitted if we know all the UID bits of the current Cascade Level.
	//
	// Description of bytes 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft: UID contents and cascade levels)
	//		UID size	Cascade level	Byte2	Byte3	Byte4	Byte5
	//		========	=============	=====	=====	=====	=====
	//		 4 bytes		1			uid0	uid1	uid2	uid3
	//		 7 bytes		1			CT		uid0	uid1	uid2
	//						2			uid3	uid4	uid5	uid6
	//		10 bytes		1			CT		uid0	uid1	uid2
	//						2			CT		uid3	uid4	uid5
	//						3			uid6	uid7	uid8	uid9
	
	// Sanity checks
	if (validBits > 80) {
		return STATUS_INVALID;
	}
	
	// Prepare MFRC522
	Register::clearBitMask(Register::Coll, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.
	
	// Repeat Cascade Level loop until we have a complete UID.
	uidComplete = false;
	while (!uidComplete) {
		// Set the Cascade Level in the SEL byte, find out if we need to use the Cascade Tag in byte 2.
		switch (cascadeLevel) {
			case 1:
				buffer[0] = TAG_CMD_SEL_CL1;
				uidIndex = 0;
				useCascadeTag = validBits && uid->size > 4;	// When we know that the UID has more than 4 bytes
				break;
			
			case 2:
				buffer[0] = TAG_CMD_SEL_CL2;
				uidIndex = 3;
				useCascadeTag = validBits && uid->size > 7;	// When we know that the UID has more than 7 bytes
				break;
			
			case 3:
				buffer[0] = TAG_CMD_SEL_CL3;
				uidIndex = 6;
				useCascadeTag = false;						// Never used in CL3.
				break;
			
			default:
				return STATUS_INTERNAL_ERROR;
				//break;
		}
		
		// How many UID bits are known in this Cascade Level?
		currentLevelKnownBits = validBits - (8 * uidIndex);
		if (currentLevelKnownBits < 0) {
			currentLevelKnownBits = 0;
		}
		// Copy the known bits from uid->uidByte[] to buffer[]
		index = 2; // destination index in buffer[]
		if (useCascadeTag) {
			buffer[index++] = TAG_CMD_CT;
		}
		uint8_t bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0); // The number of bytes needed to represent the known bits for this level.
		if (bytesToCopy) {
			uint8_t maxBytes = useCascadeTag ? 3 : 4; // Max 4 bytes in each Cascade Level. Only 3 left if we use the Cascade Tag
			if (bytesToCopy > maxBytes) {
				bytesToCopy = maxBytes;
			}
			for (count = 0; count < bytesToCopy; count++) {
				buffer[index++] = uid->uidByte[uidIndex + count];
			}
		}
		// Now that the data has been copied we need to include the 8 bits in CT in currentLevelKnownBits
		if (useCascadeTag) {
			currentLevelKnownBits += 8;
		}
		
		// Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
		selectDone = false;
		while (!selectDone) {
			// Find out how many bits and bytes to send and receive.
			if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
				//Serial.print(F("SELECT: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
				buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes
				// Calculate BCC - Block Check Character
				buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
				// Calculate CRC_A
				result = calculateCRC(buffer, 7, &buffer[7]);
				if (result != STATUS_OK) {
					return result;
				}
				txLastBits		= 0; // 0 => All 8 bits are valid.
				bufferUsed		= 9;
				// Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
				responseBuffer	= &buffer[6];
				responseLength	= 3;
			}
			else { // This is an ANTICOLLISION.
				//Serial.print(F("ANTICOLLISION: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
				txLastBits		= currentLevelKnownBits % 8;
				count			= currentLevelKnownBits / 8;	// Number of whole bytes in the UID part.
				index			= 2 + count;					// Number of whole bytes: SEL + NVB + UIDs
				buffer[1]		= (index << 4) + txLastBits;	// NVB - Number of Valid Bits
				bufferUsed		= index + (txLastBits ? 1 : 0);
				// Store response in the unused part of buffer
				responseBuffer	= &buffer[index];
				responseLength	= sizeof(buffer) - index;
			}
			
			// Set bit adjustments
			rxAlign = txLastBits;											// Having a separate variable is overkill. But it makes the next line easier to read.
			Register::write(Register::BitFraming, (rxAlign << 4) + txLastBits);	// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]
			
			// Transmit the buffer and receive the response.
			result = transceiveData(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign);
			if (result == STATUS_COLLISION) { // More than one PICC in the field => collision.
				uint8_t valueOfCollReg = Register::read(Register::Coll); // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
				if (valueOfCollReg & 0x20) { // CollPosNotValid
					return STATUS_COLLISION; // Without a valid collision position we cannot continue
				}
				uint8_t collisionPos = valueOfCollReg & 0x1F; // Values 0-31, 0 means bit 32.
				if (collisionPos == 0) {
					collisionPos = 32;
				}
				if (collisionPos <= currentLevelKnownBits) { // No progress - should not happen 
					return STATUS_INTERNAL_ERROR;
				}
				// Choose the PICC with the bit set.
				currentLevelKnownBits = collisionPos;
				count			= (currentLevelKnownBits - 1) % 8; // The bit to modify
				index			= 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First byte is index 0.
				buffer[index]	|= (1 << count);
			}
			else if (result != STATUS_OK) {
				return result;
			}
			else { // STATUS_OK
				if (currentLevelKnownBits >= 32) { // This was a SELECT.
					selectDone = true; // No more anticollision 
					// We continue below outside the while.
				}
				else { // This was an ANTICOLLISION.
					// We now have all 32 bits of the UID in this Cascade Level
					currentLevelKnownBits = 32;
					// Run loop again to do the SELECT.
				}
			}
		} // End of while (!selectDone)
		
		// We do not check the CBB - it was constructed by us above.
		
		// Copy the found UID bytes from buffer[] to uid->uidByte[]
		index			= (buffer[2] == TAG_CMD_CT) ? 3 : 2; // source index in buffer[]
		bytesToCopy		= (buffer[2] == TAG_CMD_CT) ? 3 : 4;
		for (count = 0; count < bytesToCopy; count++) {
			uid->uidByte[uidIndex + count] = buffer[index++];
		}
		
		// Check response SAK (Select Acknowledge)
		if (responseLength != 3 || txLastBits != 0) { // SAK must be exactly 24 bits (1 byte + CRC_A).
			return STATUS_ERROR;
		}
		// Verify CRC_A - do our own calculation and store the control in buffer[2..3] - those bytes are not needed anymore.
		result = calculateCRC(responseBuffer, 1, &buffer[2]);
		if (result != STATUS_OK) {
			return result;
		}
		if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
			return STATUS_CRC_WRONG;
		}
		if (responseBuffer[0] & 0x04) { // Cascade bit set - UID not complete yes
			cascadeLevel++;
		}
		else {
			uidComplete = true;
			uid->sak = responseBuffer[0];
		}
	} // End of while (!uidComplete)
	
	// Set correct uid->size
	uid->size = 3 * cascadeLevel + 1;
	
	return STATUS_OK;	
}
//=================================================================================================
bool MFRC522::Tag::readSerial() {
	bool result = (select(&uid) == STATUS_OK);
	if (result==true){
		readSerialCounter++;
	}
	return result;
}
//=================================================================================================
bool MFRC522::Tag::isDetected(){

	uint8_t bufferATQA[2];
	uint8_t bufferSize = sizeof(bufferATQA);
	StatusCode requestResult = requestA(bufferATQA, &bufferSize);
	
	bool result = (requestResult == STATUS_OK || requestResult == STATUS_COLLISION);
	if (result==true){
		detectionCounter++;
	}	
	return result;
}
//=================================================================================================



