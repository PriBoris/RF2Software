
#ifndef ODOMETER_H
#define ODOMETER_H

#include <stdint.h>


class Odometer{
	
public:
	
	struct Struct {
		
		uint32_t	timeSeconds;
		uint32_t	distanceMillimetersDegrees;
		
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
	static void updateStruct();

public:
	


	static void init();
	static uint32_t getTimeMinutes();
	static uint32_t getDistanceMetresRotations();
	static void incrementSeconds();

	
};





#endif
