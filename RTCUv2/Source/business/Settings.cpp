
#include "Settings.h"

#include <string.h>

#include "nv/bkpsram.h"
#include "crc32.h"

Settings::Struct *Settings::struct0;//located in BKSRAM
Settings::Struct *Settings::struct1;
uint8_t Settings::actualIndex;

const int32_t Settings::INT32_UNKNOWN_VALUE = -1;	
const float Settings::FLOAT_UNKNOWN_VALUE = -12345678.0f;//TODO: infinity or other distinct value	


//=================================================================================================
void Settings::init(){

	struct0 = (Struct*)(NV::BkpSram::settingsBlock0Addr());
	struct1 = (Struct*)(NV::BkpSram::settingsBlock1Addr());
	
	bool check0 = check(0);
	bool check1 = check(1);

	//check0 = check1 = false;//debug

	if ((check0==false)&&(check1==false)){
		
		initializeStruct(0);
		actualIndex = 0;
		
	} else if (check0==true){
		
		actualIndex = 0;
		
	} else if (check1==true){

		actualIndex = 1;
		
	} else {
		
		if (compareDwords(struct0->index,struct1->index)>0){
			actualIndex = 0;
		} else {
			actualIndex = 1;
		}
		
	}
	
	
	
}
//=================================================================================================
int_fast8_t Settings::compareDwords(uint32_t dw1,uint32_t dw2){
	uint32_t diff = dw1 - dw2;
	if (diff==0){
		return 0;
	} else if (diff>0x7FFFFFFF){
		return -1;
	} else {
		return 1;
	}
}

//=================================================================================================
bool Settings::check(uint8_t structIndex){
	
	
	uint32_t crcCalculated;
	uint32_t crcExpected;
	if (structIndex==0){
		crcCalculated = calculateChecksum(structIndex);
		crcExpected = struct0->checksum;
	} else {
		crcCalculated = calculateChecksum(structIndex);
		crcExpected = struct1->checksum;
	}
	if (crcCalculated==crcExpected){
		return true;
	} else {
		return false;
	}

}
//=================================================================================================
uint32_t Settings::calculateChecksum(uint8_t structIndex){
	
	uint32_t *p = (structIndex==0) ? ((uint32_t*)struct0) : ((uint32_t*)struct1);
	uint32_t structSize = sizeof(Struct)/4-1;
	uint32_t crc = Crc32::getInitValue();
	for(uint32_t i=0;i<structSize;i++){
		Crc32::appendDword(*p++,crc);
	}
	return crc;
}
//=================================================================================================
void Settings::initializeStruct(uint8_t structIndex){
	
	Struct *ss = (structIndex==0) ? (struct0) : (struct1);
		
	ss->index = 0;
	
	ss->positionMainMax = INT32_UNKNOWN_VALUE;
	ss->positionMainMin = INT32_UNKNOWN_VALUE;
	ss->positionAux1Max = INT32_UNKNOWN_VALUE;
	ss->positionAux1Min = INT32_UNKNOWN_VALUE;
	ss->positionAux2Max = INT32_UNKNOWN_VALUE;
	ss->positionAux2Min = INT32_UNKNOWN_VALUE;;
	ss->positionAux3Max = INT32_UNKNOWN_VALUE;
	ss->positionAux3Min = INT32_UNKNOWN_VALUE;;
	ss->positionAux4Max = INT32_UNKNOWN_VALUE;
	ss->positionAux4Min = INT32_UNKNOWN_VALUE;;
	ss->speedAbsMainMax = INT32_UNKNOWN_VALUE;
	ss->speedAbsMainPersonal = INT32_UNKNOWN_VALUE;
	ss->encoderBitCount = INT32_UNKNOWN_VALUE;
	ss->encoderDirection = INT32_UNKNOWN_VALUE;
	ss->encoderOffset = INT32_UNKNOWN_VALUE;
	ss->forceSensorGain = FLOAT_UNKNOWN_VALUE;
	ss->forceSensorOffset0 = FLOAT_UNKNOWN_VALUE;
	ss->forceSensorOffset1 = FLOAT_UNKNOWN_VALUE;
	ss->forceSensorOffset2 = FLOAT_UNKNOWN_VALUE;

	ss->checksum = calculateChecksum(structIndex);
	

	
}
//=================================================================================================
Settings::Struct *Settings::getStruct(){
	return (actualIndex==0) ? (struct0) : (struct1);
}
//=================================================================================================
void Settings::updateStruct(){

	if (actualIndex==0){
		(struct0->checksum) = calculateChecksum(0);
		memcpy(struct1,struct0,sizeof(Struct));
		(struct1->index)++;
		(struct1->checksum) = calculateChecksum(1);
	} else {
		(struct1->checksum) = calculateChecksum(1);
		memcpy(struct0,struct1,sizeof(Struct));
		(struct0->index)++;
		(struct0->checksum) = calculateChecksum(0);
	}
	actualIndex ^= 1;
}
//=================================================================================================




