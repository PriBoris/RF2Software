
#ifndef MACHINE_SETTINGS_H
#define MACHINE_SETTINGS_H

#include <stdint.h>

#include "hmi/RxMessageQueue.h"

class MachineSettings{


public:

	static const uint32_t ProtocolStructSize = 40;
	struct ProtocolStructExtended{
		
		int32_t	positionMainMax;
		int32_t	positionMainMin;
		int32_t	positionAux1Max;
		int32_t	positionAux1Min;
		int32_t	positionAux2Max;
		int32_t	positionAux2Min;
		int32_t	positionAux3Max;
		int32_t	positionAux3Min;
		int32_t speedAbsMainMax;
		int32_t speedAbsMainPersonal;
		//end of old ProtocolStruct, start of ProtocolStructExtended
		int32_t	encoderBitCount;
		int32_t	encoderDirection;
		int32_t	encoderOffset;
		int32_t forceSensorDirection;
		int32_t	forceSensorOffset;
		float forceSensorGain;

	};



	static void init();
	static void report();
	static void reportExtended();
	static void load(RxMessage *message);
	static bool protocolStructExtendedValid;


	static ProtocolStructExtended protocolStructExtended;

	static bool checkProtocolStructExtended(
		ProtocolStructExtended *protocolStructExtended
		);

	static int32_t getMainRange();


	//static float absSpeedToServoFrequency(int32_t absSpeed);

};



#endif
