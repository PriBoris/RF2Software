

#ifndef MAIN_TICK_H
#define MAIN_TICK_H

#include <stdint.h>

#include "../nv/rtc.h"


class MainTick{

private:

	//-----------------------------------------------------------

	enum Submode{

		INITIALIZING_JustStarted = 0,
		INITIALIZING_HMIPoweringOff,
		INITIALIZING_HMIPoweringOn,
		INITIALIZING_HMIStarting,
		INITIALIZING_WaitingStableServoConnection,

		IDLE_Idle,

		WAITING_Waiting,

		PERSONAL_Starting,
		PERSONAL_SettingPositiveSpeed,
		PERSONAL_SettingNegativeSpeed,
		PERSONAL_Waiting,
		PERSONAL_MovingMain,
		PERSONAL_MovingAux,

		PARKING_Starting,
		PARKING_SettingPositiveSpeed,
		PARKING_SettingNegativeSpeed,
		PARKING_PreparingMain,
		PARKING_MovingMain,
		PARKING_PreparingAux,
		PARKING_MovingAux,

		FTEST_DYNAMIC_Starting,
		FTEST_DYNAMIC_Homing_SettingPositiveSpeed,
		FTEST_DYNAMIC_Homing_SettingNegativeSpeed,
		FTEST_DYNAMIC_Homing_Preparing,
		FTEST_DYNAMIC_Homing_Moving,
		FTEST_DYNAMIC_Pause,
		FTEST_DYNAMIC_Testing_SettingPositiveSpeed,
		FTEST_DYNAMIC_Testing_SettingNegativeSpeed,
		FTEST_DYNAMIC_Testing_Preparing,
		FTEST_DYNAMIC_Testing_Moving,

		FTEST_STATIC_Starting,
		FTEST_STATIC_Homing_SettingPositiveSpeed,
		FTEST_STATIC_Homing_SettingNegativeSpeed,
		FTEST_STATIC_Homing_Preparing,
		FTEST_STATIC_Homing_Moving,
		FTEST_STATIC_Pause,
		FTEST_STATIC_Testing,


		EXERCISE_Starting,
		EXERCISE_Homing_PreparingAux,
		EXERCISE_Homing_MovingAux,
		EXERCISE_StartingSet,
		EXERCISE_Homing_SettingPositiveSpeed,
		EXERCISE_Homing_SettingNegativeSpeed,
		EXERCISE_Homing_PreparingMain,
		EXERCISE_Homing_MovingMain,
		EXERCISE_Pause,
		EXERCISE_SettingPositiveSpeed,
		EXERCISE_SettingNegativeSpeed,
		EXERCISE_FirstMovement,
		EXERCISE_FirstInterruption,
		EXERCISE_SecondMovement,
		EXERCISE_SecondInterruption,


		ERROR_Error,
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
		ERROR=8,
	  	TEST_STATIC=9,


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
	
	

	//-----------------------------------------------------------

	static const uint16_t MSGLEN_Mode = 1;
	static const uint16_t MSGLEN_CurrentDateTime = sizeof(NV::RealTimeClock::TCurrentDateTime);
	static const uint16_t MSGLEN_OdometerTimeWay = 8;
	static const uint16_t MSGLEN_Position = 16;
	static const uint16_t MSGLEN_Phase = 4;
	static const uint16_t MSGLEN_SetIndex = 4;
	static const uint16_t MSGLEN_RepIndex = 4;
	static const uint16_t MSGLEN_RepDirection = 4;
	static const uint16_t MSGLEN_PositionRel = 4;
	static const uint16_t MSGLEN_Force = 4;
	static const uint16_t MSGLEN_TimeToSet = 4;
	static const uint16_t MSGLEN_TimeToMovement = 4;


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

	//-----------------------------------------------------------

	static const uint16_t MSGLEN_ReportServoMode_DEFAULT = 
		sizeof(uint32_t)+
		sizeof(NV::RealTimeClock::TCurrentDateTime)+
		4*sizeof(uint8_t)+
		0;



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

	static uint32_t tickID;

public:

	static void init();
	static void process();



};




#endif
