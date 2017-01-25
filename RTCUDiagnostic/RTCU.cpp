#include "RTCU.h"

QMap<quint8,QString> RTCU::Mode::titles;
QMap<quint8,QString> RTCU::Submode::titles;



//=================================================================================================
void RTCU::init(){

	Mode::init();
	Submode::init();








}
//=================================================================================================
void RTCU::Mode::init(){

	titles[(quint8)INITIALIZING] = "INITIALIZING";
	titles[(quint8)IDLE] = "IDLE";
	titles[(quint8)WAITING] = "WAITING";
	titles[(quint8)PARKING] = "PARKING";
	titles[(quint8)PERSONAL] = "PERSONAL";
	titles[(quint8)TEST_CONCENTRIC] = "TEST_CONCENTRIC";
	titles[(quint8)TEST_ECCENTRIC] = "TEST_ECCENTRIC";
	titles[(quint8)EXCERCISE_ISOKINETIC] = "EXCERCISE_ISOKINETIC";
	titles[(quint8)FAULT] = "FAULT";
	titles[(quint8)TEST_STATIC] = "TEST_STATIC";
	titles[(quint8)GENERIC_SET] = "GENERIC_SET";

}
//=================================================================================================
void RTCU::Submode::init(){

	titles[(quint8)INITIALIZING_JustStarted] = "INITIALIZING_JustStarted";
	titles[(quint8)INITIALIZING_HMIPoweringOff] = "INITIALIZING_HMIPoweringOff";
	titles[(quint8)INITIALIZING_HMIPoweringOn] = "INITIALIZING_HMIPoweringOn";
	titles[(quint8)INITIALIZING_HMIStarting] = "INITIALIZING_HMIStarting";
	titles[(quint8)INITIALIZING_WaitingStableServoConnection] = "INITIALIZING_WaitingStableServoConnection";
	titles[(quint8)IDLE_Idle] = "IDLE_Idle";
	titles[(quint8)WAITING_Waiting] = "WAITING_Waiting";
	titles[(quint8)PERSONAL_Starting] = "PERSONAL_Starting";
	titles[(quint8)PERSONAL_SettingPositiveSpeed] = "PERSONAL_SettingPositiveSpeed";
	titles[(quint8)PERSONAL_SettingNegativeSpeed] = "PERSONAL_SettingNegativeSpeed";
	titles[(quint8)PERSONAL_Waiting] = "PERSONAL_Waiting";
	titles[(quint8)PERSONAL_MovingMain] = "PERSONAL_MovingMain";
	titles[(quint8)PERSONAL_MovingAux] = "PERSONAL_MovingAux";
	titles[(quint8)PARKING_Starting] = "PARKING_Starting";
	titles[(quint8)PARKING_SettingPositiveSpeed] = "PARKING_SettingPositiveSpeed";
	titles[(quint8)PARKING_SettingNegativeSpeed] = "PARKING_SettingNegativeSpeed";
	titles[(quint8)PARKING_PreparingMain] = "PARKING_PreparingMain";
	titles[(quint8)PARKING_MovingMain] = "PARKING_MovingMain";
	titles[(quint8)PARKING_PreparingAux] = "PARKING_PreparingAux";
	titles[(quint8)PARKING_MovingAux] = "PARKING_MovingAux";
	titles[(quint8)FTEST_DYNAMIC_Starting] = "FTEST_DYNAMIC_Starting";
	titles[(quint8)FTEST_DYNAMIC_Homing_SettingPositiveSpeed] = "FTEST_DYNAMIC_Homing_SettingPositiveSpeed";
	titles[(quint8)FTEST_DYNAMIC_Homing_SettingNegativeSpeed] = "FTEST_DYNAMIC_Homing_SettingNegativeSpeed";
	titles[(quint8)FTEST_DYNAMIC_Homing_Preparing] = "FTEST_DYNAMIC_Homing_Preparing";
	titles[(quint8)FTEST_DYNAMIC_Homing_Moving] = "FTEST_DYNAMIC_Homing_Moving";
	titles[(quint8)FTEST_DYNAMIC_Pause] = "FTEST_DYNAMIC_Pause";
	titles[(quint8)FTEST_DYNAMIC_Testing_SettingPositiveSpeed] = "FTEST_DYNAMIC_Testing_SettingPositiveSpeed";
	titles[(quint8)FTEST_DYNAMIC_Testing_SettingNegativeSpeed] = "FTEST_DYNAMIC_Testing_SettingNegativeSpeed";
	titles[(quint8)FTEST_DYNAMIC_Testing_Preparing] = "FTEST_DYNAMIC_Testing_Preparing";
	titles[(quint8)FTEST_DYNAMIC_Testing_Moving] = "FTEST_DYNAMIC_Testing_Moving";
	titles[(quint8)FTEST_STATIC_Starting] = "FTEST_STATIC_Starting";
	titles[(quint8)FTEST_STATIC_Homing_SettingPositiveSpeed] = "FTEST_STATIC_Homing_SettingPositiveSpeed";
	titles[(quint8)FTEST_STATIC_Homing_SettingNegativeSpeed] = "FTEST_STATIC_Homing_SettingNegativeSpeed";
	titles[(quint8)FTEST_STATIC_Homing_Preparing] = "FTEST_STATIC_Homing_Preparing";
	titles[(quint8)FTEST_STATIC_Homing_Moving] = "FTEST_STATIC_Homing_Moving";
	titles[(quint8)FTEST_STATIC_Pause] = "FTEST_STATIC_Pause";
	titles[(quint8)FTEST_STATIC_Testing] = "FTEST_STATIC_Testing";
	titles[(quint8)EXERCISE_Starting] = "EXERCISE_Starting";
	titles[(quint8)EXERCISE_Homing_PreparingAux] = "EXERCISE_Homing_PreparingAux";
	titles[(quint8)EXERCISE_Homing_MovingAux] = "EXERCISE_Homing_MovingAux";
	titles[(quint8)EXERCISE_StartingSet] = "EXERCISE_StartingSet";
	titles[(quint8)EXERCISE_Homing_SettingPositiveSpeed] = "EXERCISE_Homing_SettingPositiveSpeed";
	titles[(quint8)EXERCISE_Homing_SettingNegativeSpeed] = "EXERCISE_Homing_SettingNegativeSpeed";
	titles[(quint8)EXERCISE_Homing_PreparingMain] = "EXERCISE_Homing_PreparingMain";
	titles[(quint8)EXERCISE_Homing_MovingMain] = "EXERCISE_Homing_MovingMain";
	titles[(quint8)EXERCISE_Pause] = "EXERCISE_Pause";
	titles[(quint8)EXERCISE_SettingPositiveSpeed] = "EXERCISE_SettingPositiveSpeed";
	titles[(quint8)EXERCISE_SettingNegativeSpeed] = "EXERCISE_SettingNegativeSpeed";
	titles[(quint8)EXERCISE_FirstMovement] = "EXERCISE_FirstMovement";
	titles[(quint8)EXERCISE_FirstInterruption] = "EXERCISE_FirstInterruption";
	titles[(quint8)EXERCISE_SecondMovement] = "EXERCISE_SecondMovement";
	titles[(quint8)EXERCISE_SecondInterruption] = "EXERCISE_SecondInterruption";

	titles[(quint8)GENERIC_SET_Starting] = "GENERIC_SET_Starting";
	titles[(quint8)GENERIC_SET_Homing_PreparingAux] = "GENERIC_SET_Homing_PreparingAux";
	titles[(quint8)GENERIC_SET_Homing_MovingAux] = "GENERIC_SET_Homing_MovingAux";
	titles[(quint8)GENERIC_SET_Pause1] = "GENERIC_SET_Pause1";
	titles[(quint8)GENERIC_SET_Homing_SettingPositiveSpeed] = "GENERIC_SET_Homing_SettingPositiveSpeed";
	titles[(quint8)GENERIC_SET_Homing_SettingNegativeSpeed] = "GENERIC_SET_Homing_SettingNegativeSpeed";
	titles[(quint8)GENERIC_SET_Homing_PreparingMain] = "GENERIC_SET_Homing_PreparingMain";
	titles[(quint8)GENERIC_SET_Homing_MovingMain] = "GENERIC_SET_Homing_MovingMain";
	titles[(quint8)GENERIC_SET_Pause2] = "GENERIC_SET_Pause2";
	titles[(quint8)GENERIC_SET_Move_Preparing] = "GENERIC_SET_Move_Preparing";
	titles[(quint8)GENERIC_SET_Move_Moving] = "GENERIC_SET_Move_Moving";

	
	titles[(quint8)ERROR_Error] = "ERROR_Error";


}
//=================================================================================================
QString RTCU::Mode::getTitle(quint8 index){
	return titles[index];
}
QString RTCU::Submode::getTitle(quint8 index){
	return titles[index];
}
//=================================================================================================


