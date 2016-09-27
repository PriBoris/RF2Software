
#ifndef ERRORS_H
#define ERRORS_H

#include <stdint.h>


class Errors{

private:

	static uint32_t allFlags;


	static const uint8_t ERROR_ServoInternal	= 0x00;
	static const uint8_t ERROR_ServoConnectionLost	= 0x01;
	static const uint8_t ERROR_ServoVoltageLost	= 0x02;
	static const uint8_t ERROR_ServoControlLost	= 0x03;
	static const uint8_t ERROR_RtcuLogicError	= 0x04;
	static const uint8_t ERROR_AuxMoved	= 0x05;
	static const uint8_t ERROR_HmiParametersError	= 0x06;
	static const uint8_t ERROR_StrainGaugeLost 	= 0x07;
	static const uint8_t ERROR_StoppedManually	= 0x08;
	static const uint8_t ERROR_PositionMainSensorLost	= 0x09;
	static const uint8_t ERROR_RfidReaderLost	= 0x0A;

	static const uint8_t ERROR_Unknown	= 0xFF;





public:

	static void init();

	static const uint32_t FLAG_ENCODER = 0;
	static const uint32_t FLAG_USS_RESPONSE = 1;
	static const uint32_t FLAG_EMERGENCY_STOP = 2;
	static const uint32_t FLAG_RFID = 3;


	static void setFlag(uint32_t flag);

	static bool asserted();

	static uint8_t getErrorCode();

};


#endif


