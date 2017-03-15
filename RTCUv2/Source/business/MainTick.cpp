
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
#include "business/AuxMonitor.h"

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

FrequencyModulation MainTick::fmTest;
FrequencyModulation MainTick::fmHoming;
FrequencyModulation MainTick::fmExcercise;
FrequencyModulation MainTick::fmGenericSet;
FrequencyModulation MainTick::fmPersonal;


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

		Servo::brake();
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
				Fieldbus::pushUSSRequest(USS::makeHeatsinkTemperatureRequest());
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
		Fieldbus::pushUSSRequest(USS::makeHeatsinkTemperatureRequest());

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
			Servo::brake();
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
				//--------------------------------------------------------------------------
				case Protocol::TAG_LoadPersonalSettings:
					PersonalSettings::load(message);
					break;
				//--------------------------------------------------------------------------
				case Protocol::TAG_LoadExcerciseSettings:
					ExcerciseSettings::load(message);
					break;
				//--------------------------------------------------------------------------
				case Protocol::TAG_LoadGenericSetSettings:
					GenericSetSettings::load(message);
					break;
				//--------------------------------------------------------------------------
				case Protocol::TAG_LoadMachineSettingsExtended:
					MachineSettings::load(message);
					break;
				//--------------------------------------------------------------------------
				case Protocol::TAG_Personal:
					if (MachineSettings::protocolStructExtendedValid==true){
						setSubmode(PERSONAL_Waiting);//TODO:check message length
						DebugConsole::pushMessage(" #Personal\0");
					}
					break;
				//--------------------------------------------------------------------------
				case Protocol::TAG_Parking: 
					if (
						(MachineSettings::protocolStructExtendedValid==true)&&
						(PersonalSettings::protocolStructValid==true)
					){
						setSubmode(PARKING_Starting);//TODO:check message length
						DebugConsole::pushMessage(" #Parking\0");
					}
					break;
				//--------------------------------------------------------------------------
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
				//--------------------------------------------------------------------------
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
				//--------------------------------------------------------------------------
				case Protocol::TAG_ExcerciseIsokinetic:

					if (
						(MachineSettings::protocolStructExtendedValid==true)&&
						(PersonalSettings::protocolStructValid==true)&&
						(ExcerciseSettings::valid==true)
					){
						setSubmode(EXERCISE_Starting);
						DebugConsole::pushMessage(" #ExcerciseIsokinetic\0");
						AuxMonitor::setDataFromRxMessage(message->valueLen, message->value);
						

					}
					break;
				//--------------------------------------------------------------------------
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
				//--------------------------------------------------------------------------
				}
			}else{

			}			
		}

		processFieldbus();

		break;
	//--------------------------------------------------PERSONAL----------------------------------
	//--------------------------------------------------PERSONAL----------------------------------
	case PERSONAL_Waiting:

		{
			Servo::brake();

			RxMessage *message = RxMessageQueue::pop();
			if (message!=NULL){
				switch(message->tag){
				//---message----------------------------------------------------------------------------
				default:
					processFieldbus();
					break;
				//---message----------------------------------------------------------------------------
				case Protocol::TAG_PersonalExit:
					setSubmode(WAITING_Waiting);
					processFieldbus();
					break;
				//---message----------------------------------------------------------------------------
				case Protocol::TAG_PersonalButtonPressed:

					RangeAdjustment::actualButtonID = message->value[0];//TODO: check message length
					switch(RangeAdjustment::actualButtonID){
					//-----button------------------------------------------------------------------------
					case RangeAdjustment::BUTTONID_MAIN_MINUS:

						{
							bool positionTaskIsNotNeeded = 
								PositionTask::checkPositionStatically(
									MachineSettings::protocolStructExtended.positionMainMin
									);

							if (positionTaskIsNotNeeded==false){

								float maxFrequency = Servo::rangeToFrequency(
									MachineSettings::getMainRange(),
									MachineSettings::protocolStructExtended.speedAbsMainPersonal
									);

								fmPersonal.prepare(
									-maxFrequency,
									MachineSettings::protocolStructExtended.positionMainMin,
									Servo::NEGATIVE_DIRECTION,
									FM_PERSONAL_LAW,
									FM_PERSONAL_LAW,
									FM_PERSONAL_MIN_RANGE
									);

								Fieldbus::pushUSSRequest(
									USS::makeSetFrequencyRequest(
										fmPersonal.getDirection(),
										servoFrequencyNegative=fmPersonal.getFrequency()
										)
									);

								Servo::moveNegative();
								setSubmode(PERSONAL_MovingMain);
								reportServoModeNegative();

								rangeAdjustmentTimeoutCounter = TIMEOUT_RangeAdjusmentMessage;

							}


						}

						break;
					//-----button------------------------------------------------------------------------
					case RangeAdjustment::BUTTONID_MAIN_PLUS:

						{
							bool positionTaskIsNotNeeded = 
								PositionTask::checkPositionStatically(
									MachineSettings::protocolStructExtended.positionMainMax
									);

							if (positionTaskIsNotNeeded==false){

								float maxFrequency = Servo::rangeToFrequency(
									MachineSettings::getMainRange(),
									MachineSettings::protocolStructExtended.speedAbsMainPersonal
									);

								fmPersonal.prepare(
									maxFrequency,
									MachineSettings::protocolStructExtended.positionMainMax,
									Servo::POSITIVE_DIRECTION,
									FM_PERSONAL_LAW,
									FM_PERSONAL_LAW,
									FM_PERSONAL_MIN_RANGE
									);

								Fieldbus::pushUSSRequest(
									USS::makeSetFrequencyRequest(
										fmPersonal.getDirection(),
										servoFrequencyPositive=fmPersonal.getFrequency()
										)
									);

								Servo::movePositive();
								setSubmode(PERSONAL_MovingMain);
								reportServoModePositive();	
								
								rangeAdjustmentTimeoutCounter = TIMEOUT_RangeAdjusmentMessage;							
								
							}


						}

						break;
					//-----button------------------------------------------------------------------------
					case RangeAdjustment::BUTTONID_AUX1_MINUS:

						Actuators::enable(
							0,
							MachineSettings::protocolStructExtended.positionAux1Max,
							MachineSettings::protocolStructExtended.positionAux1Min,
							MachineSettings::protocolStructExtended.positionAux1Max
						);	
						setSubmode(PERSONAL_MovingAux);
						processFieldbus();
						break;
					//-----button------------------------------------------------------------------------
					case RangeAdjustment::BUTTONID_AUX1_PLUS:

						Actuators::enable(
							0,
							MachineSettings::protocolStructExtended.positionAux1Min,
							MachineSettings::protocolStructExtended.positionAux1Min,
							MachineSettings::protocolStructExtended.positionAux1Max
						);	
						setSubmode(PERSONAL_MovingAux);
						processFieldbus();
						break;
					//-----button------------------------------------------------------------------------
					case RangeAdjustment::BUTTONID_AUX2_MINUS:

						Actuators::enable(
							1,
							MachineSettings::protocolStructExtended.positionAux2Max,
							MachineSettings::protocolStructExtended.positionAux2Min,
							MachineSettings::protocolStructExtended.positionAux2Max
						);	
						setSubmode(PERSONAL_MovingAux);
						processFieldbus();
						break;
					//-----button------------------------------------------------------------------------
					case RangeAdjustment::BUTTONID_AUX2_PLUS:

						Actuators::enable(
							1,
							MachineSettings::protocolStructExtended.positionAux2Min,
							MachineSettings::protocolStructExtended.positionAux2Min,
							MachineSettings::protocolStructExtended.positionAux2Max
						);	
						setSubmode(PERSONAL_MovingAux);
						processFieldbus();
						break;
					//-----button------------------------------------------------------------------------
					}
					break;
				//---message----------------------------------------------------------------------------
				}
			}else{
				processFieldbus();
			}			
		}



		break;
	//--------------------------------------------------PERSONAL----------------------------------
	case PERSONAL_MovingMain:

		if (
			(RangeAdjustment::BUTTONID_MAIN_PLUS==RangeAdjustment::actualButtonID)&&
			(PositionTask::checkPositionStatically(
									MachineSettings::protocolStructExtended.positionMainMax
									)==true)
		){

			Servo::brake();
			setSubmode(PERSONAL_Waiting);
			reportServoModeStop();
			processFieldbus();

		}else if (
			(RangeAdjustment::BUTTONID_MAIN_MINUS==RangeAdjustment::actualButtonID)&&
			(PositionTask::checkPositionStatically(
									MachineSettings::protocolStructExtended.positionMainMin
									)==true)
		){

			Servo::brake();
			setSubmode(PERSONAL_Waiting);
			reportServoModeStop();
			processFieldbus();

		}else if ((rangeAdjustmentTimeoutCounter--)==0){

			Servo::brake();
			setSubmode(PERSONAL_Waiting);
			reportServoModeStop();
			processFieldbus();

		}else{

			RxMessage *message = RxMessageQueue::pop();
			if (message!=NULL){
				switch(message->tag){
				//---------------------------------------------------------------------------------------
				default:
					processFieldbus();
					break;
				//---------------------------------------------------------------------------------------
				case Protocol::TAG_PersonalExit:

					Servo::brake();
					setSubmode(WAITING_Waiting);
					reportServoModeStop();

					processFieldbus();
					break;
				//---------------------------------------------------------------------------------------
				case Protocol::TAG_PersonalButtonHold:

					if (RangeAdjustment::actualButtonID==message->value[0]){

						rangeAdjustmentTimeoutCounter = TIMEOUT_RangeAdjusmentMessage;

						//keep moving

						float frequency;
						Fieldbus::pushUSSRequest(
							USS::makeSetFrequencyRequest(
								fmPersonal.getDirection(),
								frequency=Servo::limitFrequency(
									fmPersonal.getFrequency(),
									fmPersonal.getDirection()
									)
								)
							);			
						if (fmPersonal.getDirection()==Servo::POSITIVE_DIRECTION){
							servoFrequencyPositive = frequency;
						}else{
							servoFrequencyNegative = frequency;
						}				
						reportServoModeContinue();

					}else{

						Servo::brake();
						setSubmode(PERSONAL_Waiting);
						reportServoModeStop();
						processFieldbus();
					}
					break;
				//---------------------------------------------------------------------------------------
				case Protocol::TAG_PersonalButtonReleased:
				
					Servo::brake();
					setSubmode(PERSONAL_Waiting);
					reportServoModeStop();
					processFieldbus();
					break;
				//---------------------------------------------------------------------------------------
				}
			}else{
				reportServoModeContinue();				
				processFieldbus();
			}			
		}

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
		setSubmode(PARKING_PreparingAux);

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

		if (true==RxMessageQueue::cancelMessageReceived()){

			Actuators::disable(0);
			Actuators::disable(1);
			setSubmode(WAITING_Waiting);

		}else if ((Actuators::targetPositionReached(0)==true)&&(Actuators::targetPositionReached(1)==true)){

			Actuators::disable(0);
			Actuators::disable(1);
			
			setSubmode(PARKING_Preparing);
		}

		processFieldbus();

		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_Preparing:

		//MessageQueue::flush();

		{
			bool positionTaskIsNotNeeded = 
				PositionTask::checkPositionStatically(
					PersonalSettings::protocolStruct.positionMainParking
					);

			if (positionTaskIsNotNeeded==true){

				//skip moving
				Servo::brake();				
				setSubmode(WAITING_Waiting);
				reportServoModeStop();

				processFieldbus();				

			}else{

				if (PositionTask::getDirection(PersonalSettings::protocolStruct.positionMainParking)==Servo::POSITIVE_DIRECTION){

					fmHoming.prepare(
						Parking::servoFrequencyPositive,
						PersonalSettings::protocolStruct.positionMainParking,
						Servo::POSITIVE_DIRECTION,
						FM_HOMING_LAW,
						FM_HOMING_LAW,
						FM_HOMING_MIN_RANGE
						);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmHoming.getDirection(),
							servoFrequencyPositive=fmHoming.getFrequency()
							)
						);

					Servo::movePositive();
					setSubmode(PARKING_Moving);
					reportServoModePositive();

				}else{

					fmHoming.prepare(
						Parking::servoFrequencyNegative,
						PersonalSettings::protocolStruct.positionMainParking,
						Servo::NEGATIVE_DIRECTION,
						FM_HOMING_LAW,
						FM_HOMING_LAW,
						FM_HOMING_MIN_RANGE
						);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmHoming.getDirection(),
							servoFrequencyNegative=fmHoming.getFrequency()
							)
						);

					Servo::moveNegative();
					setSubmode(PARKING_Moving);
					reportServoModeNegative();

				}
			}	
		}

		

		break;
	//-------------------------------------------------PARKING-----------------------------------
	case PARKING_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);	
			reportServoModeStop();

			processFieldbus();

		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					PersonalSettings::protocolStruct.positionMainParking,
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				Servo::brake();				
				setSubmode(WAITING_Waiting);
				reportServoModeStop();

				processFieldbus();

			}else{

				float frequency;

				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmHoming.getDirection(),
						frequency=Servo::limitFrequency(
							fmHoming.getFrequency(),
							fmHoming.getDirection()
							)
						)
					);			
				if (fmHoming.getDirection()==Servo::POSITIVE_DIRECTION){
					servoFrequencyPositive = frequency;
				}else{
					servoFrequencyNegative = frequency;
				}				

				reportServoModeContinue();
			}
		}

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Starting:

		Parking::recalculateServoFrequency();// for homing
		ForceTestDynamic::recalculateServoFrequency();// for actual testing

		setSubmode(FTEST_DYNAMIC_Homing_PreparingAux);

		processFieldbus();
		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Homing_PreparingAux:

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
		setSubmode(FTEST_DYNAMIC_Homing_MovingAux);

		processFieldbus();

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Homing_MovingAux:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Actuators::disable(0);
			Actuators::disable(1);
			setSubmode(WAITING_Waiting);

		}else if ((Actuators::targetPositionReached(0)==true)&&(Actuators::targetPositionReached(1)==true)){

			Actuators::disable(0);
			Actuators::disable(1);
			
			setSubmode(FTEST_DYNAMIC_Homing_Preparing);
		}

		processFieldbus();

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Homing_Preparing:

		{
			bool positionTaskIsNotNeeded = 
				PositionTask::checkPositionStatically(
					ForceTestDynamic::getStartPosition()
					);

			if (positionTaskIsNotNeeded==true){
				//skip homing
				Servo::brake();				
				setSubmode(FTEST_DYNAMIC_Pause);
				reportServoModeStop();

				processFieldbus();				

			}else{

				if (PositionTask::getDirection(ForceTestDynamic::getStartPosition())==Servo::POSITIVE_DIRECTION){

					fmHoming.prepare(
						Parking::servoFrequencyPositive,
						ForceTestDynamic::getStartPosition(),
						Servo::POSITIVE_DIRECTION,
						FM_HOMING_LAW,
						FM_HOMING_LAW,
						FM_HOMING_MIN_RANGE
					);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmHoming.getDirection(),
							servoFrequencyPositive=fmHoming.getFrequency()
							)
						);

					Servo::movePositive();
					setSubmode(FTEST_DYNAMIC_Homing_Moving);
					reportServoModePositive();

				}else{

					fmHoming.prepare(
						Parking::servoFrequencyNegative,
						ForceTestDynamic::getStartPosition(),
						Servo::NEGATIVE_DIRECTION,
						FM_HOMING_LAW,
						FM_HOMING_LAW,
						FM_HOMING_MIN_RANGE
					);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmHoming.getDirection(),
							servoFrequencyNegative=fmHoming.getFrequency()
							)
						);

					Servo::moveNegative();
					setSubmode(FTEST_DYNAMIC_Homing_Moving);
					reportServoModeNegative();

				}	

			}

		}

		
		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Homing_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

			processFieldbus();			

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

				processFieldbus();			

			}else{

				float frequency;

				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmHoming.getDirection(),
						frequency=Servo::limitFrequency(
							fmHoming.getFrequency(),
							fmHoming.getDirection()
							)
						)
					);

				if (fmHoming.getDirection()==Servo::POSITIVE_DIRECTION){
					servoFrequencyPositive = frequency;
				}else{
					servoFrequencyNegative = frequency;
				}

				reportServoModeContinue();

			}

		}

		

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Pause:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();	
			setSubmode(WAITING_Waiting);

		}else{

			ForceTestDynamic::pauseBeforeTestMsec -= 100;
			if (ForceTestDynamic::pauseBeforeTestMsec<=0){
				setSubmode(FTEST_DYNAMIC_Testing_Preparing);	
			}			
		}

		processFieldbus();


		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Testing_Preparing:

		if (PositionTask::getDirection(ForceTestDynamic::getStopPosition())==Servo::POSITIVE_DIRECTION){

			fmTest.prepare(
				ForceTestDynamic::servoFrequencyPositive,
				ForceTestDynamic::getStopPosition(),
				Servo::POSITIVE_DIRECTION,
				MachineSettings::protocolStructExtended.eccentricAccelerationLaw,
				MachineSettings::protocolStructExtended.eccentricDecelerationLaw,
				FM_TEST_MIN_RANGE
			);

			Fieldbus::pushUSSRequest(
				USS::makeSetFrequencyRequest(
					fmTest.getDirection(),
					servoFrequencyPositive=fmTest.getFrequency()
					)
				);
			Servo::movePositive();
			setSubmode(FTEST_DYNAMIC_Testing_Moving);
			reportServoModePositive();

		}else{

			fmTest.prepare(
				ForceTestDynamic::servoFrequencyNegative,
				ForceTestDynamic::getStopPosition(),
				Servo::NEGATIVE_DIRECTION,
				MachineSettings::protocolStructExtended.concentricAccelerationLaw,
				MachineSettings::protocolStructExtended.concentricDecelerationLaw,
				FM_TEST_MIN_RANGE
			);

			Fieldbus::pushUSSRequest(
				USS::makeSetFrequencyRequest(
					fmTest.getDirection(),
					servoFrequencyNegative=fmTest.getFrequency()
					)
				);
			Servo::moveNegative();
			setSubmode(FTEST_DYNAMIC_Testing_Moving);
			reportServoModeNegative();
		}	

		break;
	//------------------------------------------------FTEST--DYNAMIC----------------------------------
	case FTEST_DYNAMIC_Testing_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);	
			reportServoModeStop();			

			processFieldbus();

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

				processFieldbus();

			}else{

				float frequency;
				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmTest.getDirection(),
						frequency = Servo::limitFrequency(
							fmTest.getFrequency(),
							fmTest.getDirection()
							)
						)
					);

				if (fmTest.getDirection()==Servo::POSITIVE_DIRECTION){
					servoFrequencyPositive = frequency;
				}else{
					servoFrequencyNegative = frequency;
				}

				reportServoModeContinue();

			}

		}

		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Starting:

		Parking::recalculateServoFrequency();// for homing

		setSubmode(FTEST_STATIC_Homing_PreparingAux);


		processFieldbus();
		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Homing_PreparingAux:

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
		setSubmode(FTEST_STATIC_Homing_MovingAux);

		processFieldbus();

		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Homing_MovingAux:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Actuators::disable(0);
			Actuators::disable(1);
			setSubmode(WAITING_Waiting);

		}else if ((Actuators::targetPositionReached(0)==true)&&(Actuators::targetPositionReached(1)==true)){

			Actuators::disable(0);
			Actuators::disable(1);
			
			setSubmode(FTEST_STATIC_Homing_Preparing);
		}

		processFieldbus();

		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Homing_Preparing:

		{

			bool positionTaskIsNotNeeded = 
				PositionTask::checkPositionStatically(
					ForceTestStatic::getTestPosition()
					);

			if (positionTaskIsNotNeeded==true){
				//skip homing
				Servo::brake();				
				setSubmode(FTEST_STATIC_Pause);
				reportServoModeStop();

				processFieldbus();

			}else{

				if (PositionTask::getDirection(ForceTestStatic::getTestPosition())==Servo::POSITIVE_DIRECTION){

					fmHoming.prepare(
						Parking::servoFrequencyPositive,
						ForceTestStatic::getTestPosition(),
						Servo::POSITIVE_DIRECTION,
						FM_HOMING_LAW,
						FM_HOMING_LAW,
						FM_HOMING_MIN_RANGE
					);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmHoming.getDirection(),
							servoFrequencyPositive=fmHoming.getFrequency()
							)
						);

					Servo::movePositive();
					setSubmode(FTEST_STATIC_Homing_Moving);
					reportServoModePositive();

				}else{

					fmHoming.prepare(
						Parking::servoFrequencyNegative,
						ForceTestStatic::getTestPosition(),
						Servo::NEGATIVE_DIRECTION,
						FM_HOMING_LAW,
						FM_HOMING_LAW,
						FM_HOMING_MIN_RANGE
					);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmHoming.getDirection(),
							servoFrequencyNegative=fmHoming.getFrequency()
							)
						);

					Servo::moveNegative();
					setSubmode(FTEST_STATIC_Homing_Moving);
					reportServoModeNegative();
				}	

			}

		}

		break;
	//------------------------------------------------FTEST--STATIC----------------------------------
	case FTEST_STATIC_Homing_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

			processFieldbus();	

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

				processFieldbus();

			}else{

				float frequency;

				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmHoming.getDirection(),
						frequency=Servo::limitFrequency(
							fmHoming.getFrequency(),
							fmHoming.getDirection()
							)
						)
					);

				if (fmHoming.getDirection()==Servo::POSITIVE_DIRECTION){
					servoFrequencyPositive = frequency;
				}else{
					servoFrequencyNegative = frequency;
				}

				reportServoModeContinue();
			}

		}

		
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
		setSubmode(EXERCISE_Homing_Preparing);


		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Homing_Preparing:

		{
			bool positionTaskIsNotNeeded = 
				PositionTask::checkPositionStatically(
					Excercise::getPositionMainHoming()
					);

			if (positionTaskIsNotNeeded==true){
				//skip homing
				Servo::brake();				
				setSubmode(EXERCISE_Pause);
				reportServoModeStop();


			}else{

				if (PositionTask::getDirection(Excercise::getPositionMainHoming())==Servo::POSITIVE_DIRECTION){

					fmHoming.prepare(
						Parking::servoFrequencyPositive,
						Excercise::getPositionMainHoming(),
						Servo::POSITIVE_DIRECTION,
						FM_HOMING_LAW,
						FM_HOMING_LAW,
						FM_HOMING_MIN_RANGE	
					);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmHoming.getDirection(),
							servoFrequencyPositive=fmHoming.getFrequency()
							)
						);

					Servo::movePositive();
					setSubmode(EXERCISE_Homing_Moving);
					reportServoModePositive();
					
				}else{

					fmHoming.prepare(
						Parking::servoFrequencyNegative,
						Excercise::getPositionMainHoming(),
						Servo::NEGATIVE_DIRECTION,
						FM_HOMING_LAW,
						FM_HOMING_LAW,
						FM_HOMING_MIN_RANGE
						);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmHoming.getDirection(),
							servoFrequencyNegative=fmHoming.getFrequency()
							)
						);

					Servo::moveNegative();
					setSubmode(EXERCISE_Homing_Moving);
					reportServoModeNegative();

				}	

			}

		}

		processFieldbus();

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Homing_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

			processFieldbus();

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

				processFieldbus();

			}else{

				float frequency;

				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmHoming.getDirection(),
						frequency=Servo::limitFrequency(
							fmHoming.getFrequency(),
							fmHoming.getDirection()
							)
						)
					);

				if (fmHoming.getDirection()==Servo::POSITIVE_DIRECTION){
					servoFrequencyPositive = frequency;
				}else{
					servoFrequencyNegative = frequency;
				}

				reportServoModeContinue();

			}

		} 

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_Pause:

		if (true==RxMessageQueue::cancelMessageReceived()){

			setSubmode(WAITING_Waiting);

			processFieldbus();

		}else if (Excercise::isSetPauseDone()==true){

			Excercise::recalculateServoFrequency();

			Excercise::repetitionStart();
			bool firstMovementDirection = 
				PositionTask::getDirection(
					Excercise::getPositionMainFirstMovement()
					);

			if (firstMovementDirection==Servo::POSITIVE_DIRECTION){

				fmExcercise.prepare(
					Excercise::servoFrequencyPositive,
					Excercise::getPositionMainFirstMovement(),
					Servo::POSITIVE_DIRECTION,
					MachineSettings::protocolStructExtended.eccentricAccelerationLaw,
					MachineSettings::protocolStructExtended.eccentricDecelerationLaw,
					FM_EXCERCISE_MIN_RANGE
					);

				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmExcercise.getDirection(),
						servoFrequencyPositive=fmExcercise.getFrequency()
						)
					);

				Servo::movePositive();
				setSubmode(EXERCISE_FirstMovement);
				reportServoModePositive();

			}else{

				fmExcercise.prepare(
					Excercise::servoFrequencyNegative,
					Excercise::getPositionMainFirstMovement(),
					Servo::NEGATIVE_DIRECTION,
					MachineSettings::protocolStructExtended.concentricAccelerationLaw,
					MachineSettings::protocolStructExtended.concentricDecelerationLaw,
					FM_EXCERCISE_MIN_RANGE
					);

				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmExcercise.getDirection(),
						servoFrequencyNegative=fmExcercise.getFrequency()
						)
					);

				Servo::moveNegative();
				setSubmode(EXERCISE_FirstMovement);
				reportServoModeNegative();

			}


		}

		break;
	//------------------------------------------------EXCERCISE----------------------------------
	case EXERCISE_FirstMovement:

	/*
		Steps here from 
			EXERCISE_Pause
			EXERCISE_SecondMovement
			EXERCISE_SecondInterruption
	*/

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

			processFieldbus();

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

					processFieldbus();

				}else{
					//interruption disabled
					bool secondMovementDirection = 
						PositionTask::getDirection(
							Excercise::getPositionMainSecondMovement()
							);

					if (secondMovementDirection==Servo::POSITIVE_DIRECTION){

						fmExcercise.prepare(
							Excercise::servoFrequencyPositive,
							Excercise::getPositionMainSecondMovement(),
							Servo::POSITIVE_DIRECTION,
							MachineSettings::protocolStructExtended.eccentricAccelerationLaw,
							MachineSettings::protocolStructExtended.eccentricDecelerationLaw,
							FM_EXCERCISE_MIN_RANGE
							);

						Fieldbus::pushUSSRequest(
							USS::makeSetFrequencyRequest(
								fmExcercise.getDirection(),
								servoFrequencyPositive=fmExcercise.getFrequency()
								)
							);

						Servo::movePositive();
						setSubmode(EXERCISE_SecondMovement);
						reportServoModePositive();


					}else{

						fmExcercise.prepare(
							Excercise::servoFrequencyNegative,
							Excercise::getPositionMainSecondMovement(),
							Servo::NEGATIVE_DIRECTION,
							MachineSettings::protocolStructExtended.concentricAccelerationLaw,
							MachineSettings::protocolStructExtended.concentricDecelerationLaw,
							FM_EXCERCISE_MIN_RANGE
							);

						Fieldbus::pushUSSRequest(
							USS::makeSetFrequencyRequest(
								fmExcercise.getDirection(),
								servoFrequencyNegative=fmExcercise.getFrequency()
								)
							);

						Servo::moveNegative();
						setSubmode(EXERCISE_SecondMovement);
						reportServoModeNegative();

					}

				}

				Odometer::incrementDegrees((uint32_t)PersonalSettings::getMainRangeDegrees());

				

			}else{

				float frequency;
				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmExcercise.getDirection(),
						frequency = Servo::limitFrequency(
							fmExcercise.getFrequency(),
							fmExcercise.getDirection()
							)
						)
					);

				if (fmExcercise.getDirection()==Servo::POSITIVE_DIRECTION){
					servoFrequencyPositive = frequency;
				}else{
					servoFrequencyNegative = frequency;
				}

				reportServoModeContinue();

			}

		}

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

			processFieldbus();

		}else if (Excercise::isFirstInterruptionDone()==true){

			bool secondMovementDirection = 
				PositionTask::getDirection(
					Excercise::getPositionMainSecondMovement()
					);

			if (secondMovementDirection==Servo::POSITIVE_DIRECTION){

				fmExcercise.prepare(
					Excercise::servoFrequencyPositive,
					Excercise::getPositionMainSecondMovement(),
					Servo::POSITIVE_DIRECTION,
					MachineSettings::protocolStructExtended.eccentricAccelerationLaw,
					MachineSettings::protocolStructExtended.eccentricDecelerationLaw,
					FM_EXCERCISE_MIN_RANGE
					);

				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmExcercise.getDirection(),
						servoFrequencyPositive=fmExcercise.getFrequency()
						)
					);

				Servo::movePositive();
				setSubmode(EXERCISE_SecondMovement);
				reportServoModePositive();

			}else{

				fmExcercise.prepare(
					Excercise::servoFrequencyNegative,
					Excercise::getPositionMainSecondMovement(),
					Servo::NEGATIVE_DIRECTION,
					MachineSettings::protocolStructExtended.concentricAccelerationLaw,
					MachineSettings::protocolStructExtended.concentricDecelerationLaw,
					FM_EXCERCISE_MIN_RANGE
					);

				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmExcercise.getDirection(),
						servoFrequencyNegative=fmExcercise.getFrequency()
						)
					);

				Servo::moveNegative();
				setSubmode(EXERCISE_SecondMovement);
				reportServoModeNegative();

			}

		}

		

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

			processFieldbus();

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

					processFieldbus();

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

							fmExcercise.prepare(
								Excercise::servoFrequencyPositive,
								Excercise::getPositionMainFirstMovement(),
								Servo::POSITIVE_DIRECTION,
								MachineSettings::protocolStructExtended.eccentricAccelerationLaw,
								MachineSettings::protocolStructExtended.eccentricDecelerationLaw,
								FM_EXCERCISE_MIN_RANGE
								);

							Fieldbus::pushUSSRequest(
								USS::makeSetFrequencyRequest(
									fmExcercise.getDirection(),
									servoFrequencyPositive=fmExcercise.getFrequency()
									)
								);

							Servo::movePositive();
							setSubmode(EXERCISE_FirstMovement);
							reportServoModePositive();

						}else{

							fmExcercise.prepare(
								Excercise::servoFrequencyNegative,
								Excercise::getPositionMainFirstMovement(),
								Servo::NEGATIVE_DIRECTION,
								MachineSettings::protocolStructExtended.concentricAccelerationLaw,
								MachineSettings::protocolStructExtended.concentricDecelerationLaw,
								FM_EXCERCISE_MIN_RANGE							
								);

							Fieldbus::pushUSSRequest(
								USS::makeSetFrequencyRequest(
									fmExcercise.getDirection(),
									servoFrequencyNegative=fmExcercise.getFrequency()
									)
								);

							Servo::moveNegative();
							setSubmode(EXERCISE_FirstMovement);
							reportServoModeNegative();
						}
					}

				}

				Odometer::incrementDegrees((uint32_t)PersonalSettings::getMainRangeDegrees());

			}else{

				float frequency;
				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmExcercise.getDirection(),
						frequency = Servo::limitFrequency(
							fmExcercise.getFrequency(),
							fmExcercise.getDirection()
							)
						)
					);

				if (fmExcercise.getDirection()==Servo::POSITIVE_DIRECTION){
					servoFrequencyPositive = frequency;
				}else{
					servoFrequencyNegative = frequency;
				}

				reportServoModeContinue();


			}

		}

		

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

			processFieldbus();

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

				processFieldbus();				

			}else{			

				Excercise::repetitionStart();

				bool firstMovementDirection = 
					PositionTask::getDirection(
						Excercise::getPositionMainFirstMovement()
						);

				if (firstMovementDirection==Servo::POSITIVE_DIRECTION){

					fmExcercise.prepare(
						Excercise::servoFrequencyPositive,
						Excercise::getPositionMainFirstMovement(),
						Servo::POSITIVE_DIRECTION,
						MachineSettings::protocolStructExtended.eccentricAccelerationLaw,
						MachineSettings::protocolStructExtended.eccentricDecelerationLaw,
						FM_EXCERCISE_MIN_RANGE
						);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmExcercise.getDirection(),
							servoFrequencyPositive=fmExcercise.getFrequency()
							)
						);

					Servo::movePositive();
					setSubmode(EXERCISE_FirstMovement);
					reportServoModePositive();

				}else{

					fmExcercise.prepare(
						Excercise::servoFrequencyNegative,
						Excercise::getPositionMainFirstMovement(),
						Servo::NEGATIVE_DIRECTION,
						MachineSettings::protocolStructExtended.concentricAccelerationLaw,
						MachineSettings::protocolStructExtended.concentricDecelerationLaw,
						FM_EXCERCISE_MIN_RANGE
						);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmExcercise.getDirection(),
							servoFrequencyNegative=fmExcercise.getFrequency()
							)
						);

					Servo::moveNegative();
					setSubmode(EXERCISE_FirstMovement);
					reportServoModeNegative();

				}
			}
		}

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

			Parking::recalculateServoFrequency();// for homing
			
			setSubmode(GENERIC_SET_Homing_Preparing);

		}else{
			
			__asm("	nop");
		}

		processFieldbus();

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
/*	case GENERIC_SET_Homing_SettingPositiveSpeed:

		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(
			Servo::POSITIVE_DIRECTION,
			servoFrequencyPositive=Parking::servoFrequencyPositive
			));			

		setSubmode(GENERIC_SET_Homing_SettingNegativeSpeed);
	
	
		break;
*/	//------------------------------------------------GENERIC-SET---------------------------------
/*	case GENERIC_SET_Homing_SettingNegativeSpeed:

		Fieldbus::pushUSSRequest(USS::makeSetFrequencyRequest(
			Servo::NEGATIVE_DIRECTION,
			servoFrequencyNegative=Parking::servoFrequencyNegative
			));			
	
		//GenericSet::pause2Start();setSubmode(GENERIC_SET_Pause2);
		setSubmode(GENERIC_SET_Homing_PreparingMain);
	

		break;*/
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Homing_Preparing:

		{
			bool positionTaskIsNotNeeded = 
				PositionTask::checkPositionStatically(
					GenericSet::getPositionMainStart()
					);

			if (positionTaskIsNotNeeded==true){

				//skip homing
				Servo::brake();	

				GenericSet::pause2Start();
				setSubmode(GENERIC_SET_Pause2);

				reportServoModeStop();

				processFieldbus();				


			}else{

				if (PositionTask::getDirection(GenericSet::getPositionMainStart())==Servo::POSITIVE_DIRECTION){

					fmHoming.prepare(
						Parking::servoFrequencyPositive,
						GenericSet::getPositionMainStart(),
						Servo::POSITIVE_DIRECTION,
						FM_HOMING_LAW,
						FM_HOMING_LAW,
						FM_HOMING_MIN_RANGE
					);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmHoming.getDirection(),
							servoFrequencyPositive=fmHoming.getFrequency()
							)
						);

					Servo::movePositive();
					setSubmode(GENERIC_SET_Homing_Moving);
					reportServoModePositive();

				}else{

					fmHoming.prepare(
						Parking::servoFrequencyNegative,
						GenericSet::getPositionMainStart(),
						Servo::NEGATIVE_DIRECTION,
						FM_HOMING_LAW,
						FM_HOMING_LAW,
						FM_HOMING_MIN_RANGE
					);

					Fieldbus::pushUSSRequest(
						USS::makeSetFrequencyRequest(
							fmHoming.getDirection(),
							servoFrequencyNegative=fmHoming.getFrequency()
							)
						);

					Servo::moveNegative();
					setSubmode(GENERIC_SET_Homing_Moving);
					reportServoModeNegative();
				}	

			}

		}


		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Homing_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

			processFieldbus();

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

				processFieldbus();	

			}else{

				float frequency;

				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmHoming.getDirection(),
						frequency=Servo::limitFrequency(
							fmHoming.getFrequency(),
							fmHoming.getDirection()
							)
						)
					);

				if (fmHoming.getDirection()==Servo::POSITIVE_DIRECTION){
					servoFrequencyPositive = frequency;
				}else{
					servoFrequencyNegative = frequency;
				}

				reportServoModeContinue();

			}

		}

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Pause2:

		if (true==RxMessageQueue::cancelMessageReceived()){

			setSubmode(WAITING_Waiting);

		}else if (GenericSet::isPauseDone()==true){

			if (GenericSet::isMoveStatic()){

				GenericSet::staticMoveStart();
				setSubmode(GENERIC_SET_Move_Static);
			}else{
				setSubmode(GENERIC_SET_Move_Preparing);
			}

		}else{
			__asm("	nop");
		}

		processFieldbus();
	
	
		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Move_Preparing:

		if (PositionTask::getDirection(GenericSet::getMoveDestinationPosition())==Servo::POSITIVE_DIRECTION){

			GenericSet::recalculateServoFrequency(Servo::POSITIVE_DIRECTION);

			fmGenericSet.prepare(
				GenericSet::servoFrequencyPositive_,
				GenericSet::getMoveDestinationPosition(),
				Servo::POSITIVE_DIRECTION,
				MachineSettings::protocolStructExtended.eccentricAccelerationLaw,
				MachineSettings::protocolStructExtended.eccentricDecelerationLaw,
				FM_GENERIC_SET_MIN_RANGE
			);

			Fieldbus::pushUSSRequest(
				USS::makeSetFrequencyRequest(
					fmGenericSet.getDirection(),
					servoFrequencyPositive=fmGenericSet.getFrequency()
					)
				);

			Servo::movePositive();
			setSubmode(GENERIC_SET_Move_Moving);
			reportServoModePositive();

		}else{

			GenericSet::recalculateServoFrequency(Servo::NEGATIVE_DIRECTION);

			fmGenericSet.prepare(
				GenericSet::servoFrequencyNegative_,
				GenericSet::getMoveDestinationPosition(),
				Servo::NEGATIVE_DIRECTION,
				MachineSettings::protocolStructExtended.concentricAccelerationLaw,
				MachineSettings::protocolStructExtended.concentricDecelerationLaw,
				FM_GENERIC_SET_MIN_RANGE
			);

			Fieldbus::pushUSSRequest(
				USS::makeSetFrequencyRequest(
					fmGenericSet.getDirection(),
					servoFrequencyNegative=fmGenericSet.getFrequency()
					)
				);

			Servo::moveNegative();
			setSubmode(GENERIC_SET_Move_Moving);
			reportServoModeNegative();

		}	



		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Move_Moving:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();

			processFieldbus();

		}else{

			bool positionTaskIsComplete = 
				PositionTask::checkPositionDynamically(
					GenericSet::getMoveDestinationPosition(),
					Servo::getMoveDirection()
					);

			if (positionTaskIsComplete==true){

				Servo::brake();

				{
					int32_t moveDistance = GenericSet::getMoveDestinationPosition() - GenericSet::getMoveStartPosition();
					Odometer::incrementDegrees(MachineSettings::moveDistanceToDegrees(moveDistance));
				}

				GenericSet::moveComplete();

				if (GenericSet::isSetComplete()==false){

					if (GenericSet::isMoveStatic()){

						GenericSet::staticMoveStart();
						setSubmode(GENERIC_SET_Move_Static);
						reportServoModeStop();
						processFieldbus();

					}else{

						setSubmode(GENERIC_SET_Move_Preparing);
						reportServoModeStop();
						processFieldbus();

					}

				}else{

					//set complete
					setSubmode(WAITING_Waiting);
					reportServoModeStop();
					processFieldbus();

				}

				

			}else{

				float frequency;
				Fieldbus::pushUSSRequest(
					USS::makeSetFrequencyRequest(
						fmGenericSet.getDirection(),
						frequency = Servo::limitFrequency(
							fmGenericSet.getFrequency(),
							fmGenericSet.getDirection()
							)
						)
					);

				if (fmGenericSet.getDirection()==Servo::POSITIVE_DIRECTION){
					servoFrequencyPositive = frequency;
				}else{
					servoFrequencyNegative = frequency;
				}

				reportServoModeContinue();

			}

		}

		break;
	//------------------------------------------------GENERIC-SET---------------------------------
	case GENERIC_SET_Move_Static:

		if (true==RxMessageQueue::cancelMessageReceived()){

			Servo::brake();
			setSubmode(WAITING_Waiting);
			reportServoModeStop();



		}else if (GenericSet::isStaticMoveDone()){

			GenericSet::moveComplete();

			if (GenericSet::isSetComplete()==false){

				if (GenericSet::isMoveStatic()){

					GenericSet::staticMoveStart();
					setSubmode(GENERIC_SET_Move_Static);
				}else{
					setSubmode(GENERIC_SET_Move_Preparing);
				}


			}else{

				setSubmode(WAITING_Waiting);
			}

		}else{

			__asm("	nop");
		}
		reportServoModeStop();

		processFieldbus();

		break;
	//------------------------------------------------GENERIC-SET---------------------------------


	//------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	case FAULT_Fault:

		Servo::brake();
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
	Fieldbus::pushUSSRequest(USS::makeHeatsinkTemperatureRequest());

}
//==================================================================================================================





