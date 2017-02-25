
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

	case PERSONAL_Waiting:
	case PERSONAL_MovingMain:
	case PERSONAL_MovingAux:
		mode = PERSONAL;
		break;

	case PARKING_Starting:
	case PARKING_Preparing:
	case PARKING_Moving:
	case PARKING_PreparingAux:
	case PARKING_MovingAux:
		mode = PARKING;
		break;

	case FTEST_DYNAMIC_Starting:
	case FTEST_DYNAMIC_Homing_Preparing:
	case FTEST_DYNAMIC_Homing_Moving:
	case FTEST_DYNAMIC_Pause:
	case FTEST_DYNAMIC_Testing_Preparing:
	case FTEST_DYNAMIC_Testing_Moving:

		if (ForceTestDynamic::testType==ForceTestDynamic::CONCENTRIC){
			mode = TEST_CONCENTRIC;
		}else{
			mode = TEST_ECCENTRIC;
		}
		break;

	case FTEST_STATIC_Starting:
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
	case EXERCISE_Homing_Preparing:
	case EXERCISE_Homing_Moving:
	case EXERCISE_Pause:
	case EXERCISE_FirstMovement:
	case EXERCISE_FirstInterruption:
	case EXERCISE_SecondMovement:
	case EXERCISE_SecondInterruption:

		mode = EXERCISE_ISOKINETIC;
		break;

	case GENERIC_SET_Starting:
	case GENERIC_SET_Homing_PreparingAux:
	case GENERIC_SET_Homing_MovingAux:
	case GENERIC_SET_Pause1:
	case GENERIC_SET_Homing_Preparing:
	case GENERIC_SET_Homing_Moving:
	case GENERIC_SET_Pause2:
	case GENERIC_SET_Move_Preparing:
	case GENERIC_SET_Move_Moving:
	case GENERIC_SET_Move_Static:



		mode = GENERIC_SET;
		break;

	case FAULT_Fault:
		mode = FAULT;
		break;
	}



}
//==================================================================================================================

