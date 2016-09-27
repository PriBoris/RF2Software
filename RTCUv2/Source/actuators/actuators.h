

#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <stdint.h>


class Actuators{
private:

	static const uint_fast8_t actuatorCount = 2;


	static const uint_fast8_t filterLength = 16;
	static uint16_t feedbackFilter[actuatorCount][filterLength];
	static uint32_t feedbackDataCount;
	static uint8_t feedbackChannel;
	static bool feedbackInitialized;
	static bool feedbackValueReadyFlag;
	static const int16_t FEEDBACK_VALUE_MAX = (4095-50);
	static const int16_t FEEDBACK_VALUE_MIN = (0+50);

	static int32_t feedbackValue[actuatorCount];


	static bool enabled[actuatorCount];
	static int32_t targetPosition[actuatorCount];
	static int32_t maxPosition[actuatorCount];
	static int32_t minPosition[actuatorCount];

	static const int32_t FEEDBACK_VALUE_HYSTERESIS = 50; 


	
	static void bridgeAssert(uint_fast8_t actuatorIndex,bool extend);
	static void bridgeDeassert(uint_fast8_t actuatorIndex);
	static void bridgeBrake(uint_fast8_t actuatorIndex);
	
	

	enum Action{
		EXTEND,
		CONTRACT,
		HOLD
	};
	static Action actions[actuatorCount];

	static uint_fast8_t timeSlots[actuatorCount]; 
	static const uint_fast8_t TIME_SLOT_MAX_EXTEND[actuatorCount];
	static const uint_fast8_t TIME_SLOT_MAX_CONTRACT[actuatorCount];
	static const uint_fast8_t TIME_SLOT_MAX_HOLD;


public:	
	static void feedbackInit();
	static void feedbackStartAdc();
	static void feedbackReadAdc();
	static bool feedbackValueReady();

	static void control();

	static int32_t getPosition(uint_fast8_t actuatorIndex);

	static void controlInit();

	static void enable(uint_fast8_t actuatorIndex,int32_t targetPosition,int32_t minPosition,int32_t maxPosition);
	static void disable(uint_fast8_t actuatorIndex);
	static bool targetPositionReached(uint_fast8_t actuatorIndex);
	static void emergencyStop();
	
};






#endif
