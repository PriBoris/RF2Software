
#include "protocol.h"

#include <string.h>

#include "crc32.h"


	

//===============================================================================================
void Protocol::init(
	uint8_t *txBuffer,
	uint32_t txBufferLen,
	uint8_t *rxBuffer,
	uint32_t rxBufferLen
	){
		this->txBuffer = txBuffer;
		this->txBufferLen = txBufferLen;
		txBufferLenMinus1 = txBufferLen-1;
		txPtrPending = txPtrTransmitted = 0;
		//memset(txBuffer,0,txBufferLen);
	
		this->rxBuffer = rxBuffer;
		this->rxBufferLen = rxBufferLen;
		rxBufferLenMinus1 = rxBufferLen-1;
		rxPtrReceived = rxPtrProcessed = 0;
		//memset(rxBuffer,0,rxBufferLen);
	
		stuffState = STUFF_Waiting;
		protocolState = PROTOCOL_Error;

		txID = 0;
	
}

//===============================================================================================
void Protocol::receiveByte(uint8_t byte){
	rxBuffer[rxPtrReceived] = byte;
	rxPtrReceived = (rxPtrReceived+1) & rxBufferLenMinus1;		
}
//===============================================================================================
bool Protocol::processRx(void){

	if (rxPtrReceived!=rxPtrProcessed)
	{
			uint8_t newByte = rxBuffer[rxPtrProcessed];
			rxPtrProcessed = (rxPtrProcessed+1) & (rxBufferLenMinus1);
		
	
			if (newByte==SLIP_END)
			{
				stuffState = STUFF_Waiting;
				protocolState = PROTOCOL_TagExpected;
			}
			else
			{
				switch(stuffState)
				{
					//------------------------------------------------
					default:
						stuffState = STUFF_Waiting;
					//------------------------------------------------
					case STUFF_Waiting:
						if (newByte==SLIP_ESC)
						{
							stuffState = STUFF_EscByte;
						}
						else
						{
							stuffState = STUFF_NewByte;
						}
						break;
					//------------------------------------------------
					case STUFF_EscByte:
						if (newByte==SLIP_ESC_END)
						{
							stuffState = STUFF_NewByte;
							newByte = SLIP_END;
						}
						else if (newByte==SLIP_ESC_ESC)
						{
							stuffState = STUFF_NewByte;
							newByte = SLIP_ESC;
						}
						else
						{
							stuffState = STUFF_Waiting;
							protocolState = PROTOCOL_Error; //stuff error
						}
						break;
					//------------------------------------------------
				}
			}
			
			if (stuffState==STUFF_NewByte)
			{
					stuffState = STUFF_Waiting;
				
					static uint32_t lengthReceived;
					static uint32_t crcExpected;
					static uint32_t crcCalculated;
				
				
				
					switch(protocolState)
					{
						//---protocolState---------------------------------------------------------------
						default:
							protocolState = PROTOCOL_Error;
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_TagExpected:
							crcCalculated = Crc32::getInitValue();
							Crc32::appendByte(newByte,crcCalculated);
							rxTag = newByte;
							protocolState = PROTOCOL_IDByte1Expected;
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_IDByte1Expected:
							Crc32::appendByte(newByte,crcCalculated);
							rxID = (uint32_t)newByte;
							protocolState = PROTOCOL_IDByte2Expected;
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_IDByte2Expected:
							Crc32::appendByte(newByte,crcCalculated);
							rxID |= ((uint32_t)newByte)<<8;
							protocolState = PROTOCOL_IDByte3Expected;
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_IDByte3Expected:
							Crc32::appendByte(newByte,crcCalculated);
							rxID |= ((uint32_t)newByte)<<16;
							protocolState = PROTOCOL_IDByte4Expected;
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_IDByte4Expected:
							Crc32::appendByte(newByte,crcCalculated);
							rxID |= ((uint32_t)newByte)<<24;
							protocolState = PROTOCOL_LenByte1Expected;
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_LenByte1Expected:
							Crc32::appendByte(newByte,crcCalculated);
							rxDataLen = (uint16_t)newByte;
							protocolState = PROTOCOL_LenByte2Expected;
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_LenByte2Expected:
							Crc32::appendByte(newByte,crcCalculated);
							rxDataLen |= ((uint16_t)newByte)<<8;
						
						
							if (rxDataLen>Protocol::MAX_RX_VALUE_LEN)
							{
								protocolState = PROTOCOL_Error;
							}
							else if (rxDataLen==0)
							{
								protocolState = PROTOCOL_CrcByte1Expected;
							}
							else
							{
								lengthReceived=0;
								protocolState = PROTOCOL_ValueByteExpected;
							}
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_ValueByteExpected:
								
							Crc32::appendByte(newByte,crcCalculated);
							rxValue[lengthReceived++] = newByte;
							if (lengthReceived==rxDataLen)
							{
								protocolState = PROTOCOL_CrcByte1Expected;
							}
							
						
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_CrcByte1Expected:
							
							crcExpected = (uint32_t)newByte;
							protocolState = PROTOCOL_CrcByte2Expected;
					
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_CrcByte2Expected:
							crcExpected |= ((uint32_t)newByte)<<8;
							protocolState = PROTOCOL_CrcByte3Expected;
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_CrcByte3Expected:
							crcExpected |= ((uint32_t)newByte)<<16;
							protocolState = PROTOCOL_CrcByte4Expected;
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_CrcByte4Expected:						
							crcExpected |= ((uint32_t)newByte)<<24;
							//protocolState = PROTOCOL_TagExpected;
					
							if (crcExpected==crcCalculated)
							{
								stuffState = STUFF_Waiting;
								protocolState = PROTOCOL_Success;
								return true;
							}else{
									protocolState = PROTOCOL_Error;
							}	
						
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_Success:
							//TODO: add counters for diagnostics (no bytes are normally expected here)
							break;
						//---protocolState---------------------------------------------------------------
						case PROTOCOL_Error:
							//TODO: add counters for diagnostics (no bytes are normally expected here)
							break;
						//---protocolState---------------------------------------------------------------
					}
				
				
			}
			return false;
	}	
	else
	{
			return false;		
	}

	
	
		
}
//===============================================================================================
void Protocol::getRxValue(uint8_t *dest){
	memcpy(dest,rxValue,rxDataLen);
}
uint8_t Protocol::getRxTag(){
		return rxTag;
}
uint32_t Protocol::getRxID(){
		return rxID;
}
uint16_t Protocol::getRxDataLen(){
		return rxDataLen;
}
//===============================================================================================
bool Protocol::bytesPending(){
		if (txPtrPending==txPtrTransmitted)		{
			return false;
		}else{
			return true;
		}
}
uint8_t Protocol::popTxByte(){
		uint8_t result = txBuffer[txPtrTransmitted];
		txPtrTransmitted = (txPtrTransmitted+1) & (txBufferLenMinus1);	
		return result;
}

//===============================================================================================
void Protocol::pushByte(uint8_t byte,bool pushStuffed,uint32_t *crc){
	if (crc!=0){
		Crc32::appendByte(byte,*crc);
	}
	
	if (pushStuffed==true){
		
		if (byte==SLIP_END){
			
				pushByte(SLIP_ESC,false,0);
				pushByte(SLIP_ESC_END,false,0);
		}else if (byte==SLIP_ESC){
			
				pushByte(SLIP_ESC,false,0);
				pushByte(SLIP_ESC_ESC,false,0);
		}else{
			
				pushByte(byte,false,0);
		}	
	}else{
		
		txBuffer[txPtrPending] = byte;
		txPtrPending = (txPtrPending+1) & txBufferLenMinus1;		
	}
}
void Protocol::pushWord(uint16_t word,bool pushStuffed,uint32_t *crc){
	uint8_t *pb = (uint8_t*)&word;
	for(uint8_t i=0;i<2;i++){
		pushByte(*pb++,pushStuffed,crc);
	}
}
void Protocol::pushDword(uint32_t dword,bool pushStuffed,uint32_t *crc){
	uint8_t *pb = (uint8_t*)&dword;
	for(uint8_t i=0;i<4;i++){
		pushByte(*pb++,pushStuffed,crc);
	}
}
//===============================================================================================
void Protocol::sendPacket(uint8_t tag,uint8_t *data,uint32_t dataLen){
	uint32_t crc = Crc32::getInitValue();
	
	pushByte(SLIP_END,false,0);
	pushByte(SLIP_END,false,0);
	pushByte(SLIP_END,false,0);
	pushByte(SLIP_END,false,0);
	
	pushByte(tag,true,&crc);
	pushDword(txID++,true,&crc);
	pushWord(dataLen,true,&crc);
	for(uint16_t i=0;i<dataLen;i++){
		pushByte(data[i],true,&crc);
	}
	pushDword(crc,true,0);
	
}
//===============================================================================================



