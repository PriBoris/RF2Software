

#include <string.h>

#include "stm32f4xx_conf.h"

#include "system/heartbeat.h"
#include "system/ports.h"
#include "actuators/actuators.h"
#include "servo/servo.h"
#include "servo/encoder.h"
#include "servo/StrainGauge.h"
#include "servo/Fieldbus.h"
#include "servo/USS.h"


#include "crc32.h"
#include "hmi/hmi.h"
#include "hmi/diagnostics.h"
#include "hmi/RxMessageQueue.h"
#include "hmi/debugConsole.h"


#include "nfc/nfc.h"
#include "nv/eeprom.h"
#include "nv/rtc.h"
#include "nv/bkpsram.h"

#include "business/Settings.h"
#include "business/Odometer.h"
#include "business/MainTick.h"
#include "business/Errors.h"
#include "business/MachineSettings.h"
#include "business/PersonalSettings.h"
#include "business/ExcerciseSettings.h"
#include "business/GenericSetSettings.h"




int main(void){

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	Heartbeat::init();
	
	Crc32::init();
	
	ports::init();
	ports::led1on();
	

	DebugConsole::init(true);


	NV::RealTimeClock::init();
	NV::BkpSram::init();
	Settings::init();
	Odometer::init();
	
	Actuators::feedbackInit();
	Actuators::controlInit();
	
	Servo::parkingBrake(true);//TODO: make Servo::init()
	Servo::brake();


	StrainGauge::init();
	Encoder::init();
	Fieldbus::init();
	USS::init();
	

	HMI::init();
	Diagnostics::init();
	RxMessageQueue::init();
	
	NFC::init();
	MainTick::init();
	Errors::init();

	MachineSettings::init();
	PersonalSettings::init();
	ExcerciseSettings::init();
	GenericSetSettings::init();

	Heartbeat::startMainTick();

	
	while(true){
		
		//------------------------------------------------------------------------------------------
		HMI::process();	
		Diagnostics::process();
		NFC::processTx();

		Fieldbus::processRx();
		//------------------------------------------------------------------------------------------
		if (Heartbeat::mainTick()){
			
			MainTick::process();
			
			NV::RealTimeClock::poll();
		}
		//------------------------------------------------------------------------------------------
		if (Actuators::feedbackValueReady()!=false){
			Actuators::control();
		}
		//------------------------------------------------------------------------------------------
		
	}
	
}




