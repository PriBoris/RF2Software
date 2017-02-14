

#ifndef MAIN_TICK_H
#define MAIN_TICK_H

#include <stdint.h>

#include "../nv/rtc.h"


class MainTick{

private:



	//-----------------------------------------------------------

	enum Submode{

		INITIALIZING_JustStarted = 0,
		INITIALIZING_HMIPoweringOff = 1,
		INITIALIZING_HMIPoweringOn = 2,
		INITIALIZING_HMIStarting = 3,
		INITIALIZING_WaitingStableServoConnection = 4,
		//TODO: write 0Hz to servo before any use
		IDLE_Idle = 5,
		WAITING_Waiting =6,
		PERSONAL_Starting = 7,
		PERSONAL_SettingPositiveSpeed = 8,
		PERSONAL_SettingNegativeSpeed = 9,
		PERSONAL_Waiting = 10,
		PERSONAL_MovingMain = 11,
		PERSONAL_MovingAux =12,
		PARKING_Starting = 13,
		PARKING_SettingPositiveSpeed = 14,
		PARKING_SettingNegativeSpeed = 15,
		PARKING_PreparingMain = 16,
		PARKING_MovingMain = 17,
		PARKING_PreparingAux = 18,
		PARKING_MovingAux = 19,
		FTEST_DYNAMIC_Starting = 20,
		FTEST_DYNAMIC_Homing_SettingPositiveSpeed = 21,
		FTEST_DYNAMIC_Homing_SettingNegativeSpeed = 22,
		FTEST_DYNAMIC_Homing_Preparing = 23,
		FTEST_DYNAMIC_Homing_Moving = 24,
		FTEST_DYNAMIC_Pause = 25,
//		FTEST_DYNAMIC_Testing_SettingPositiveSpeed = 26,
//		FTEST_DYNAMIC_Testing_SettingNegativeSpeed = 27,
		FTEST_DYNAMIC_Testing_Preparing = 28,
		FTEST_DYNAMIC_Testing_Moving = 29,
		FTEST_STATIC_Starting = 30,
		FTEST_STATIC_Homing_SettingPositiveSpeed = 31,
		FTEST_STATIC_Homing_SettingNegativeSpeed = 32,
		FTEST_STATIC_Homing_Preparing = 33,
		FTEST_STATIC_Homing_Moving = 34,
		FTEST_STATIC_Pause = 35,
		FTEST_STATIC_Testing = 36,

		EXERCISE_Starting = 37,
		EXERCISE_Homing_PreparingAux = 38,
		EXERCISE_Homing_MovingAux = 39,
		EXERCISE_StartingSet = 40,
		EXERCISE_Homing_SettingPositiveSpeed = 41,
		EXERCISE_Homing_SettingNegativeSpeed = 42,
		EXERCISE_Homing_PreparingMain = 43,
		EXERCISE_Homing_MovingMain = 44,
		EXERCISE_Pause = 45,
		EXERCISE_SettingPositiveSpeed = 46,
		EXERCISE_SettingNegativeSpeed = 47,
		EXERCISE_FirstMovement = 48,
		EXERCISE_FirstInterruption = 49,
		EXERCISE_SecondMovement = 50,
		EXERCISE_SecondInterruption = 51,

		GENERIC_SET_Starting = 52,
		GENERIC_SET_Homing_PreparingAux = 53,
		GENERIC_SET_Homing_MovingAux = 54,
		GENERIC_SET_Pause1 = 55,
		GENERIC_SET_Homing_SettingPositiveSpeed = 56,
		GENERIC_SET_Homing_SettingNegativeSpeed = 57,
		GENERIC_SET_Homing_PreparingMain = 58,
		GENERIC_SET_Homing_MovingMain = 59,
		GENERIC_SET_Pause2 = 60,
		GENERIC_SET_Move_Preparing = 61,
		GENERIC_SET_Move_Moving = 62,
		GENERIC_SET_Move_Static = 63,



		FAULT_Fault = 64,
	};
	static Submode submode;
	static Submode submodePrev;


	enum Mode{
		INITIALIZING=0,
		IDLE=1,
		WAITING=2,
		PARKING=3,
	 	PERSONAL=4,
	  	TEST_CONCENTRIC=5,
	  	TEST_ECCENTRIC=6,
	 	EXERCISE_ISOKINETIC=7,
		FAULT=8,
	  	TEST_STATIC=9,
	  	GENERIC_SET=10,
	};

	static Mode mode;
	static Mode modePrev;

	static void setSubmode(Submode newSubmode);

	static void reportCurrentMode();
	static void reportCurrentModeHeader(uint8_t *message);
	static void reportCurrentModePosition(uint8_t *message);

	static void reportMachineSettings();

	static bool reportServoModeIssued;
	static void reportServoModeDefault();
	static void reportServoModeHeader(uint8_t *message);

	static void reportServoMode(int32_t servoCommand);


	static void reportServoModePositive();
	static void reportServoModeNegative();
	static void reportServoModeStop();
	static void reportServoModeContinue();
	

	static int32_t actualServoCommand;
	static const int32_t REPORT_SERVO_MODE_STOP = 0;
	static const int32_t REPORT_SERVO_MODE_POSITIVE = 1;
	static const int32_t REPORT_SERVO_MODE_NEGATIVE = -1;
	
	static float servoFrequencyPositive;
	static float servoFrequencyNegative;
	//-----------------------------------------------------------


	static const uint16_t MSGLEN_Mode = sizeof(uint8_t);
	static const uint16_t MSGLEN_CurrentDateTime = sizeof(NV::RealTimeClock::TCurrentDateTime);
	static const uint16_t MSGLEN_OdometerTimeWay = 2*sizeof(int32_t);
	static const uint16_t MSGLEN_Position = 4*sizeof(int32_t);
	static const uint16_t MSGLEN_Phase = sizeof(int32_t);
	static const uint16_t MSGLEN_SetIndex = sizeof(int32_t);
	static const uint16_t MSGLEN_MoveIndex = sizeof(int32_t);
	static const uint16_t MSGLEN_RepIndex = sizeof(int32_t);
	static const uint16_t MSGLEN_RepDirection = sizeof(int32_t);
	static const uint16_t MSGLEN_PositionRel = sizeof(int32_t);
	static const uint16_t MSGLEN_Force = sizeof(int32_t);
	static const uint16_t MSGLEN_TimeToSet = sizeof(int32_t);
	static const uint16_t MSGLEN_TimeToMovement = sizeof(int32_t);
	static const uint16_t MSGLEN_PauseTimeRemaining = sizeof(int32_t);


	static const uint16_t MSGLEN_ReportCurrentMode_INITIALIZING = (1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay);
	static const uint16_t MSGLEN_ReportCurrentMode_IDLE_WAITING = (1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position);
	static const uint16_t MSGLEN_ReportCurrentMode_ERROR = (1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+1);

	static const uint16_t MSGLEN_ReportCurrentMode_FTEST_DYNAMIC_Homing = (1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4);
	static const uint16_t MSGLEN_ReportCurrentMode_FTEST_DYNAMIC_Pause = (1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4+4);
	static const uint16_t MSGLEN_ReportCurrentMode_FTEST_DYNAMIC_Testing = (1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4+4+4);

	static const uint16_t MSGLEN_ReportCurrentMode_FTEST_STATIC_Homing = (1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4+4);
	static const uint16_t MSGLEN_ReportCurrentMode_FTEST_STATIC_Pause = (1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4+4+4);
	static const uint16_t MSGLEN_ReportCurrentMode_FTEST_STATIC_Testing = (1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4+4+4+4);



	static const uint16_t MSGLEN_ReportCurrentMode_EXCERCISE_Homing = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+MSGLEN_SetIndex+
		0;//39

	static const uint16_t MSGLEN_ReportCurrentMode_EXCERCISE_Pause = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+MSGLEN_SetIndex+
		MSGLEN_TimeToSet+
		0;//43

	static const uint16_t MSGLEN_ReportCurrentMode_EXCERCISE_Movement = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+MSGLEN_SetIndex+
		MSGLEN_RepIndex+
		MSGLEN_RepDirection+
		MSGLEN_PositionRel+
		MSGLEN_Force+
		0;//55

	static const uint16_t MSGLEN_ReportCurrentMode_EXCERCISE_Interruption = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+MSGLEN_SetIndex+
		MSGLEN_TimeToMovement+
		0;//43

	static const uint16_t MSGLEN_ReportCurrentMode_GENERIC_SET_Pause = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+MSGLEN_PauseTimeRemaining+
		0;//39

	static const uint16_t MSGLEN_ReportCurrentMode_GENERIC_SET_Homing = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		0;//35

	static const uint16_t MSGLEN_ReportCurrentMode_GENERIC_SET_Move = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		MSGLEN_MoveIndex+
		MSGLEN_PositionRel+
		MSGLEN_Force+
		0;//47


	static const uint16_t MSGPOS_Mode = 0;
	static const uint16_t MSGPOS_CurrentDateTime = 1;
	static const uint16_t MSGPOS_OdometerTime = 1+sizeof(NV::RealTimeClock::TCurrentDateTime);
	static const uint16_t MSGPOS_OdometerDistance = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+4;
	static const uint16_t MSGPOS_PositionMain = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+8+0*4;
	static const uint16_t MSGPOS_PositionAux1 = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+8+1*4;
	static const uint16_t MSGPOS_PositionAux2 = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+8+2*4;
	static const uint16_t MSGPOS_PositionAux3 = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+8+3*4;
	static const uint16_t MSGPOS_ErrorCode = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+8;
	static const uint16_t MSGPOS_FTEST_DYNAMIC_Phase = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position;
	static const uint16_t MSGPOS_FTEST_DYNAMIC_TimeToTest = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4;
	static const uint16_t MSGPOS_FTEST_DYNAMIC_PositionRel = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4;
	static const uint16_t MSGPOS_FTEST_DYNAMIC_Force = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4+4;
	static const uint16_t MSGPOS_FTEST_STATIC_Phase = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position;
	static const uint16_t MSGPOS_FTEST_STATIC_PositionRel = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4;
	static const uint16_t MSGPOS_FTEST_STATIC_TimeToTest = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4+4;
	static const uint16_t MSGPOS_FTEST_STATIC_Force = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4+4;
	static const uint16_t MSGPOS_FTEST_STATIC_TimeToEnd = 1+sizeof(NV::RealTimeClock::TCurrentDateTime)+MSGLEN_OdometerTimeWay+MSGLEN_Position+4+4+4;


	static const uint16_t MSGPOS_EXCERCISE_Phase = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		0;
	static const uint16_t MSGPOS_EXCERCISE_SetIndex = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		0;
	static const uint16_t MSGPOS_EXCERCISE_TimeToSet = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		MSGLEN_SetIndex+
		0;
	static const uint16_t MSGPOS_EXCERCISE_RepIndex = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		MSGLEN_SetIndex+
		0;
	static const uint16_t MSGPOS_EXCERCISE_RepDirection = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		MSGLEN_SetIndex+
		MSGLEN_RepIndex+
		0;
	static const uint16_t MSGPOS_EXCERCISE_PositionRel = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		MSGLEN_SetIndex+
		MSGLEN_RepIndex+
		MSGLEN_RepDirection+
		0;
	static const uint16_t MSGPOS_EXCERCISE_Force = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		MSGLEN_SetIndex+
		MSGLEN_RepIndex+
		MSGLEN_RepDirection+
		MSGLEN_PositionRel+
		0;
	static const uint16_t MSGPOS_EXCERCISE_TimeToMovement = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		MSGLEN_SetIndex+
		0;

	static const uint16_t MSGPOS_GENERIC_SET_Phase = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		0;

	static const uint16_t MSGPOS_GENERIC_SET_PauseTimeRemaining = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		0;

	static const uint16_t MSGPOS_GENERIC_SET_MoveIndex = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		0;
	static const uint16_t MSGPOS_GENERIC_SET_PositionRel = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		MSGLEN_MoveIndex+
		0;
	static const uint16_t MSGPOS_GENERIC_SET_Force = 
		MSGLEN_Mode+MSGLEN_CurrentDateTime+MSGLEN_OdometerTimeWay+MSGLEN_Position+
		MSGLEN_Phase+
		MSGLEN_MoveIndex+
		MSGLEN_PositionRel+
		0;



	//-----------------------------------------------------------

	static const uint16_t MSGLEN_ReportServoMode_DEFAULT = 
		sizeof(uint32_t)+
		sizeof(NV::RealTimeClock::TCurrentDateTime)+
		4*sizeof(uint8_t)+//mode submode before after
		2*sizeof(float)+//frequencies
		1*sizeof(int32_t)+//position
		3*sizeof(int32_t)+//temperatures
		0;

	static const uint16_t MSGLEN_ReportServoMode = 
		sizeof(uint32_t)+
		sizeof(NV::RealTimeClock::TCurrentDateTime)+
		4*sizeof(uint8_t)+//mode submode before after
		2*sizeof(float)+//frequencies
		1*sizeof(int32_t)+//position
		3*sizeof(int32_t)+//temperatures
		1*sizeof(int32_t)+//command
		0;
		

	static const uint32_t MSGPOS_ReportServoMode_heatsinkTemperature = 26;
	static const uint32_t MSGPOS_ReportServoMode_internalTemperature = 30;
	static const uint32_t MSGPOS_ReportServoMode_motorTemperature = 34;

	static const uint32_t MSGPOS_ReportServoMode_servoCommand = 38;
		


	//-----------------------------------------------------------

	static const uint_fast8_t PAUSE_HMIPoweringOff = 10;
	static const uint_fast8_t PAUSE_HMIPoweringOn = 40;
	static const uint_fast8_t PAUSE_HMIStarting = 30;
	static const uint_fast8_t PAUSE_ReportSettings = 10;
	static const uint_fast8_t TIMEOUT_RangeAdjusmentMessage = 3;


//-----------------------------------------------------------

	static uint_fast8_t rangeAdjustmentTimeoutCounter;

//-----------------------------------------------------------

	static void profilerStart();
	static void profilerStop();
	static uint32_t profilerStartTimestamp;
	static uint32_t profilerLastValue;
	static uint32_t profilerMaxValue;
	static Submode profilerMaxValueSubmode;
	static Submode profilerStartSubmode;

//-----------------------------------------------------------

	static void processFieldbus();
	static uint32_t fieldbusErrorCounter;
	static uint32_t fieldbusErrorCounterMax;
	static const uint32_t FIELDBUS_FAULT_TRESHOLD = 4;


	static uint32_t tickID;

public:

	static void init();
	static void process();



};




#endif
