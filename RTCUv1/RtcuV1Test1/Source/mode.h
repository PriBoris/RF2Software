
#ifndef MODE_H
#define MODE_H

#include "RtcuV1Test1.h"

extern void modeInit();
extern void modeProcess();

struct FhppInputData
{
	uint8_t scon;
	uint8_t spos;
	uint8_t sdir;
	uint8_t actualValue1;
	int32_t actualValue2;
};
struct FhppOutputData
{
	uint8_t ccon;
	uint8_t cpos;
	uint8_t cdir;
	uint8_t setpointValue1;
	uint32_t setpointValue2;
};


enum Mode
{
	MODE_INITIALIZING=0,
	MODE_IDLE=1,
	MODE_WAITING=2,
	MODE_PARKING=3,
	MODE_PERSONAL=4,
	MODE_TEST_CONCENTRIC=5,
	MODE_TEST_ECCENTRIC=6,
	MODE_EXCERCISE_ISOKINETIC=7,
	MODE_ERROR=8
	
};
extern Mode mode;

enum Submode
{
	SUBMODE_INITIALIZING_Start=0,
	SUBMODE_INITIALIZING_WaitingServoConnection,
	SUBMODE_INITIALIZING_DisablingSoftwareAccess,
	SUBMODE_IDLE,

	SUBMODE_WAITING_HaltingServo,
	SUBMODE_WAITING_ReceivingSettings,
	
	SUBMODE_PARKING_Running1,
	SUBMODE_PARKING_Running2,
	SUBMODE_PARKING_Running3,
	
	SUBMODE_PERSONAL_Waiting,
	SUBMODE_PERSONAL_StartMovement1,
	SUBMODE_PERSONAL_StartMovement2,
	SUBMODE_PERSONAL_KeepMovement,
	SUBMODE_PERSONAL_StopMovement1,
	SUBMODE_PERSONAL_StopMovement2,

	SUBMODE_TEST_Parking1,
	SUBMODE_TEST_Parking2,
	SUBMODE_TEST_Parking3,
	SUBMODE_TEST_Pause,
	SUBMODE_TEST_Running1,
	SUBMODE_TEST_Running2,
	SUBMODE_TEST_Running3,
	
	SUBMODE_EXCERCISE_ISOKINETIC_Start,
	SUBMODE_EXCERCISE_ISOKINETIC_Parking1,
	SUBMODE_EXCERCISE_ISOKINETIC_Parking2,
	SUBMODE_EXCERCISE_ISOKINETIC_Parking3,
	SUBMODE_EXCERCISE_ISOKINETIC_Pause,
	SUBMODE_EXCERCISE_ISOKINETIC_PrepareFirstMovement,
	SUBMODE_EXCERCISE_ISOKINETIC_StartFirstMovement1,
	SUBMODE_EXCERCISE_ISOKINETIC_StartFirstMovement2,
	SUBMODE_EXCERCISE_ISOKINETIC_KeepFirstMovement,
	SUBMODE_EXCERCISE_ISOKINETIC_FirstInterruption,
	SUBMODE_EXCERCISE_ISOKINETIC_StartSecondMovement1,
	SUBMODE_EXCERCISE_ISOKINETIC_StartSecondMovement2,
	SUBMODE_EXCERCISE_ISOKINETIC_KeepSecondMovement,
	SUBMODE_EXCERCISE_ISOKINETIC_SecondInterruption,

	
	SUBMODE_ERROR,
	
	SUBMODE_CANCEL_1,
	SUBMODE_CANCEL_2,
	
	
};
extern Submode submode;



#define SCON_ENABLED	(1<<0)
#define SCON_OPEN	(1<<1)
#define SCON_WARN	(1<<2)
#define SCON_FAULT	(1<<3)
#define SCON_VLOAD	(1<<4)
#define SCON_FCT	(1<<5)
#define SCON_OPM	(3<<6)

#define CCON_ENABLE	(1<<0)
#define CCON_STOP	(1<<1)
#define CCON_BRAKE	(1<<2)
#define CCON_RESET	(1<<3)
#define CCON_LOCK	(1<<5)
#define CCON_OPM	(3<<6)

#define CCON_OPM_DIRECT	(1<<6)

#define CPOS_HALT	(1<<0)
#define CPOS_START	(1<<1)

#define SPOS_MC	(1<<2)

#define CDIR_POSITION_MODE	((0<<1)+(0<<2))
#define CDIR_TORQUE_MODE	((1<<1)+(0<<2))
#define CDIR_VELOCITY_MODE	((0<<1)+(1<<2))



enum ErrorType
{
	ERROR_ServoInternal=0,
	ERROR_ServoConnectionLost=1,
	ERROR_ServoVoltageLost=2,
	ERROR_ServoControlLost=3,
	ERROR_RtcuLogicError=4,
	ERROR_AuxMoved=5,
	ERROR_HmiParametersError=6,
	ERROR_StrainGaugeLost=7,
	ERROR_StoppedManually=8,
	
	
};
extern ErrorType errorType;


extern uint16_t modbusTxTransactionIdentifier;

extern void servoReadHoldingRegisters();
extern void servoPresetMultipleRegisters(FhppOutputData &data);
extern void servoGetFhppInputData(FhppInputData *data,uint8_t *rxMessage);
extern bool servoCheckFhppInputData(FhppInputData *data);

extern void setErrorMode(ErrorType type);



struct PersonalSettings
{
	int32_t	positionMainA;
	int32_t	positionMainB;
	int32_t	positionMainParking;
	int32_t	positionAux1;
	int32_t	positionAux2;
	int32_t	positionAux3;
	int32_t	speedAbsMainParking;
	int32_t	speedRelMainTestEccentric;
	int32_t	speedRelMainTestConcentric;
	int32_t forceCurveAB[21];
	int32_t forceCurveBA[21];
	
};

struct PersonalSettingsEx
{
	int32_t maxSpeedRel;
	int32_t minSpeedRel;
	double speedRelToAbsConversionCoeff;
};

struct MachineSettings
{
	int32_t	positionMainMax;
	int32_t	positionMainMin;
	int32_t	positionAux1Max;
	int32_t	positionAux1Min;
	int32_t	positionAux2Max;
	int32_t	positionAux2Min;
	int32_t	positionAux3Max;
	int32_t	positionAux3Min;
	int32_t speedAbsMainMax;
	int32_t speedAbsMainPersonal;
};

struct IsokineticSetSettings
{
	int32_t pauseTime;
	int32_t firstInterruptionTime;
	int32_t secondInterruptionTime;
	int32_t	startPoint;
	int32_t	speedAB;
	int32_t	speedBA;
	int32_t	repsCount;
};
#define MAX_SET_COUNT	(50)
extern int32_t isokineticSetCount;
extern IsokineticSetSettings isokineticSetSettings[MAX_SET_COUNT];


extern PersonalSettings personalSettings;
extern PersonalSettingsEx personalSettingsEx;
extern bool personalSettingsOk;
extern MachineSettings machineSettings;
extern bool excerciseSettingsOk;

extern uint8_t getPersonalSpeed(void);
extern uint8_t getParkingSpeed(void);
extern uint8_t getSpeedRel(int32_t speedRel);


#define MODBUS_READ_RESPONSE_LENGTH	(17)
#define MODBUS_WRITE_RESPONSE_LENGTH	(12)



//#define DEFAULT_PARKING_POSITION	(5000)
//#define DEFAULT_PARKING_SPEED	(100)
//#define DEFAULT_PERSONAL_SPEED	(5)
//#define DEFAULT_TEST_SPEED	(30)
//#define DEFAULT_EXCERCISE_SPEED	(30)

extern uint8_t convertSpeedHmiToServo(uint32_t hmiSpeed);
extern uint32_t convertSpeedServoToHmi(uint8_t servoSpeed);


enum
{
	BUTTONID_MAIN_INCREMENT=0,
	BUTTONID_MAIN_DECREMENT=1,
	BUTTONID_AUX1_INCREMENT=2,
	BUTTONID_AUX1_DECREMENT=3,
	BUTTONID_AUX2_INCREMENT=4,
	BUTTONID_AUX2_DECREMENT=5,
	BUTTONID_AUX3_INCREMENT=6,
	BUTTONID_AUX3_DECREMENT=7
};

extern int32_t getRelativePosition(int32_t absolutePosition);

enum Phase {
	PHASE_HOMING=0,
	PHASE_PAUSE=1,
	PHASE_TEST_CONCENTRIC=2,
	PHASE_TEST_ECCENTRIC=3,
	PHASE_ISOKINETIC_FIRSTMOVE=4,
	PHASE_ISOKINETIC_FIRSTINTERRUPTION=5,
	PHASE_ISOKINETIC_SECONDMOVE=6,
	PHASE_ISOKINETIC_SECONDINTERRUPTION=7,
};
extern Phase phase;

#endif

