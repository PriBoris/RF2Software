
#include "Odometer.h"

#include <string.h>

#include "nv/bkpsram.h"
#include "crc32.h"


Odometer::Struct *Odometer::struct0;
Odometer::Struct *Odometer::struct1;
uint8_t Odometer::actualIndex;


void Odometer::init(){

	struct0 = (Struct*)(NV::BkpSram::odometerBlock0Addr());
	struct1 = (Struct*)(NV::BkpSram::odometerBlock1Addr());
	
	bool check0 = check(0);
	bool check1 = check(1);

	if ((check0==false)&&(check1==false)){
		
		initializeStruct(0);
		actualIndex = 0;
		
	} else if (check1==false){
		
		actualIndex = 0;
		
	} else if (check0==false){

		actualIndex = 1;
		
	} else {
		
		if (compareDwords(struct0->timeSeconds,struct1->timeSeconds)>0){
			actualIndex = 0;
		} else {
			actualIndex = 1;
		}
		
	}

}

void Odometer::initializeStruct(uint8_t structIndex){
	
	Struct *ss = (structIndex==0) ? (struct0) : (struct1);
	ss->timeSeconds = 0;
	ss->distanceMillimetersDegrees = 0;
	ss->checksum = calculateChecksum(structIndex);
	

	
}


int_fast8_t Odometer::compareDwords(uint32_t dw1,uint32_t dw2){
	uint32_t diff = dw1 - dw2;
	if (diff==0){
		return 0;
	} else if (diff>0x7FFFFFFF){
		return -1;
	} else {
		return 1;
	}
}


bool Odometer::check(uint8_t structIndex){
	
	
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

uint32_t Odometer::calculateChecksum(uint8_t structIndex){
	
	uint32_t *p = (structIndex==0) ? ((uint32_t*)struct0) : ((uint32_t*)struct1);
	uint32_t structSize = sizeof(Struct)/4-1;
	uint32_t crc = Crc32::getInitValue();
	for(uint32_t i=0;i<structSize;i++){
		Crc32::appendDword(*p++,crc);
	}
	return crc;
}

uint32_t Odometer::getTimeMinutes(){

	return (actualIndex==0) ? (struct0->timeSeconds/60) : (struct1->timeSeconds/60);

}

uint32_t Odometer::getDistanceRotations(){

	return (actualIndex==0) ? (struct0->distanceMillimetersDegrees/360) : (struct1->distanceMillimetersDegrees/360);


}

void Odometer::incrementSeconds(){

	if (actualIndex==0){
		(struct0->timeSeconds)++;
	} else {
		(struct1->timeSeconds)++;
	}
	updateStruct();
}

void Odometer::updateStruct(){

	if (actualIndex==0){
		(struct0->checksum) = calculateChecksum(0);
		memcpy(struct1,struct0,sizeof(Struct));
	} else {
		(struct1->checksum) = calculateChecksum(1);
		memcpy(struct0,struct1,sizeof(Struct));
	}
	actualIndex ^= 1;
}

void Odometer::incrementDegrees(uint32_t degrees){

	if (actualIndex==0){
		(struct0->distanceMillimetersDegrees)+=degrees;
	} else {
		(struct1->distanceMillimetersDegrees)+=degrees;
	}
	updateStruct();	
	
}


