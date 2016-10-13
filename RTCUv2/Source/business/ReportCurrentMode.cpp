

#include "MainTick.h"

#include <string.h>

#include "business/Odometer.h"
#include "business/ForceTestDynamic.h"
#include "business/ForceTestStatic.h"
#include "business/Errors.h"
#include "business/PositionTask.h"
#include "business/Excercise.h"
#include "business/ExcerciseSettings.h"

#include "hmi/diagnostics.h"
#include "hmi/hmi.h"
#include "servo/encoder.h"
#include "servo/StrainGauge.h"
#include "actuators/actuators.h"


//=================================================================================================
void MainTick::reportCurrentMode(){




	switch(mode){
	//-------------------------------------------------------------
	case INITIALIZING:

		{
			uint8_t message[MSGLEN_ReportCurrentMode_INITIALIZING];
			memset(message,0,sizeof(message));
			reportCurrentModeHeader(message);


			Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
		}


		break;
	//-------------------------------------------------------------
	case IDLE:
	case WAITING:
	case PERSONAL:
	case PARKING:

		{
			uint8_t message[MSGLEN_ReportCurrentMode_IDLE_WAITING];
			memset(message,0,sizeof(message));
			reportCurrentModeHeader(message);
			reportCurrentModePosition(message);

			Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
		}



		break;
	//-------------------------------------------------------------
	case TEST_CONCENTRIC:
	case TEST_ECCENTRIC:

		switch(submode){
		case FTEST_DYNAMIC_Starting:
		case FTEST_DYNAMIC_Homing_SettingPositiveSpeed:
		case FTEST_DYNAMIC_Homing_SettingNegativeSpeed:
		case FTEST_DYNAMIC_Homing_Preparing:
		case FTEST_DYNAMIC_Homing_Moving:
			{
				uint8_t message[MSGLEN_ReportCurrentMode_FTEST_DYNAMIC_Homing];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				memcpy(
					&message[MSGPOS_FTEST_DYNAMIC_Phase],
					&ForceTestDynamic::PHASE_HOMING,
					sizeof(int32_t)
					);



				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}

			break;
		case FTEST_DYNAMIC_Pause:
		case FTEST_DYNAMIC_Testing_SettingPositiveSpeed:
		case FTEST_DYNAMIC_Testing_SettingNegativeSpeed:
			{
				uint8_t message[MSGLEN_ReportCurrentMode_FTEST_DYNAMIC_Pause];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				memcpy(
					&message[MSGPOS_FTEST_DYNAMIC_Phase],
					&ForceTestDynamic::PHASE_PAUSE,
					sizeof(int32_t)
					);

				memcpy(
					&message[MSGPOS_FTEST_DYNAMIC_TimeToTest],
					&ForceTestDynamic::pauseBeforeTestMsec,
					sizeof(int32_t)
					);

				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}


			break;
		case FTEST_DYNAMIC_Testing_Preparing:
		case FTEST_DYNAMIC_Testing_Moving:
			{
				uint8_t message[MSGLEN_ReportCurrentMode_FTEST_DYNAMIC_Testing];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				if (ForceTestDynamic::testType==ForceTestDynamic::CONCENTRIC){
					memcpy(
						&message[MSGPOS_FTEST_DYNAMIC_Phase],
						&ForceTestDynamic::PHASE_TEST_CONCENTRIC,
						sizeof(int32_t)
						);
				}else{
					memcpy(
						&message[MSGPOS_FTEST_DYNAMIC_Phase],
						&ForceTestDynamic::PHASE_TEST_ECCENTRIC,
						sizeof(int32_t)
						);
				}

				int32_t positionRel = PositionTask::getRelativePosition();
				memcpy(
					&message[MSGPOS_FTEST_DYNAMIC_PositionRel],
					&positionRel,
					sizeof(int32_t)
					);

				int32_t forceValue = StrainGauge::getFilteredValue();
				memcpy(
					&message[MSGPOS_FTEST_DYNAMIC_Force],
					&forceValue,
					sizeof(int32_t)
					);

				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}

			break;
		}

		break;
	//-------------------------------------------------------------
	case TEST_STATIC:

		switch(submode){
		case FTEST_STATIC_Starting:
		case FTEST_STATIC_Homing_SettingPositiveSpeed:
		case FTEST_STATIC_Homing_SettingNegativeSpeed:
		case FTEST_STATIC_Homing_Preparing:
		case FTEST_STATIC_Homing_Moving:

			{
				uint8_t message[MSGLEN_ReportCurrentMode_FTEST_STATIC_Homing];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				memcpy(
					&message[MSGPOS_FTEST_STATIC_Phase],
					&ForceTestStatic::PHASE_HOMING,
					sizeof(int32_t)
					);

				int32_t positionRel = PositionTask::getRelativePosition();
				memcpy(
					&message[MSGPOS_FTEST_STATIC_PositionRel],
					&positionRel,
					sizeof(int32_t)
					);

				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}
			break;


		case FTEST_STATIC_Pause:

			{
				uint8_t message[MSGLEN_ReportCurrentMode_FTEST_STATIC_Pause];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				memcpy(
					&message[MSGPOS_FTEST_STATIC_Phase],
					&ForceTestStatic::PHASE_PAUSE,
					sizeof(int32_t)
					);

				int32_t positionRel = PositionTask::getRelativePosition();
				memcpy(
					&message[MSGPOS_FTEST_STATIC_PositionRel],
					&positionRel,
					sizeof(int32_t)
					);

				memcpy(
					&message[MSGPOS_FTEST_STATIC_TimeToTest],
					&ForceTestStatic::pauseBeforeTestMsec,
					sizeof(int32_t)
					);


				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}
			break;


		case FTEST_STATIC_Testing:

			{
				uint8_t message[MSGLEN_ReportCurrentMode_FTEST_STATIC_Testing];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				memcpy(
					&message[MSGPOS_FTEST_STATIC_Phase],
					&ForceTestStatic::PHASE_TEST_STATIC,
					sizeof(int32_t)
					);

				int32_t positionRel = PositionTask::getRelativePosition();
				memcpy(
					&message[MSGPOS_FTEST_STATIC_PositionRel],
					&positionRel,
					sizeof(int32_t)
					);

				int32_t forceValue = StrainGauge::getFilteredValue();
				memcpy(
					&message[MSGPOS_FTEST_STATIC_Force],
					&forceValue,
					sizeof(int32_t)
					);

				memcpy(
					&message[MSGPOS_FTEST_STATIC_TimeToEnd],
					&ForceTestStatic::testDurationMsec,
					sizeof(int32_t)
					);


				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}		
			break;
		}


		break;
	//-------------------------------------------------------------
	case EXERCISE_ISOKINETIC:

		switch(submode){
		//----------------------------------------------------------------EXERCISE_ISOKINETIC---submode------------------------------
		case EXERCISE_Starting:
		case EXERCISE_Homing_PreparingAux:
		case EXERCISE_Homing_MovingAux:
		case EXERCISE_StartingSet:
		case EXERCISE_Homing_SettingPositiveSpeed:
		case EXERCISE_Homing_SettingNegativeSpeed:
		case EXERCISE_Homing_PreparingMain:
		case EXERCISE_Homing_MovingMain:

			{
				uint8_t message[MSGLEN_ReportCurrentMode_EXCERCISE_Homing];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				int32_t phase = Excercise::PHASE_HOMING;
				memcpy(
					&message[MSGPOS_EXCERCISE_Phase],
					&phase,
					sizeof(int32_t)
					);

				int32_t setIndex = Excercise::getActualSetIndex();
				memcpy(
					&message[MSGPOS_EXCERCISE_SetIndex],
					&setIndex,
					sizeof(int32_t)
					);

				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}

			break;
		//----------------------------------------------------------------EXERCISE_ISOKINETIC---submode------------------------------
		case EXERCISE_Pause:
		case EXERCISE_SettingPositiveSpeed:
		case EXERCISE_SettingNegativeSpeed:

			{
				uint8_t message[MSGLEN_ReportCurrentMode_EXCERCISE_Pause];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				int32_t phase = Excercise::PHASE_PAUSE;
				memcpy(
					&message[MSGPOS_EXCERCISE_Phase],
					&phase,
					sizeof(int32_t)
					);

				int32_t setIndex = Excercise::getActualSetIndex();
				memcpy(
					&message[MSGPOS_EXCERCISE_SetIndex],
					&setIndex,
					sizeof(int32_t)
					);

				int32_t timeToSet = Excercise::getActualTimeToSet();
				memcpy(
					&message[MSGPOS_EXCERCISE_TimeToSet],
					&timeToSet,
					sizeof(int32_t)
					);

				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}
			break;
		//----------------------------------------------------------------EXERCISE_ISOKINETIC---submode------------------------------
		case EXERCISE_FirstMovement:

			{
				uint8_t message[MSGLEN_ReportCurrentMode_EXCERCISE_Movement];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				int32_t phase = Excercise::PHASE_ISOKINETIC_FIRSTMOVE;
				memcpy(
					&message[MSGPOS_EXCERCISE_Phase],
					&phase,
					sizeof(int32_t)
					);

				int32_t setIndex = Excercise::getActualSetIndex();
				memcpy(
					&message[MSGPOS_EXCERCISE_SetIndex],
					&setIndex,
					sizeof(int32_t)
					);

				int32_t repIndex = Excercise::getActualepetitionIndex();
				memcpy(
					&message[MSGPOS_EXCERCISE_RepIndex],
					&repIndex,
					sizeof(int32_t)
					);

				int32_t repDirection = Excercise::getActualRepDirection(Excercise::PHASE_ISOKINETIC_FIRSTMOVE);
				memcpy(
					&message[MSGPOS_EXCERCISE_RepDirection],
					&repDirection,
					sizeof(int32_t)
					);

				int32_t positionRel = PositionTask::getRelativePosition();
				memcpy(
					&message[MSGPOS_EXCERCISE_PositionRel],
					&positionRel,
					sizeof(int32_t)
					);

				int32_t force = StrainGauge::getFilteredValue();
				memcpy(
					&message[MSGPOS_EXCERCISE_Force],
					&force,
					sizeof(int32_t)
					);

				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}
			break;
		//----------------------------------------------------------------EXERCISE_ISOKINETIC---submode------------------------------
		case EXERCISE_FirstInterruption:

			{
				uint8_t message[MSGLEN_ReportCurrentMode_EXCERCISE_Interruption];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				int32_t phase = Excercise::PHASE_ISOKINETIC_FIRSTINTERRUPTION;
				memcpy(
					&message[MSGPOS_EXCERCISE_Phase],
					&phase,
					sizeof(int32_t)
					);

				int32_t setIndex = Excercise::getActualSetIndex();
				memcpy(
					&message[MSGPOS_EXCERCISE_SetIndex],
					&setIndex,
					sizeof(int32_t)
					);

				int32_t repIndex = Excercise::getActualepetitionIndex();
				memcpy(
					&message[MSGPOS_EXCERCISE_RepIndex],
					&repIndex,
					sizeof(int32_t)
					);

				int32_t timeToMovement = Excercise::getActualTimeToMovement(Excercise::PHASE_ISOKINETIC_FIRSTINTERRUPTION);
				memcpy(
					&message[MSGPOS_EXCERCISE_TimeToMovement],
					&timeToMovement,
					sizeof(int32_t)
					);

				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}
			break;
		//----------------------------------------------------------------EXERCISE_ISOKINETIC---submode------------------------------
		case EXERCISE_SecondMovement:

			{
				uint8_t message[MSGLEN_ReportCurrentMode_EXCERCISE_Movement];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				int32_t phase = Excercise::PHASE_ISOKINETIC_SECONDMOVE;
				memcpy(
					&message[MSGPOS_EXCERCISE_Phase],
					&phase,
					sizeof(int32_t)
					);

				int32_t setIndex = Excercise::getActualSetIndex();
				memcpy(
					&message[MSGPOS_EXCERCISE_SetIndex],
					&setIndex,
					sizeof(int32_t)
					);

				int32_t repIndex = Excercise::getActualepetitionIndex();
				memcpy(
					&message[MSGPOS_EXCERCISE_RepIndex],
					&repIndex,
					sizeof(int32_t)
					);

				int32_t repDirection = Excercise::getActualRepDirection(Excercise::PHASE_ISOKINETIC_SECONDMOVE);
				memcpy(
					&message[MSGPOS_EXCERCISE_RepDirection],
					&repDirection,
					sizeof(int32_t)
					);

				int32_t positionRel = PositionTask::getRelativePosition();
				memcpy(
					&message[MSGPOS_EXCERCISE_PositionRel],
					&positionRel,
					sizeof(int32_t)
					);

				int32_t force = StrainGauge::getFilteredValue();
				memcpy(
					&message[MSGPOS_EXCERCISE_Force],
					&force,
					sizeof(int32_t)
					);

				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}

			break;
		//----------------------------------------------------------------EXERCISE_ISOKINETIC---submode------------------------------
		case EXERCISE_SecondInterruption:

			{
				uint8_t message[MSGLEN_ReportCurrentMode_EXCERCISE_Interruption];
				memset(message,0,sizeof(message));
				reportCurrentModeHeader(message);
				reportCurrentModePosition(message);

				int32_t phase = Excercise::PHASE_ISOKINETIC_SECONDINTERRUPTION;
				memcpy(
					&message[MSGPOS_EXCERCISE_Phase],
					&phase,
					sizeof(int32_t)
					);

				int32_t setIndex = Excercise::getActualSetIndex();
				memcpy(
					&message[MSGPOS_EXCERCISE_SetIndex],
					&setIndex,
					sizeof(int32_t)
					);

				int32_t repIndex = Excercise::getActualepetitionIndex();
				memcpy(
					&message[MSGPOS_EXCERCISE_RepIndex],
					&repIndex,
					sizeof(int32_t)
					);

				int32_t timeToMovement = Excercise::getActualTimeToMovement(Excercise::PHASE_ISOKINETIC_SECONDINTERRUPTION);
				memcpy(
					&message[MSGPOS_EXCERCISE_TimeToMovement],
					&timeToMovement,
					sizeof(int32_t)
					);

				Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			}
			break;
		}
		//----------------------------------------------------------------EXERCISE_ISOKINETIC---submode------------------------------
		break;
	//-------------------------------------------------------------
	case ERROR:

		{
			uint8_t message[MSGLEN_ReportCurrentMode_ERROR];
			memset(message,0,sizeof(message));
			reportCurrentModeHeader(message);

			message[MSGPOS_ErrorCode] = Errors::getErrorCode();



			Diagnostics::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
			HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,message,sizeof(message));
		}




		break;
	//-------------------------------------------------------------




	}



}
//=================================================================================================
void MainTick::reportCurrentModeHeader(uint8_t *message){

	NV::RealTimeClock::TCurrentDateTime currentDateTime;
	NV::RealTimeClock::getCurrentDateTime(currentDateTime);


	message[MSGPOS_Mode] = mode;
	memcpy(&message[MSGPOS_CurrentDateTime],&currentDateTime,sizeof(currentDateTime));
	uint32_t ododmeterMinutes = Odometer::getTimeMinutes();
	memcpy(&message[MSGPOS_OdometerTime],&ododmeterMinutes,sizeof(ododmeterMinutes));
	uint32_t ododmeterMms = 0;
	memcpy(&message[MSGPOS_OdometerDistance],&ododmeterMms,sizeof(ododmeterMms));


}
//=================================================================================================
void MainTick::reportCurrentModePosition(uint8_t *message){

	uint16_t encoderValue16 = Encoder::getValue();
	int32_t encoderValue32 = (int32_t)encoderValue16;
	memcpy(
		&message[MSGPOS_PositionMain],
		&encoderValue32,
		sizeof(int32_t)
		);


	int32_t actuatorValue0 = Actuators::getPosition(0);
	memcpy(
		&message[MSGPOS_PositionAux1],
		&actuatorValue0,
		sizeof(int32_t)
		);

	int32_t actuatorValue1 = Actuators::getPosition(1);
	memcpy(
		&message[MSGPOS_PositionAux2],
		&actuatorValue1,
		sizeof(int32_t)
		);









}
//=================================================================================================