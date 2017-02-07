
#include "MainTick.h"

#include <string.h>

#include "business/Odometer.h"
#include "business/Errors.h"
#include "business/EmergencyStop.h"
#include "business/MachineSettings.h"
#include "business/PersonalSettings.h"
#include "business/ExcerciseSettings.h"
#include "business/GenericSetSettings.h"
#include "business/RangeAdjustment.h"
#include "business/Parking.h"
#include "business/ForceTestDynamic.h"
#include "business/ForceTestStatic.h"
#include "business/PositionTask.h"
#include "business/Excercise.h"
#include "business/GenericSet.h"

#include "hmi/diagnostics.h"
#include "hmi/hmi.h"
#include "hmi/debugConsole.h"
#include "servo/servo.h"
#include "servo/Fieldbus.h"
#include "hmi/hmi.h"
#include "hmi/RxMessageQueue.h"
#include "nfc/nfc.h"

#include "actuators/actuators.h"
#include "system/heartbeat.h"


MainTick::Mode MainTick::mode;
MainTick::Mode MainTick::modePrev;
MainTick::Submode MainTick::submode;
MainTick::Submode MainTick::submodePrev;

uint_fast8_t MainTick::rangeAdjustmentTimeoutCounter;

uint32_t MainTick::profilerStartTimestamp = 0;
uint32_t MainTick::profilerLastValue = 0;
uint32_t MainTick::profilerMaxValue = 0;
MainTick::Submode MainTick::profilerMaxValueSubmode = FAULT_Fault;
MainTick::Submode MainTick::profilerStartSubmode = FAULT_Fault;

uint32_t MainTick::tickID;

float MainTick::servoFrequencyPositive = 0.0f;
float MainTick::servoFrequencyNegative = 0.0f;

uint32_t MainTick::fieldbusErrorCounter;
uint32_t MainTick::fieldbusErrorCounterMax;



//==================================================================================================================
void MainTick::init(){

	setSubmode(INITIALIZING_JustStarted);
	modePrev = mode;
	submodePrev = submode;
	tickID = 0;

	fieldbusErrorCounter = 0;
	fieldbusErrorCounterMax = 0;

}
//==================================================================================================================
void MainTick::process(){ //called every 100ms

	modePrev = mode;
	submodePrev = submode;

	profilerStart();

	if (EmergencyStop::pressed()){

		Servo::brake(true);
		Servo::movePositive(false);
		Servo::moveNegative(false);
		Actuators::emergencyStop();
		
		Errors::setFlag(Errors::FLAG_EMERGENCY_STOP);
		Servo::parkingBrake(true);
		DebugConsole::pushMessage(" #EmergencyStop\0");
		
	}

	{
		for(uint32_t rx=0;rx<64;rx++){
			if (NFC::protocol.processRx()==true){
				
				if (
					(NFC::protocol.rxTag==Protocol::TAG_CheckRfidProximity)&&
					(NFC::protocol.rxDataLen!=0)
				){
					Diagnostics::protocol.sendPacket(Protocol::TAG_RfidProximity,NFC::protocol.rxValue,NFC::protocol.rxDataLen);
					HMI::protocol.sendPacket(Protocol::TAG_RfidProximity,NFC::protocol.rxValue,NFC::protocol.rxDataLen);				
					
				}
				break;
				
			}else{
				
			}
			
		}
		

		NFC::protocol.sendPacket(Protocol::TAG_CheckRfidProximity,NULL,0);
	}

	if (Errors::asserted()){
		setSubmode(FAULT_Fault);
	}



	reportCurrentMode();

	switch(submode){
	//--------------------------------------------------------INITIALIZING---------------------------
	case INITIALIZING_JustStarted:

		DebugConsole::pushMessage(" #Started ===========================\0");

		RxMessageQueue::flush();

		HMI::powerOff();
		setSubmode(INITIALIZING_HMIPoweringOff);



		

		break;
	//--------------------------------------------------------INITIALIZING---------------------------
	case INITIALIZING_HMIPoweringOff:

		RxMessageQueue::flush();


		{
			static uint_fast8_t pauseCounter = 0;
			pauseCounter++;
			if (pauseCounter==PAUSE_HMIPoweringOff){
				pauseCounter=0;
				setSubmode(INITIALIZING_HMIPoweringOn);
				HMI::powerOn();
			}
		}


		break;
	//--------------------------------------------------------INITIALIZING---------------------------
	case INITIALIZING_HMIPoweringOn:

		RxMessageQueue::flush();


		{
			static uint_fast8_t pauseCounter = 0;
			pauseCounter++;
			if (pauseCounter==PAUSE_HMIPoweringOn){
				pauseCounter=0;
				setSubmode(INITIALIZING_HMIStarting);
				HMI::powerKeyAssert();
			}
		}

		break;
	//--------------------------------------------------------INITIALIZING---------------------------
	case INITIALIZING_HMIStarting:

		RxMessageQueue::flush();

		{
			static uint_fast8_t pauseCounter = 0;
			pauseCounter++;
			if (pauseCounter==PAUSE_HMIStarting){
				pauseCounter=0;
				setSubmode(INITIALIZING_WaitingStableServoConnection);
				HMI::powerKeyDeassert();
				Fieldbus::pushUSSRequest(USS::makeInverterReadyRequest());
			}
		}


		break;
	//--------------------------------------------------------INITIALIZING---------------------------
	case INITIALIZING_WaitingStableServoConnection:

		RxMessageQueue::flush();

		{
			static uint8_t servoResponseCounter = 0;
			if (Fieldbus::responseIsValid()==true){
				servoResponseCounter++;
				if (servoResponseCounter==30){
					servoResponseCounter = 0;

					//TODO: check encoder and strain gauge existence
					setSubmode(IDLE_Idle);
				}else{
				}
			}else{
			}
		}
		Fieldbus::pushUSSRequest(USS::makeInverterReadyRequest());

		break;
	//------------------------------------------------------------IDLE------------------------
	case IDLE_Idle:

		{
			RxMessage *message = RxMessageQueue::pop();
			if (message!=NULL){
				if (
					(message->tag==Protocol::TAG_EnableServo)&&
					(message->valueLen==0)
					){
					setSubmode(WAITING_Waiting);
					Servo::parkingBrake(false);
					Servo::brake();
					DebugConsole::pushMessage(" #Servo enabled\0");


				}else{
					__asm("	nop");
				}
			}
		}


		processFieldbus();


		break;
	//--------------------------------------------------WAITING----------------------------------
	case WAITING_Waiting:

		{
			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			//TODO: aux

		}



		{
			static uint_fast8_t reportSettingsCounter = 0;
			if ((++reportSettingsCounter)==PAUSE_ReportSettings){
				MachineSettings::report();
				PersonalSettings::report();
				ExcerciseSettings::report();
				GenericSetSettings::report();
				reportSettingsCounter = 0;
			}
		}

		{
			RxMessage *message = RxMessageQueue::pop();
			if (message!=NULL){
				switch(message->tag){
				case Protocol::TAG_LoadPersonalSettings:
					PersonalSettings::load(message);
					break;
				case Protocol::TAG_LoadExcerciseSettings:
					ExcerciseSettings::load(message);
					break;
				case Protocol::TAG_LoadGenericSetSettings:
					GenericSetSettings::load(message);
					break;
				case Protocol::TAG_LoadMachineSettingsExtended:
					MachineSettings::load(message);
					break;
				case Protocol::TAG_Personal:
					if (MachineSettings::protocolStructExtendedValid==true){
						setSubmode(PERSONAL_Starting);//TODO:check message length
						DebugConsole::pushMessage(" #Personal\0");
					}
					break;
				case Protocol::TAG_Parking: 
					if (
						(MachineSettings::protocolStructExtendedValid==true)&&
						(PersonalSettings::protocolStructValid==true)
					){
						setSubmode(PARKING_Starting);//TODO:check message length
						DebugConsole::pushMessage(" #Parking\0");
					}
					break;
				case Protocol::TAG_TestConcentric:
				case Protocol::TAG_TestEccentric:

					if (
						(MachineSettings::protocolStructExtendedValid==true)&&
						(PersonalSettings::protocolStructValid==true)&&
						(true==ForceTestDynamic::load(message))
					){
						setSubmode(FTEST_DYNAMIC_Starting);
						DebugConsole::pushMessage(" #ForceTestDynamic\0");
					}
					break;


				case Protocol::TAG_TestStatic:

					if (
						(MachineSettings::protocolStructExtendedValid==true)&&
						(PersonalSettings::protocolStructValid==true)&&
						(true==ForceTestStatic::load(message))
					){
						setSubmode(FTEST_STATIC_Starting);
						DebugConsole::pushMessage(" #ForceTestStatic\0");
					}
					break;
				
				case Protocol::TAG_ExcerciseIsokinetic:

					if (
						(MachineSettings::protocolStructExtendedValid==true)&&
						(PersonalSettings::protocolStructValid==true)&&
						(ExcerciseSettings::valid==true)
					){
						setSubmode(EXERCISE_Starting);
						DebugConsole::pushMessage(" #ExcerciseIsokinetic\0");
					}
					break;
				case Protocol::TAG_GenericSet:
					if (
						(MachineSettings::protocolStructExtendedValid==true)&&
						(PersonalSettings::protocolStructValid==true)&&
						(GenericSetSettings::valid==true)
					){
						setSubmode(GENERIC_SET_Starting);
						DebugConsole::pushMessage(" #GenericSet\0");
					}
					break;
				}
			}else{

			}			
		}

		processFieldbus();

		break;
	//--------------------------------------------------PERSONAL----------------------------------
	//--------------------------------------------------PERSONAL----------------------------------
	case PERSONAL_Starting:	

		RxMessageQueue::flush();
		setSubmode(PERSONAL_SettingPositiveSpeed);

		processFieldbus();

		break;
	//--------------------------------------------------PERSONAL----------------------------------
	case PERSONAL_SettingPositiveSpeed:

		RangeAdjustment::servoFrequency = Servo::rangeToFrequency(
			MachineSettings::getMainRange(),
			MachineSettings::protocolStructExtended.speedAbsMainPersonal
			);

		RxMessageQueue::flush();
		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(true,RangeAdjustment::servoFrequency)==true){
				setSubmode(PERSONAL_SettingNegativeSpeed);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::POSITIVE_DIRECTION,servoFrequencyPositive=RangeAdjustment::servoFrequency));

		break;
	//--------------------------------------------------PERSONAL----------------------------------
	case PERSONAL_SettingNegativeSpeed:

		RxMessageQueue::flush();
		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(false,-RangeAdjustment::servoFrequency)==true){
				setSubmode(PERSONAL_Waiting);
			}else{
				//TODO: countdown is necessary here
			}
		}

		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::NEGATIVE_DIRECTION,servoFrequencyNegative=-RangeAdjustment::servoFrequency));

		break;
	//--------------------------------------------------PERSONAL----------------------------------
	case PERSONAL_Waiting:

		Servo::brake(true);
		Servo::movePositive(false);
		Servo::moveNegative(false);


		{
			RxMessage *message = RxMessageQueue::pop();
			if (message!=NULL){
				switch(message->tag){
				case Protocol::TAG_PersonalExit:
					setSubmode(WAITING_Waiting);
					break;
				case Protocol::TAG_PersonalButtonPressed:

					RangeAdjustment::actualButtonID = message->value[0];//TODO: check message length
					switch(RangeAdjustment::actualButtonID){
					case RangeAdjustment::BUTTONID_MAIN_MINUS:
						if (true==Servo::validateActualPosition(Servo::NEGATIVE_DIRECTION)){

							Servo::movePositive(false);
							Servo::moveNegative(true);
							Servo::brake(false);
							setSubmode(PERSONAL_MovingMain);
							reportServoModeNegative();

							rangeAdjustmentTimeoutCounter = TIMEOUT_RangeAdjusmentMessage;
						}else{
						}
						break;
					case RangeAdjustment::BUTTONID_MAIN_PLUS:

						if (true==Servo::validateActualPosition(Servo::POSITIVE_DIRECTION)){

							Servo::movePositive(true);
							Servo::moveNegative(false);
							Servo::brake(false);
							setSubmode(PERSONAL_MovingMain);
							reportServoModePositive();

							rangeAdjustmentTimeoutCounter = TIMEOUT_RangeAdjusmentMessage;
						}else{
						}
						break;
					case RangeAdjustment::BUTTONID_AUX1_MINUS:

						Actuators::enable(
							0,
							MachineSettings::protocolStructExtended.positionAux1Max,
							MachineSettings::protocolStructExtended.positionAux1Min,
							MachineSettings::protocolStructExtended.positionAux1Max
						);	
						setSubmode(PERSONAL_MovingAux);
						break;
					case RangeAdjustment::BUTTONID_AUX1_PLUS:

						Actuators::enable(
							0,
							MachineSettings::protocolStructExtended.positionAux1Min,
							MachineSettings::protocolStructExtended.positionAux1Min,
							MachineSettings::protocolStructExtended.positionAux1Max
						);	
						setSubmode(PERSONAL_MovingAux);
						break;
					case RangeAdjustment::BUTTONID_AUX2_MINUS:

						Actuators::enable(
							1,
							MachineSettings::protocolStructExtended.positionAux2Max,
							MachineSettings::protocolStructExtended.positionAux2Min,
							MachineSettings::protocolStructExtended.positionAux2Max
						);	
						setSubmode(PERSONAL_MovingAux);
						break;
					case RangeAdjustment::BUTTONID_AUX2_PLUS:

						Actuators::enable(
							1,
							MachineSettings::protocolStructExtended.positionAux2Min,
							MachineSettings::protocolStructExtended.positionAux2Min,
							MachineSettings::protocolStructExtended.positionAux2Max
						);	
						setSubmode(PERSONAL_MovingAux);
						break;
					}
					break;
				}
			}			
		}


		processFieldbus();

		break;
	//--------------------------------------------------PERSONAL----------------------------------
	case PERSONAL_MovingMain:

		if (
			(RangeAdjustment::BUTTONID_MAIN_PLUS==RangeAdjustment::actualButtonID)&&
			(false==Servo::validateActualPosition(Servo::POSITIVE_DIRECTION))
		){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(PERSONAL_Waiting);
			reportServoModeStop();

		}else if (
			(RangeAdjustment::BUTTONID_MAIN_MINUS==RangeAdjustment::actualButtonID)&&
			(false==Servo::validateActualPosition(Servo::NEGATIVE_DIRECTION))
		){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(PERSONAL_Waiting);
			reportServoModeStop();

		}else if ((rangeAdjustmentTimeoutCounter--)==0){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(PERSONAL_Waiting);
			reportServoModeStop();

		}else{

			RxMessage *message = RxMessageQueue::pop();
			if (message!=NULL){
				switch(message->tag){
				case Protocol::TAG_PersonalExit:

					Servo::brake(true);
					Servo::movePositive(false);
					Servo::moveNegative(false);
					setSubmode(WAITING_Waiting);
					reportServoModeStop();

					break;
				case Protocol::TAG_PersonalButtonHold:

					if (RangeAdjustment::actualButtonID==message->value[0]){//TODO: check message length

						rangeAdjustmentTimeoutCounter = TIMEOUT_RangeAdjusmentMessage;
						//keep moving
						reportServoModeContinue();

					}else{
						Servo::brake(true);
						Servo::movePositive(false);
						Servo::moveNegative(false);
						setSubmode(PERSONAL_Waiting);
						reportServoModeStop();
					}
					break;

				case Protocol::TAG_PersonalButtonReleased:
				
					Servo::brake(true);
					Servo::movePositive(false);
					Servo::moveNegative(false);
					setSubmode(PERSONAL_Waiting);
					reportServoModeStop();

				}
			}else{
				reportServoModeContinue();				
			}			
		}

		processFieldbus();
		break;

	//--------------------------------------------------PERSONAL----------------------------------
	case PERSONAL_MovingAux:

		if ((rangeAdjustmentTimeoutCounter--)==0){

			Actuators::disable(0);
			Actuators::disable(1);
			setSubmode(PERSONAL_Waiting);

		}else{

			RxMessage *message = RxMessageQueue::pop();
			if (message!=NULL){
				switch(message->tag){
				case Protocol::TAG_PersonalExit:
					Actuators::disable(0);
					setSubmode(WAITING_Waiting);
					break;
				case Protocol::TAG_PersonalButtonHold:

					if (RangeAdjustment::actualButtonID==message->value[0]){//TODO: check message length

						//keep moving
						rangeAdjustmentTimeoutCounter = TIMEOUT_RangeAdjusmentMessage;

					}else{
						Actuators::disable(0);
						Actuators::disable(1);
						setSubmode(PERSONAL_Waiting);
					}
					break;

				case Protocol::TAG_PersonalButtonReleased:
				
					Actuators::disable(0);
					Actuators::disable(1);
					setSubmode(PERSONAL_Waiting);

				}
			}	
		}


		processFieldbus();
		break;
	//-------------------------------------------------PARKING-----------------------------------
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_Starting:

		Parking::recalculateServoFrequency();

		//MessageQueue::flush();
		setSubmode(PARKING_SettingPositiveSpeed);

		processFieldbus();

		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_SettingPositiveSpeed:

		//MessageQueue::flush();
		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(true,Parking::servoFrequencyPositive)==true){
				setSubmode(PARKING_SettingNegativeSpeed);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::POSITIVE_DIRECTION,servoFrequencyPositive=Parking::servoFrequencyPositive));


		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_SettingNegativeSpeed:

		//MessageQueue::flush();
		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(false,Parking::servoFrequencyNegative)==true){
				setSubmode(PARKING_PreparingMain);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::NEGATIVE_DIRECTION,servoFrequencyNegative=Parking::servoFrequencyNegative));


		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_PreparingMain:

		//MessageQueue::flush();
		{
			bool positionTaskIsNotNeeded = 
				PositionTask::checkPositionStatically(
					PersonalSettings::protocolStruct.positionMainParking
					);

			if (positionTaskIsNotNeeded==true){

				Servo::brake();				
				setSubmode(PARKING_PreparingAux);
				reportServoModeStop();

			}else{

				if (PositionTask::getDirection(PersonalSettings::protocolStruct.positionMainParking)==Servo::POSITIVE_DIRECTION){

					Servo::movePositive();
					setSubmode(PARKING_MovingMain);
					reportServoModePositive();
				}else{
					Servo::moveNegative();
					setSubmode(PARKING_MovingMain);
					reportServoModeNegative();
				}
			}	
		}

		processFieldbus();

		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_MovingMain:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);	
			reportServoModeStop();

		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					PersonalSettings::protocolStruct.positionMainParking,
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				Servo::brake();				
				setSubmode(PARKING_PreparingAux);
				reportServoModeStop();

			}else{
				reportServoModeContinue();

			}
		}

		processFieldbus();

		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_PreparingAux:

		Actuators::enable(
			0,
			PersonalSettings::protocolStruct.positionAux1,
			MachineSettings::protocolStructExtended.positionAux1Min,
			MachineSettings::protocolStructExtended.positionAux1Max
		);	
		Actuators::enable(
			1,
			PersonalSettings::protocolStruct.positionAux2,
			MachineSettings::protocolStructExtended.positionAux2Min,
			MachineSettings::protocolStructExtended.positionAux2Max
		);	
		setSubmode(PARKING_MovingAux);


		processFieldbus();
		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_MovingAux:

		if (
			(true==RxMessageQueue::cancelMessageReceived())||
			((Actuators::targetPositionReached(0)==true)&&(Actuators::targetPositionReached(1)==true))
		){
			Actuators::disable(0);
			Actuators::disable(1);
			setSubmode(WAITING_Waiting);
		}

		processFieldbus();
		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Starting:

		Parking::recalculateServoFrequency();
		ForceTestDynamic::recalculateServoFrequency();

		setSubmode(FTEST_DYNAMIC_Homing_SettingPositiveSpeed);

		processFieldbus();

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Homing_SettingPositiveSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(true,Parking::servoFrequencyPositive)==true){
				setSubmode(FTEST_DYNAMIC_Homing_SettingNegativeSpeed);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::POSITIVE_DIRECTION,servoFrequencyPositive=Parking::servoFrequencyPositive));

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Homing_SettingNegativeSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(false,Parking::servoFrequencyNegative)==true){
				setSubmode(FTEST_DYNAMIC_Homing_Preparing);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::NEGATIVE_DIRECTION,servoFrequencyNegative=Parking::servoFrequencyNegative));

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Homing_Preparing:

		{
			bool positionTaskIsNotNeeded = 
				PositionTask::checkPositionStatically(
					ForceTestDynamic::getStartPosition()
					);

			if (positionTaskIsNotNeeded==true){

				//skip
				Servo::brake();				
				setSubmode(FTEST_DYNAMIC_Pause);
				reportServoModeStop();

			}else{

				if (PositionTask::getDirection(ForceTestDynamic::getStartPosition())==Servo::POSITIVE_DIRECTION){

					Servo::movePositive();
					setSubmode(FTEST_DYNAMIC_Homing_Moving);
					reportServoModePositive();
				}else{

					Servo::moveNegative();
					setSubmode(FTEST_DYNAMIC_Homing_Moving);
					reportServoModeNegative();
				}	

			}

		}

		processFieldbus();
		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Homing_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					ForceTestDynamic::getStartPosition(),
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				Servo::brake();				
				setSubmode(FTEST_DYNAMIC_Pause);
				reportServoModeStop();

			}else{
				reportServoModeContinue();

			}

		}

		processFieldbus();

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Pause:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();	
			setSubmode(WAITING_Waiting);

		}else{

			ForceTestDynamic::pauseBeforeTestMsec -= 100;
			if (ForceTestDynamic::pauseBeforeTestMsec<=0){
				setSubmode(FTEST_DYNAMIC_Testing_SettingPositiveSpeed);	
			}			
		}

		processFieldbus();


		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Testing_SettingPositiveSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(true,ForceTestDynamic::servoFrequencyPositive)==true){
				setSubmode(FTEST_DYNAMIC_Testing_SettingNegativeSpeed);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::POSITIVE_DIRECTION,servoFrequencyPositive=ForceTestDynamic::servoFrequencyPositive));

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Testing_SettingNegativeSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(false,ForceTestDynamic::servoFrequencyNegative)==true){
				setSubmode(FTEST_DYNAMIC_Testing_Preparing);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::NEGATIVE_DIRECTION,servoFrequencyNegative=ForceTestDynamic::servoFrequencyNegative));

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Testing_Preparing:

		if (PositionTask::getDirection(ForceTestDynamic::getStopPosition())==Servo::POSITIVE_DIRECTION){

			Servo::movePositive();
			setSubmode(FTEST_DYNAMIC_Testing_Moving);
			reportServoModePositive();

		}else{

			Servo::moveNegative();
			setSubmode(FTEST_DYNAMIC_Testing_Moving);
			reportServoModeNegative();
		}	

		processFieldbus();

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Testing_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);	
			reportServoModeStop();			
		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					ForceTestDynamic::getStopPosition(),
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				Servo::brake();				
				setSubmode(WAITING_Waiting);
				reportServoModeStop();

			}else{
				reportServoModeContinue();

			}

		}

		processFieldbus();

		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Starting:

		Parking::recalculateServoFrequency();

		setSubmode(FTEST_STATIC_Homing_SettingPositiveSpeed);


		processFieldbus();
		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Homing_SettingPositiveSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(true,Parking::servoFrequencyPositive)==true){
				setSubmode(FTEST_STATIC_Homing_SettingNegativeSpeed);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::POSITIVE_DIRECTION,servoFrequencyPositive=Parking::servoFrequencyPositive));
		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Homing_SettingNegativeSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(false,Parking::servoFrequencyNegative)==true){
				setSubmode(FTEST_STATIC_Homing_Preparing);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::NEGATIVE_DIRECTION,servoFrequencyNegative=Parking::servoFrequencyNegative));
		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Homing_Preparing:

		{

			bool positionTaskIsNotNeeded = 
				PositionTask::checkPositionStatically(
					ForceTestStatic::getTestPosition()
					);

			if (positionTaskIsNotNeeded==true){

				Servo::brake();				
				setSubmode(FTEST_STATIC_Pause);
				reportServoModeStop();

			}else{

				if (PositionTask::getDirection(ForceTestStatic::getTestPosition())==Servo::POSITIVE_DIRECTION){

					Servo::movePositive();
					setSubmode(FTEST_STATIC_Homing_Moving);
					reportServoModePositive();

				}else{

					Servo::moveNegative();
					setSubmode(FTEST_STATIC_Homing_Moving);
					reportServoModeNegative();
				}	

			}

		}

		processFieldbus();

		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Homing_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					ForceTestStatic::getTestPosition(),
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				Servo::brake();				
				setSubmode(FTEST_STATIC_Pause);
				reportServoModeStop();

			}else{
				reportServoModeContinue();
			}

		}

		processFieldbus();
		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Pause:

		if (true==RxMessageQueue::cancelMessageReceived()){
			setSubmode(WAITING_Waiting);
		}else{
			ForceTestStatic::pauseBeforeTestMsec -= 100;
			if (ForceTestStatic::pauseBeforeTestMsec<=0){
				setSubmode(FTEST_STATIC_Testing);	
			}
		}

		processFieldbus();
		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Testing:

		if (true==RxMessageQueue::cancelMessageReceived()){
			setSubmode(WAITING_Waiting);
		}else{
			ForceTestStatic::testDurationMsec -= 100;
			if (ForceTestStatic::testDurationMsec<=0){
				setSubmode(WAITING_Waiting);	
			}
		}

		processFieldbus();
		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	//------------------------------------------------FTEST--STATIC----------------------------------
	case EXERCISE_Starting:

		Excercise::excerciseStart();
		Parking::recalculateServoFrequency();

		setSubmode(EXERCISE_Homing_PreparingAux);

		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Homing_PreparingAux:

		Actuators::enable(
			0,
			PersonalSettings::protocolStruct.positionAux1,
			MachineSettings::protocolStructExtended.positionAux1Min,
			MachineSettings::protocolStructExtended.positionAux1Max
		);	
		Actuators::enable(
			1,
			PersonalSettings::protocolStruct.positionAux2,
			MachineSettings::protocolStructExtended.positionAux2Min,
			MachineSettings::protocolStructExtended.positionAux2Max
		);	
		setSubmode(EXERCISE_Homing_MovingAux);

		processFieldbus();


		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Homing_MovingAux:

		if (true==RxMessageQueue::cancelMessageReceived()){
			Actuators::disable(0);
			Actuators::disable(1);
			setSubmode(WAITING_Waiting);
		}else if ((Actuators::targetPositionReached(0)==true)&&(Actuators::targetPositionReached(1)==true)){
			Actuators::disable(0);
			Actuators::disable(1);
			
			setSubmode(EXERCISE_StartingSet);
		}

		processFieldbus();


		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_StartingSet:

		Excercise::setStart();
		setSubmode(EXERCISE_Homing_SettingPositiveSpeed);


		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Homing_SettingPositiveSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(true,Parking::servoFrequencyPositive)==true){
				
				setSubmode(EXERCISE_Homing_SettingNegativeSpeed);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::POSITIVE_DIRECTION,servoFrequencyPositive=Parking::servoFrequencyPositive));

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Homing_SettingNegativeSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(false,Parking::servoFrequencyNegative)==true){
				setSubmode(EXERCISE_Homing_PreparingMain);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::NEGATIVE_DIRECTION,servoFrequencyNegative=Parking::servoFrequencyNegative));

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Homing_PreparingMain:

		{
			bool positionTaskIsNotNeeded = 
				PositionTask::checkPositionStatically(
					Excercise::getPositionMainHoming()
					);

			if (positionTaskIsNotNeeded==true){

				Servo::brake();				
				setSubmode(EXERCISE_Pause);
				reportServoModeStop();


			}else{

				if (PositionTask::getDirection(Excercise::getPositionMainHoming())==Servo::POSITIVE_DIRECTION){

					Servo::movePositive();
					setSubmode(EXERCISE_Homing_MovingMain);
					reportServoModePositive();
					
				}else{

					Servo::moveNegative();
					setSubmode(EXERCISE_Homing_MovingMain);
					reportServoModeNegative();

				}	

			}

		}

		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Homing_MovingMain:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					Excercise::getPositionMainHoming(),
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				Servo::brake();				
				setSubmode(EXERCISE_Pause);
				reportServoModeStop();

			}else{
				reportServoModeContinue();
			}

		} 

		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Pause:

		if (true==RxMessageQueue::cancelMessageReceived()){

			setSubmode(WAITING_Waiting);

		}else if (Excercise::isSetPauseDone()==true){

			Excercise::recalculateServoFrequency();
			setSubmode(EXERCISE_SettingPositiveSpeed);

		}

		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_SettingPositiveSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(true,Excercise::servoFrequencyPositive)==true){
				setSubmode(EXERCISE_SettingNegativeSpeed);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::POSITIVE_DIRECTION,servoFrequencyPositive=Excercise::servoFrequencyPositive));

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_SettingNegativeSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{

			if (Fieldbus::checkSetFrequencyResponse(false,Excercise::servoFrequencyNegative)==true){

				Excercise::repetitionStart();

				bool firstMovementDirection = 
					PositionTask::getDirection(
						Excercise::getPositionMainFirstMovement()
						);

				if (firstMovementDirection==Servo::POSITIVE_DIRECTION){
					Servo::movePositive();
					setSubmode(EXERCISE_FirstMovement);
					reportServoModePositive();
				}else{
					Servo::moveNegative();
					setSubmode(EXERCISE_FirstMovement);
					reportServoModeNegative();
				}

			}else{

				//TODO: countdown is necessary here

			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(Servo::NEGATIVE_DIRECTION,servoFrequencyNegative=Excercise::servoFrequencyNegative));

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_FirstMovement:
	/*
		Steps here from 
			EXERCISE_SettingNegativeSpeed
			EXERCISE_SecondMovement
			EXERCISE_SecondInterruption
	*/

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					Excercise::getPositionMainFirstMovement(),
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				if (Excercise::firstInterruptionEnabled()){

					Servo::brake();
					setSubmode(EXERCISE_FirstInterruption);
					reportServoModeStop();


				}else{
					//interruption disabled
					bool secondMovementDirection = 
						PositionTask::getDirection(
							Excercise::getPositionMainSecondMovement()
							);

					if (secondMovementDirection==Servo::POSITIVE_DIRECTION){
						Servo::movePositive();
						setSubmode(EXERCISE_SecondMovement);
						reportServoModePositive();
					}else{
						Servo::moveNegative();
						setSubmode(EXERCISE_SecondMovement);
						reportServoModeNegative();
					}

				}

				Odometer::incrementDegrees((uint32_t)PersonalSettings::getMainRangeDegrees());

			}else{
				//positionTaskIsComplete==false
				reportServoModeContinue();
			}

		}

		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_FirstInterruption:
	/*
		Steps here from 
			EXERCISE_FirstMovement
	*/

		if (true==RxMessageQueue::cancelMessageReceived()){
			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

		}else if (Excercise::isFirstInterruptionDone()==true){

			bool secondMovementDirection = 
				PositionTask::getDirection(
					Excercise::getPositionMainSecondMovement()
					);

			if (secondMovementDirection==Servo::POSITIVE_DIRECTION){
				Servo::movePositive();
				setSubmode(EXERCISE_SecondMovement);
				reportServoModePositive();
			}else{
				Servo::moveNegative();
				setSubmode(EXERCISE_SecondMovement);
				reportServoModeNegative();
			}

		}

		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_SecondMovement:
	/*
		Steps here from 
			EXERCISE_FirstMovement
			EXERCISE_FirstInterruption
	*/

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					Excercise::getPositionMainSecondMovement(),
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				if (Excercise::secondInterruptionEnabled()){

					Servo::brake();
					setSubmode(EXERCISE_SecondInterruption);
					reportServoModeStop();

				}else{
					//interruption disabled

					Excercise::repetitionDone();
					if (Excercise::isSetDone()==true){

						Servo::brake();
						if (Excercise::isExcerciseDone()==true){

							setSubmode(WAITING_Waiting);
						}else{

							setSubmode(EXERCISE_StartingSet);
						}
						reportServoModeStop();

					}else{			

						Excercise::repetitionStart();

						bool firstMovementDirection = 
							PositionTask::getDirection(
								Excercise::getPositionMainFirstMovement()
								);

						if (firstMovementDirection==Servo::POSITIVE_DIRECTION){
							Servo::movePositive();
							setSubmode(EXERCISE_FirstMovement);
							reportServoModePositive();
						}else{
							Servo::moveNegative();
							setSubmode(EXERCISE_FirstMovement);
							reportServoModeNegative();
						}
					}

				}

				Odometer::incrementDegrees((uint32_t)PersonalSettings::getMainRangeDegrees());

			}else{
				//positionTaskIsComplete==false

				reportServoModeContinue();
			}

		}

		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_SecondInterruption:
	/*
		Steps here from 
			EXERCISE_SecondMovement
	*/

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

		}else if (Excercise::isSecondInterruptionDone()==true){

			Excercise::repetitionDone();
			if (Excercise::isSetDone()==true){

				Servo::brake();
				if (Excercise::isExcerciseDone()==true){

					setSubmode(WAITING_Waiting);
				}else{

					setSubmode(EXERCISE_StartingSet);
				}
				reportServoModeStop();

			}else{			

				Excercise::repetitionStart();

				bool firstMovementDirection = 
					PositionTask::getDirection(
						Excercise::getPositionMainFirstMovement()
						);

				if (firstMovementDirection==Servo::POSITIVE_DIRECTION){
					Servo::movePositive();
					setSubmode(EXERCISE_FirstMovement);
					reportServoModePositive();
				}else{
					Servo::moveNegative();
					setSubmode(EXERCISE_FirstMovement);
					reportServoModeNegative();
				}
			}
		}

		processFieldbus();

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Starting:


		GenericSet::start();

		setSubmode(GENERIC_SET_Homing_PreparingAux);

		processFieldbus();

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Homing_PreparingAux:

		Actuators::enable(
			0,
			PersonalSettings::protocolStruct.positionAux1,
			MachineSettings::protocolStructExtended.positionAux1Min,
			MachineSettings::protocolStructExtended.positionAux1Max
		);	
		Actuators::enable(
			1,
			PersonalSettings::protocolStruct.positionAux2,
			MachineSettings::protocolStructExtended.positionAux2Min,
			MachineSettings::protocolStructExtended.positionAux2Max
		);	
		setSubmode(GENERIC_SET_Homing_MovingAux);

		processFieldbus();


		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Homing_MovingAux:

		if (true==RxMessageQueue::cancelMessageReceived()){
			Actuators::disable(0);
			Actuators::disable(1);
			setSubmode(WAITING_Waiting);
		}else if ((Actuators::targetPositionReached(0)==true)&&(Actuators::targetPositionReached(1)==true)){
			Actuators::disable(0);
			Actuators::disable(1);
			
			GenericSet::pause1Start();
			setSubmode(GENERIC_SET_Pause1);

		}

		processFieldbus();

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Pause1:

		if (true==RxMessageQueue::cancelMessageReceived()){

			setSubmode(WAITING_Waiting);

		
		}else if (GenericSet::isPauseDone()==true){

			Parking::recalculateServoFrequency();
			
			setSubmode(GENERIC_SET_Homing_SettingPositiveSpeed);

		}else{
			
			__asm("	nop");
		}

		processFieldbus();

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Homing_SettingPositiveSpeed:

		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(
			Servo::POSITIVE_DIRECTION,
			servoFrequencyPositive=Parking::servoFrequencyPositive
			));			

		setSubmode(GENERIC_SET_Homing_SettingNegativeSpeed);
	
	
		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Homing_SettingNegativeSpeed:

		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(
			Servo::NEGATIVE_DIRECTION,
			servoFrequencyNegative=Parking::servoFrequencyNegative
			));			
	
		//GenericSet::pause2Start();setSubmode(GENERIC_SET_Pause2);
		setSubmode(GENERIC_SET_Homing_PreparingMain);
	

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Homing_PreparingMain:

		if (PositionTask::checkPositionStatically(GenericSet::getPositionMainStart())==false){

			if (PositionTask::getDirection(GenericSet::getPositionMainStart())==Servo::POSITIVE_DIRECTION){

				Servo::movePositive();

				setSubmode(GENERIC_SET_Homing_MovingMain);
				reportServoModePositive();
			}else{

				Servo::moveNegative();

				setSubmode(GENERIC_SET_Homing_MovingMain);
				reportServoModeNegative();
			}						
		}else{
			GenericSet::pause2Start();
			setSubmode(GENERIC_SET_Pause2);
		}
		processFieldbus();

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Homing_MovingMain:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					GenericSet::getPositionMainStart(),
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				Servo::brake();

				GenericSet::pause2Start();
				setSubmode(GENERIC_SET_Pause2);
				reportServoModeStop();

			}else{

				reportServoModeContinue();

			}

		}

		processFieldbus();

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Pause2:

		if (true==RxMessageQueue::cancelMessageReceived()){

			setSubmode(WAITING_Waiting);

		}else if (GenericSet::isPauseDone()==true){

			setSubmode(WAITING_Waiting);

			setSubmode(GENERIC_SET_Move_Preparing);

			__asm("	nop");

		}else{
			__asm("	nop");
		}

		processFieldbus();
	
	
		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Move_Preparing:

		if (PositionTask::getDirection(GenericSet::getMoveDestinationPosition())==Servo::POSITIVE_DIRECTION){

			GenericSet::recalculateServoFrequency(Servo::POSITIVE_DIRECTION);

			Servo::movePositive();

			setSubmode(GENERIC_SET_Move_Moving);
			reportServoModePositive();

			Fieldbus::pushUSSRequest(
				USS::makeSetFrequencyRequest(
					Servo::POSITIVE_DIRECTION,
					servoFrequencyPositive=GenericSet::servoFrequencyPositive_
					)
				);

		}else{

			GenericSet::recalculateServoFrequency(Servo::NEGATIVE_DIRECTION);

			Servo::moveNegative();

			setSubmode(GENERIC_SET_Move_Moving);
			reportServoModeNegative();

			Fieldbus::pushUSSRequest(
				USS::makeSetFrequencyRequest(
					Servo::NEGATIVE_DIRECTION,
					servoFrequencyNegative=GenericSet::servoFrequencyNegative_
					)
				);

		}	

		//processFieldbus();

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Move_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					GenericSet::getMoveDestinationPosition(),
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				Servo::brake();

				GenericSet::moveComplete();
				if (GenericSet::isSetComplete()==false){

					setSubmode(GENERIC_SET_Move_Preparing);

				}else{

					setSubmode(WAITING_Waiting);

				}

				reportServoModeStop();

			}else{

				reportServoModeContinue();

			}

		}

		processFieldbus();

		break;
	//------------------------------------------------GENERIC-SET---------------------------------





	//------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	case FAULT_Fault:

		Servo::brake(true);
		Servo::movePositive(false);
		Servo::moveNegative(false);
		Actuators::emergencyStop();
		Servo::parkingBrake(true);
		

		if (true==RxMessageQueue::resetErrorMessageReceived()){
			RxMessageQueue::flush();
			Errors::init();
			setSubmode(INITIALIZING_WaitingStableServoConnection);
		}

		break;
	//------------------------------------------------------------------------------------
	default:
		//ERROR_RtcuLogicError
		break;
	//------------------------------------------------------------------------------------
	}

	reportServoModeDefault();

	DebugConsole::process();

	profilerStop();
	tickID++;

}
//==================================================================================================================
void MainTick::profilerStart(){
	profilerStartTimestamp = Heartbeat::getCounterValue();
	profilerStartSubmode = submode;
}
//==================================================================================================================
void MainTick::profilerStop(){

	uint32_t profilerStopTimestamp = Heartbeat::getCounterValue();
	profilerLastValue = profilerStopTimestamp - profilerStartTimestamp;
	if (profilerLastValue>=profilerMaxValue){
		profilerMaxValue = profilerLastValue;
		profilerMaxValueSubmode = profilerStartSubmode;
	}
}
//==================================================================================================================
void MainTick::processFieldbus(){

	if (Fieldbus::responseIsValid()==false){
		fieldbusErrorCounter++;
		if (fieldbusErrorCounter>fieldbusErrorCounterMax){
			fieldbusErrorCounterMax = fieldbusErrorCounter;
		}

		DebugConsole::pushMessage(" #FieldbusResponseLost\0");
		if (fieldbusErrorCounter>=FIELDBUS_FAULT_TRESHOLD){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);	
			fieldbusErrorCounterMax = 0;
			//DebugConsole::pushMessage(" #FieldbusResponseLost FAULT\0");
		}

	}else{
		fieldbusErrorCounter = 0;

	}
	Fieldbus::pushUSSRequest(USS::makeInverterReadyRequest());

}
//==================================================================================================================





