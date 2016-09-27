
#include "MainTick.h"

#include <string.h>

#include "business/Odometer.h"
#include "business/Errors.h"
#include "business/EmergencyStop.h"
#include "business/MachineSettings.h"
#include "business/PersonalSettings.h"
#include "business/ExcerciseSettings.h"
#include "business/RangeAdjustment.h"
#include "business/Parking.h"
#include "business/ForceTestDynamic.h"
#include "business/ForceTestStatic.h"
#include "business/PositionTask.h"
#include "business/Excercise.h"

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
MainTick::Submode MainTick::submode;

uint_fast8_t MainTick::rangeAdjustmentTimeoutCounter;

uint32_t MainTick::profilerStartTimestamp = 0;
uint32_t MainTick::profilerLastValue = 0;
uint32_t MainTick::profilerMaxValue = 0;
MainTick::Submode MainTick::profilerMaxValueSubmode = ERROR_Error;
MainTick::Submode MainTick::profilerStartSubmode = ERROR_Error;



//==================================================================================================================
void MainTick::init(){




	setSubmode(INITIALIZING_JustStarted);



}
//==================================================================================================================
void MainTick::process(){ //called every 100ms

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
		

		NFC::protocol.sendPacket(Protocol::TAG_CheckRfidProximity,NULL,0);
	}

	{
		/*
		NFC::getResponse();
		NFC::sendRequest();
		//TAG_RfidProximity message consists of UID length (4bytes) followed by UID value (at least 4 bytes)
		uint8_t uidMessage[14+NFC::MAX_UID_LENGTH];	
		memset(uidMessage,0,sizeof(uidMessage));
		uidMessage[0] = NFC::getUID(&uidMessage[4]);
		if (uidMessage[0]!=0){
			Diagnostics::protocol.sendPacket(Protocol::TAG_RfidProximity,uidMessage,4+uidMessage[0]);
			HMI::protocol.sendPacket(Protocol::TAG_RfidProximity,uidMessage,4+uidMessage[0]);
		}
		*/
	}
	
	
	if (Errors::asserted()){
		setSubmode(ERROR_Error);
	}



	reportCurrentMode();

	switch(submode){
	//--------------------------------------------------------INITIALIZING---------------------------
	case INITIALIZING_JustStarted:

		DebugConsole::pushMessage(" #MainTick started\0");

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
					Servo::brake(true);

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
				case Protocol::TAG_LoadMachineSettings:
					MachineSettings::load(message);
					break;
				case Protocol::TAG_Personal:
					if (MachineSettings::protocolStructValid==true){
						setSubmode(PERSONAL_Starting);//TODO:check message length
						DebugConsole::pushMessage(" #Personal\0");
					}
					break;
				case Protocol::TAG_Parking: 
					if (
						(MachineSettings::protocolStructValid==true)&&
						(PersonalSettings::protocolStructValid==true)
					){
						setSubmode(PARKING_Starting);//TODO:check message length
						DebugConsole::pushMessage(" #Parking\0");
					}
					break;
				case Protocol::TAG_TestConcentric:
				case Protocol::TAG_TestEccentric:

					if (
						(MachineSettings::protocolStructValid==true)&&
						(PersonalSettings::protocolStructValid==true)&&
						(true==ForceTestDynamic::load(message))
					){
						setSubmode(FTEST_DYNAMIC_Starting);
						DebugConsole::pushMessage(" #ForceTestDynamic\0");
					}
					break;


				case Protocol::TAG_TestStatic:

					if (
						(MachineSettings::protocolStructValid==true)&&
						(PersonalSettings::protocolStructValid==true)&&
						(true==ForceTestStatic::load(message))
					){
						setSubmode(FTEST_STATIC_Starting);
						DebugConsole::pushMessage(" #ForceTestStatic\0");
					}
					break;
				
				case Protocol::TAG_ExcerciseIsokinetic:

					if (
						(MachineSettings::protocolStructValid==true)&&
						(PersonalSettings::protocolStructValid==true)&&
						(ExcerciseSettings::valid==true)
					){
						setSubmode(EXERCISE_Starting);
						DebugConsole::pushMessage(" #ExcerciseIsokinetic\0");
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

		RangeAdjustment::servoFrequency = Servo::absSpeedToFrequency(
			MachineSettings::getMainRange(),
			MachineSettings::protocolStruct.speedAbsMainPersonal
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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(true,RangeAdjustment::servoFrequency));

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

		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(false,-RangeAdjustment::servoFrequency));

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
							Servo::moveNegative(true);
							Servo::brake(false);
							setSubmode(PERSONAL_MovingMain);
							rangeAdjustmentTimeoutCounter = TIMEOUT_RangeAdjusmentMessage;
						}else{
							DebugConsole::pushMessage(" #unable to move further+\0");

						}
						break;
					case RangeAdjustment::BUTTONID_MAIN_PLUS:

						if (true==Servo::validateActualPosition(Servo::POSITIVE_DIRECTION)){

							Servo::movePositive(true);
							Servo::brake(false);
							setSubmode(PERSONAL_MovingMain);
							rangeAdjustmentTimeoutCounter = TIMEOUT_RangeAdjusmentMessage;
						}else{
							DebugConsole::pushMessage(" #unable to move further-\0");

						}
						break;
					case RangeAdjustment::BUTTONID_AUX1_MINUS:

						Actuators::enable(
							0,
							MachineSettings::protocolStruct.positionAux1Max,
							MachineSettings::protocolStruct.positionAux1Min,
							MachineSettings::protocolStruct.positionAux1Max
						);	
						setSubmode(PERSONAL_MovingAux);
						break;
					case RangeAdjustment::BUTTONID_AUX1_PLUS:

						Actuators::enable(
							0,
							MachineSettings::protocolStruct.positionAux1Min,
							MachineSettings::protocolStruct.positionAux1Min,
							MachineSettings::protocolStruct.positionAux1Max
						);	
						setSubmode(PERSONAL_MovingAux);
						break;
					case RangeAdjustment::BUTTONID_AUX2_MINUS:

						Actuators::enable(
							1,
							MachineSettings::protocolStruct.positionAux2Max,
							MachineSettings::protocolStruct.positionAux2Min,
							MachineSettings::protocolStruct.positionAux2Max
						);	
						setSubmode(PERSONAL_MovingAux);
						break;
					case RangeAdjustment::BUTTONID_AUX2_PLUS:

						Actuators::enable(
							1,
							MachineSettings::protocolStruct.positionAux2Min,
							MachineSettings::protocolStruct.positionAux2Min,
							MachineSettings::protocolStruct.positionAux2Max
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

		}else if (
			(RangeAdjustment::BUTTONID_MAIN_MINUS==RangeAdjustment::actualButtonID)&&
			(false==Servo::validateActualPosition(Servo::NEGATIVE_DIRECTION))
		){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(PERSONAL_Waiting);

		}else if ((rangeAdjustmentTimeoutCounter--)==0){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(PERSONAL_Waiting);

		}else{

			RxMessage *message = RxMessageQueue::pop();
			if (message!=NULL){
				switch(message->tag){
				case Protocol::TAG_PersonalExit:
					Servo::brake(true);
					Servo::movePositive(false);
					Servo::moveNegative(false);
					setSubmode(WAITING_Waiting);
					break;
				case Protocol::TAG_PersonalButtonHold:

					if (RangeAdjustment::actualButtonID==message->value[0]){//TODO: check message length

						rangeAdjustmentTimeoutCounter = TIMEOUT_RangeAdjusmentMessage;
						//keep moving

					}else{
						Servo::brake(true);
						Servo::movePositive(false);
						Servo::moveNegative(false);
						setSubmode(PERSONAL_Waiting);
					}
					break;

				case Protocol::TAG_PersonalButtonReleased:
				
					Servo::brake(true);
					Servo::movePositive(false);
					Servo::moveNegative(false);
					setSubmode(PERSONAL_Waiting);

				}
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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(true,Parking::servoFrequencyPositive));


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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(false,Parking::servoFrequencyNegative));


		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_PreparingMain:

		//MessageQueue::flush();

		if (PositionTask::checkPosition(PersonalSettings::protocolStruct.positionMainParking)==false){

			if (PositionTask::getDirection(PersonalSettings::protocolStruct.positionMainParking)==Servo::POSITIVE_DIRECTION){
				Servo::movePositive(true);
				Servo::brake(false);
			}else{
				Servo::moveNegative(true);
				Servo::brake(false);
			}						

			setSubmode(PARKING_MovingMain);
		}else{
			setSubmode(PARKING_PreparingAux);
		}




		processFieldbus();



		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_MovingMain:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(WAITING_Waiting);	


		}else if (PositionTask::checkPosition(PersonalSettings::protocolStruct.positionMainParking)==true){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(PARKING_PreparingAux);

		}


		processFieldbus();


		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_PreparingAux:

		Actuators::enable(
			0,
			PersonalSettings::protocolStruct.positionAux1,
			MachineSettings::protocolStruct.positionAux1Min,
			MachineSettings::protocolStruct.positionAux1Max
		);	
		Actuators::enable(
			1,
			PersonalSettings::protocolStruct.positionAux2,
			MachineSettings::protocolStruct.positionAux2Min,
			MachineSettings::protocolStruct.positionAux2Max
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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(true,Parking::servoFrequencyPositive));

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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(false,Parking::servoFrequencyNegative));

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Homing_Preparing:

		if (PositionTask::checkPosition(ForceTestDynamic::getParkingPosition())==false){

			if (PositionTask::getDirection(ForceTestDynamic::getParkingPosition())==Servo::POSITIVE_DIRECTION){

				Servo::movePositive(true);
				Servo::brake(false);
			}else{
				Servo::moveNegative(true);
				Servo::brake(false);

			}						

			setSubmode(FTEST_DYNAMIC_Homing_Moving);
		}else{
			setSubmode(FTEST_DYNAMIC_Pause);
		}


		processFieldbus();
		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Homing_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(WAITING_Waiting);

		}else if (PositionTask::checkPosition(ForceTestDynamic::getParkingPosition())==true){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(FTEST_DYNAMIC_Pause);

		}

		processFieldbus();




		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Pause:


		if (true==RxMessageQueue::cancelMessageReceived()){
			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(true,ForceTestDynamic::servoFrequencyPositive));

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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(false,ForceTestDynamic::servoFrequencyNegative));

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Testing_Preparing:

		if (PositionTask::checkPosition(ForceTestDynamic::getSecondPosition())==false){

			if (PositionTask::getDirection(ForceTestDynamic::getSecondPosition())==Servo::POSITIVE_DIRECTION){
				Servo::movePositive(true);
				Servo::brake(false);

			}else{
				Servo::moveNegative(true);
				Servo::brake(false);

			}						

			setSubmode(FTEST_DYNAMIC_Testing_Moving);
		}else{
			setSubmode(WAITING_Waiting);
		}


		processFieldbus();


		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Testing_Moving:



		if (
			(true==RxMessageQueue::cancelMessageReceived())||
			(PositionTask::checkPosition(ForceTestDynamic::getSecondPosition())==true)
		){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(WAITING_Waiting);

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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(true,Parking::servoFrequencyPositive));
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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(false,Parking::servoFrequencyNegative));
		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Homing_Preparing:


		if (PositionTask::checkPosition(ForceTestStatic::getTestPosition())==false){

			if (PositionTask::getDirection(ForceTestStatic::getTestPosition())==Servo::POSITIVE_DIRECTION){
				Servo::movePositive(true);
				Servo::brake(false);

			}else{
				Servo::moveNegative(true);
				Servo::brake(false);

			}						

			setSubmode(FTEST_STATIC_Homing_Moving);
		}else{
			setSubmode(FTEST_STATIC_Pause);
		}

		processFieldbus();

		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Homing_Moving:


		if (true==RxMessageQueue::cancelMessageReceived()){
			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(WAITING_Waiting);
		}else if (PositionTask::checkPosition(ForceTestStatic::getTestPosition())==true){
			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(FTEST_STATIC_Pause);
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
			MachineSettings::protocolStruct.positionAux1Min,
			MachineSettings::protocolStruct.positionAux1Max
		);	
		Actuators::enable(
			1,
			PersonalSettings::protocolStruct.positionAux2,
			MachineSettings::protocolStruct.positionAux2Min,
			MachineSettings::protocolStruct.positionAux2Max
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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(true,Parking::servoFrequencyPositive));

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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(false,Parking::servoFrequencyNegative));

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Homing_PreparingMain:

		if (PositionTask::checkPosition(Excercise::getPositionMainHoming())==false){

			if (PositionTask::getDirection(Excercise::getPositionMainHoming())==Servo::POSITIVE_DIRECTION){
				Servo::movePositive(true);
				Servo::brake(false);
			}else{
				Servo::moveNegative(true);
				Servo::brake(false);
			}						
			setSubmode(EXERCISE_Homing_MovingMain);
		}else{
			setSubmode(EXERCISE_Pause);
		}
		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Homing_MovingMain:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(WAITING_Waiting);

		}else if (PositionTask::checkPosition(Excercise::getPositionMainHoming())==true){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(EXERCISE_Pause);
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
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(true,Excercise::servoFrequencyPositive));

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_SettingNegativeSpeed:

		if (Fieldbus::responseIsValid()==false){
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
		}else{
			if (Fieldbus::checkSetFrequencyResponse(false,Excercise::servoFrequencyNegative)==true){


				Excercise::repetitionStart();
				setSubmode(EXERCISE_FirstMovement);
			}else{
				//TODO: countdown is necessary here
			}
		}
		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(false,Excercise::servoFrequencyNegative));

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_FirstMovement:

		if (true==RxMessageQueue::cancelMessageReceived()){
			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(WAITING_Waiting);

		}else if (PositionTask::checkPosition(Excercise::getPositionMainFirstMovement(),Excercise::getSpeedFirstMovement())==false){

			if (PositionTask::getDirection(Excercise::getPositionMainFirstMovement())==Servo::NEGATIVE_DIRECTION){
				Servo::moveNegative(true);
				Servo::movePositive(false);
				Servo::brake(false);
			}else{
				Servo::movePositive(true);
				Servo::moveNegative(false);
				Servo::brake(false);
			}						

		}else if (Excercise::firstInterruptionEnabled()){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(EXERCISE_FirstInterruption);

		}else{

			//test
			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);

			setSubmode(EXERCISE_SecondMovement);

		}


		processFieldbus();


		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_FirstInterruption:

		if (true==RxMessageQueue::cancelMessageReceived()){
			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(WAITING_Waiting);
		}else if (Excercise::isFirstInterruptionDone()==true){
			setSubmode(EXERCISE_SecondMovement);

		}

		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_SecondMovement:


		if (true==RxMessageQueue::cancelMessageReceived()){
			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(WAITING_Waiting);
		}else if (PositionTask::checkPosition(Excercise::getPositionMainSecondMovement(),Excercise::getSpeedSecondMovement())==false){

			if (PositionTask::getDirection(Excercise::getPositionMainSecondMovement())==Servo::NEGATIVE_DIRECTION){
				Servo::moveNegative(true);
				Servo::movePositive(false);
				Servo::brake(false);
			}else{
				Servo::movePositive(true);
				Servo::moveNegative(false);
				Servo::brake(false);
			}						

		}else if (Excercise::secondInterruptionEnabled()){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(EXERCISE_SecondInterruption);

		}else{

			Excercise::repetitionDone();
			if (Excercise::isSetDone()==true){

				Servo::brake(true);
				Servo::movePositive(false);
				Servo::moveNegative(false);
				if (Excercise::isExcerciseDone()==true){
					setSubmode(WAITING_Waiting);
				}else{
					setSubmode(EXERCISE_StartingSet);
				}

			}else{

				//debug
				Servo::brake(true);
				Servo::movePositive(false);
				Servo::moveNegative(false);



				Excercise::repetitionStart();
				setSubmode(EXERCISE_FirstMovement);
			}

		}



		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_SecondInterruption:

		if (true==RxMessageQueue::cancelMessageReceived()){
			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(WAITING_Waiting);
		}else if (Excercise::isSecondInterruptionDone()==true){
			Excercise::repetitionDone();
			if (Excercise::isSetDone()==true){

				Servo::brake(true);
				Servo::movePositive(false);
				Servo::moveNegative(false);
				if (Excercise::isExcerciseDone()==true){
					setSubmode(WAITING_Waiting);
				}else{
					setSubmode(EXERCISE_StartingSet);
				}

			}else{			
				Excercise::repetitionStart();
				setSubmode(EXERCISE_FirstMovement);
			}
		}

		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------





	//------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	case ERROR_Error:

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

	DebugConsole::process();

	profilerStop();

}
//==================================================================================================================
void MainTick::processMid(){

	__asm("	nop");

	switch(submode){

	case EXERCISE_FirstMovement:

		if (PositionTask::checkPosition(Excercise::getPositionMainFirstMovement(),Excercise::getSpeedFirstMovement())==false){

			if (PositionTask::getDirection(Excercise::getPositionMainFirstMovement())==Servo::NEGATIVE_DIRECTION){
				Servo::moveNegative(true);
				Servo::movePositive(false);
				Servo::brake(false);
			}else{
				Servo::movePositive(true);
				Servo::moveNegative(false);
				Servo::brake(false);
			}						

		}else if (Excercise::firstInterruptionEnabled()){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(EXERCISE_FirstInterruption);

		}else{

			//test
			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);

			setSubmode(EXERCISE_SecondMovement);

		}




		break;

	case EXERCISE_SecondMovement:


		if (PositionTask::checkPosition(Excercise::getPositionMainSecondMovement(),Excercise::getSpeedSecondMovement())==false){

			if (PositionTask::getDirection(Excercise::getPositionMainSecondMovement())==Servo::NEGATIVE_DIRECTION){
				Servo::moveNegative(true);
				Servo::movePositive(false);
				Servo::brake(false);
			}else{
				Servo::movePositive(true);
				Servo::moveNegative(false);
				Servo::brake(false);
			}						

		}else if (Excercise::secondInterruptionEnabled()){

			Servo::brake(true);
			Servo::movePositive(false);
			Servo::moveNegative(false);
			setSubmode(EXERCISE_SecondInterruption);

		}else{

			Excercise::repetitionDone();
			if (Excercise::isSetDone()==true){

				Servo::brake(true);
				Servo::movePositive(false);
				Servo::moveNegative(false);
				if (Excercise::isExcerciseDone()==true){
					setSubmode(WAITING_Waiting);
				}else{
					setSubmode(EXERCISE_StartingSet);
				}

			}else{

				//debug
				Servo::brake(true);
				Servo::movePositive(false);
				Servo::moveNegative(false);



				Excercise::repetitionStart();
				setSubmode(EXERCISE_FirstMovement);
			}

		}


		break;


	}

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
			Errors::setFlag(Errors::FLAG_USS_RESPONSE);
			DebugConsole::pushMessage(" #FieldbusResponseLost\0");
		}
		Fieldbus::pushUSSRequest(USS::makeInverterReadyRequest());


}
//==================================================================================================================





