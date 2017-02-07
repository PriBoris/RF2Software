
#include "Parking.h"


#include "business/PersonalSettings.h"
#include "business/MachineSettings.h"

#include "servo/servo.h"
#include "servo/encoder.h"


float Parking::servoFrequencyPositive = 0.0f;
float Parking::servoFrequencyNegative = 0.0f;


//=================================================================================================
void Parking::recalculateServoFrequency(){

	servoFrequencyPositive = Servo::rangeToFrequency(
		MachineSettings::getMainRange(),
		PersonalSettings::protocolStruct.speedAbsMainParking
		);
	servoFrequencyNegative = -(servoFrequencyPositive);
}
//=================================================================================================


