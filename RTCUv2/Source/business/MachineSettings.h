
#ifndef MACHINE_SETTINGS_H
#define MACHINE_SETTINGS_H

#include <stdint.h>

#include "hmi/RxMessageQueue.h"

class MachineSettings{


public:

	struct ProtocolStruct
	{
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
	};


	static void init();
	static void report();
	static void load(RxMessage *message);
	static bool protocolStructValid;

//private:

	static ProtocolStruct protocolStruct;

	static bool checkProtocolStruct(ProtocolStruct *protocolStruct);

	static int32_t getMainRange();


	//static float absSpeedToServoFrequency(int32_t absSpeed);

};



#endif
