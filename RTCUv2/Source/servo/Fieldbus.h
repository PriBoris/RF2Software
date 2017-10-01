
#ifndef FIELDBUS_H
#define FIELDBUS_H

#include <stdint.h>

#include "USS.h"

class Fieldbus{

private:

	/*static void readEnable();
	static void readDisable();
	static void transmitEnable();
	static void transmitDisable();*/

	static void receiver();
	static void transmitter();



	static const uint32_t RX_BUFFER_LENGTH = 2048;
	static const uint32_t TX_BUFFER_LENGTH = 2048;

	static uint8_t rxBuffer[RX_BUFFER_LENGTH];
	static uint8_t txBuffer[TX_BUFFER_LENGTH];

	static uint32_t pendingPtr;
	static uint32_t transmittedPtr;
	static uint32_t receivedPtr;
	static uint32_t processedPtr;

	static bool transmitting;

	static void pushByte(uint8_t byte);
	static void pushBytes(uint8_t* buf,uint32_t bufLen);

	static uint8_t indicationCounter;
	static const uint8_t indicationCounterMax = 50;

	enum RxState{
		STX_EXPECTED=0,
		BYTE_COUNT_EXPECTED,
		ADDRESS_EXPECTED,
		PKE_HI_EXPECTED,
		PKE_LO_EXPECTED,
		IND_HI_EXPECTED,
		IND_LO_EXPECTED,
		PWE_HI_EXPECTED,
		PWE_LO_EXPECTED,
		STW_HI_EXPECTED,
		STW_LO_EXPECTED,
		SW1_HI_EXPECTED,
		SW1_LO_EXPECTED,
		BCC_EXPECTED
	};
	static RxState rxState;
	static uint8_t rxBCCCalculated;
	static uint16_t rxPKEValue;
	static uint16_t rxPWEValue;
	static uint16_t rxINDValue;

	static uint32_t rxGoodPacketCounter;
	static uint32_t rxBadPacketCounter;


	static bool responseReceived;
	
	static uint32_t errorCounter;
	static uint32_t errorCounterMax;	
	static const uint32_t FAULT_TRESHOLD = 4;

public:

	static void init();
	static void irqHandler();

	static void pushUSSRequest(USS::Request *request);

	static void processRx();
	static void indication();

	static bool checkSetFrequencyResponse(bool direction,float frequencyValueHertz);

	static bool responseIsValid();


	static void check();
};





#endif


