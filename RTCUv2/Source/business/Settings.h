
#ifndef SETTINGS_H
#define SETTINGS_H


#include <stdint.h>






//=====================================================================================================

class Settings{
	
public:
	
	struct Struct {
		
		uint32_t index;	
		
		int32_t	positionMainMax;
		int32_t	positionMainMin;
		int32_t	positionAux1Max;
		int32_t	positionAux1Min;
		int32_t	positionAux2Max;
		int32_t	positionAux2Min;
		int32_t	positionAux3Max;
		int32_t	positionAux3Min;
		int32_t	positionAux4Max;
		int32_t	positionAux4Min;
		int32_t	speedAbsMainMax;
		int32_t	speedAbsMainPersonal;
		int32_t	encoderBitCount;
		int32_t	encoderDirection;
		int32_t	encoderOffset;
		int32_t	forceSensorOffset;
		float forceSensorGain;		
		uint32_t checksum;
		
	};	
	
private:

	static Struct *struct0;
	static Struct *struct1;

	static bool check(uint8_t structIndex);
	static void initializeStruct(uint8_t structIndex);
	static uint32_t calculateChecksum(uint8_t structIndex);
	static uint8_t actualIndex;
	static int_fast8_t compareDwords(uint32_t dw1,uint32_t dw2);


public:

	static const int32_t INT32_UNKNOWN_VALUE;	
	static const float FLOAT_UNKNOWN_VALUE;	


	static void init();
	static Struct *getStruct();
	static void updateStruct();

	
};
//=====================================================================================================

//=====================================================================================================

#endif

