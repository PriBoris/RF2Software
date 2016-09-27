
#include "debugConsole.h"

#include <string.h>

#include "hmi/diagnostics.h"


bool DebugConsole::enabled = false;

uint8_t DebugConsole::txBuffer[DebugConsole::TX_BUFFER_LENGTH];
uint32_t DebugConsole::ptrPending;
uint32_t DebugConsole::ptrTransmitted;

uint8_t DebugConsole::txMessage[DebugConsole::MAX_MESSAGE_LENGTH];


//=================================================================================================
void DebugConsole::init(bool enabled){

	DebugConsole::enabled = enabled;

	ptrPending = 0;
	ptrTransmitted = 0;
	memset(txBuffer,0,sizeof(txBuffer));

}
//=================================================================================================
void DebugConsole::process(){

	if (enabled==true){

		uint32_t messageLength = (ptrPending-ptrTransmitted)&(TX_BUFFER_LENGTH-1);
		if (messageLength>MAX_MESSAGE_LENGTH){
			messageLength = MAX_MESSAGE_LENGTH;
		}
		if (messageLength!=0){

			for(uint32_t i=0;i<messageLength;i++){
				txMessage[i] = txBuffer[ptrTransmitted];
				ptrTransmitted = (ptrTransmitted+1)&(TX_BUFFER_LENGTH-1);
			}
			Diagnostics::protocol.sendPacket(Protocol::TAG_RtcuDebugMessage,txMessage,messageLength);
		}

	}else{
	}

}
//=================================================================================================
void DebugConsole::pushMessage(char *msg){

	uint32_t charCount;
	const uint32_t MAX_CHAR_COUNT = 100;
	for(charCount=0;charCount<MAX_CHAR_COUNT;charCount++){

		if (msg[charCount]==0){
			break;
		}
	}
	if (charCount!=0){
		for(uint32_t c=0;c<charCount;c++){
			txBuffer[ptrPending] = msg[c];
			ptrPending = (ptrPending+1)&(TX_BUFFER_LENGTH-1);
		}
	}

}
//=================================================================================================



