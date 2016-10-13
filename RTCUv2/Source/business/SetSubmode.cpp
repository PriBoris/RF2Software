
#include "MainTick.h"

#include "business/ForceTestDynamic.h"



//==================================================================================================================
void MainTick::setSubmode(MainTick::Submode newSubmode){


	//TODO: add cheks which prohibit changing between modes unless from waiting mode


	submode = newSubmode;
	switch(submode){
	default:
		while(true);//TODO: goto error state
	case INITIALIZING_JustStarted:
	case INITIALIZING_HMIPoweringOff:
	case INITIALIZING_HMIPoweringOn:
	case INITIALIZING_HMIStarting:

	case INITIALIZING_WaitingStableServoConnection:
		mode = INITIALIZING;
		break;

	case IDLE_Idle:
		mode = IDLE;
		break;
	case WAITING_Waiting:
		mode = WAITING;
		break;

	case PERSONAL_Starting:
	case PERSONAL_SettingPositiveSpeed:
	case PERSONAL_SettingNegativeSpeed:		
	case PERSONAL_Waiting:
	case PERSONAL_MovingMain:
	case PERSONAL_MovingAux:
		mode = PERSONAL;
		break;

	case PARKING_Starting:
	case PARKING_SettingPositiveSpeed:
	case PARKING_SettingNegativeSpeed:
	case PARKING_PreparingMain:
	case PARKING_MovingMain:
	case PARKING_PreparingAux:
	case PARKING_MovingAux:
		mode = PARKING;
		break;

	case FTEST_DYNAMIC_Starting:
	case FTEST_DYNAMIC_Homing_SettingPositiveSpeed:
	case FTEST_DYNAMIC_Homing_SettingNegativeSpeed:
	case FTEST_DYNAMIC_Homing_Preparing:
	case FTEST_DYNAMIC_Homing_Moving:
	case FTEST_DYNAMIC_Pause:
	case FTEST_DYNAMIC_Testing_SettingPositiveSpeed:
	case FTEST_DYNAMIC_Testing_SettingNegativeSpeed:
	case FTEST_DYNAMIC_Testing_Preparing:
	case FTEST_DYNAMIC_Testing_Moving:

		if (ForceTestDynamic::testType==ForceTestDynamic::CONCENTRIC){
			mode = TEST_CONCENTRIC;
		}else{
			mode = TEST_ECCENTRIC;
		}
		break;

	case FTEST_STATIC_Starting:
	case FTEST_STATIC_Homing_SettingPositiveSpeed:
	case FTEST_STATIC_Homing_SettingNegativeSpeed:
	case FTEST_STATIC_Homing_Preparing:
	case FTEST_STATIC_Homing_Moving:
	case FTEST_STATIC_Pause:
	case FTEST_STATIC_Testing:
		mode = TEST_STATIC;
		break;

	case EXERCISE_Starting:
	case EXERCISE_Homing_PreparingAux:
	case EXERCISE_Homing_MovingAux:
	case EXERCISE_StartingSet:
	case EXERCISE_Homing_SettingPositiveSpeed:
	case EXERCISE_Homing_SettingNegativeSpeed:
	case EXERCISE_Homing_PreparingMain:
	case EXERCISE_Homing_MovingMain:
	case EXERCISE_Pause:
	case EXERCISE_SettingPositiveSpeed:
	case EXERCISE_SettingNegativeSpeed:
	case EXERCISE_FirstMovement:
	case EXERCISE_FirstInterruption:
	case EXERCISE_SecondMovement:
	case EXERCISE_SecondInterruption:


		mode = EXERCISE_ISOKINETIC;
		break;


	case ERROR_Error:
		mode = ERROR;
		break;
	}



}
//==================================================================================================================
