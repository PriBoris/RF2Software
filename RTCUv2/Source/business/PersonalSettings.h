
#ifndef PERSONAL_SETTINGS_H
#define PERSONAL_SETTINGS_H

#include <stdint.h>

#include "hmi/RxMessageQueue.h"


class PersonalSettings{

public:

	struct ProtocolStruct{

		int32_t	positionMainA;
		int32_t	positionMainB;
		int32_t	positionMainParking;
		int32_t	positionAux1;
		int32_t	positionAux2;
		int32_t	positionAux3;
		int32_t	speedAbsMainParking;
		int32_t	speedRelMainTestEccentric;
		int32_t	speedRelMainTestConcentric;
		int32_t forceCurveAB[21];
		int32_t forceCurveBA[21];
	};	





	static void init();
	static void report();
	static void load(RxMessage *message);

	static bool protocolStructValid;


//private:

	static ProtocolStruct protocolStruct;
	static bool checkProtocolStruct(ProtocolStruct *protocolStruct);


	static int32_t getMainRange();
	static int32_t getMainRangeDegrees();

};



#endif

