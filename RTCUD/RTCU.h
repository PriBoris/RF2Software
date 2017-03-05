#ifndef RTCU_H
#define RTCU_H

#include <QObject>
#include <QMap>

class RTCU{

public:

    static void init();


	//----------------------------------------------------------------------------------
    class Mode{
    private:
        static QMap<quint8,QString> titles;

 
    public:

		static const quint8 INITIALIZING = 0;
		static const quint8 IDLE = 1;
		static const quint8 WAITING = 2;
		static const quint8 PARKING = 3;
		static const quint8 PERSONAL = 4;
		static const quint8 TEST_CONCENTRIC = 5;
		static const quint8 TEST_ECCENTRIC = 6;
		static const quint8 EXCERCISE_ISOKINETIC = 7;
		static const quint8 FAULT = 8;
		static const quint8 TEST_STATIC = 9;
        static const quint8 GENERIC_SET = 10;


        static void init();
        static QString getTitle(quint8 index);
    };

	//----------------------------------------------------------------------------------
    class Submode{
    private:
        static QMap<quint8,QString> titles;
    public:

        static void init();
        static QString getTitle(quint8 index);

		static const quint8 INITIALIZING_JustStarted = 0;
		static const quint8 INITIALIZING_HMIPoweringOff = 1;
		static const quint8 INITIALIZING_HMIPoweringOn = 2;
		static const quint8 INITIALIZING_HMIStarting = 3;
		static const quint8 INITIALIZING_WaitingStableServoConnection = 4;
		static const quint8 IDLE_Idle = 5;
		static const quint8 WAITING_Waiting =6;
		static const quint8 PERSONAL_Starting = 7;
		static const quint8 PERSONAL_SettingPositiveSpeed = 8;
		static const quint8 PERSONAL_SettingNegativeSpeed = 9;
		static const quint8 PERSONAL_Waiting = 10;
		static const quint8 PERSONAL_MovingMain = 11;
		static const quint8 PERSONAL_MovingAux =12;
		static const quint8 PARKING_Starting = 13;
		static const quint8 PARKING_SettingPositiveSpeed = 14;
		static const quint8 PARKING_SettingNegativeSpeed = 15;
		static const quint8 PARKING_PreparingMain = 16;
		static const quint8 PARKING_MovingMain = 17;
		static const quint8 PARKING_PreparingAux = 18;
		static const quint8 PARKING_MovingAux = 19;
		static const quint8 FTEST_DYNAMIC_Starting = 20;
		static const quint8 FTEST_DYNAMIC_Homing_SettingPositiveSpeed = 21;
		static const quint8 FTEST_DYNAMIC_Homing_SettingNegativeSpeed = 22;
		static const quint8 FTEST_DYNAMIC_Homing_Preparing = 23;
		static const quint8 FTEST_DYNAMIC_Homing_Moving = 24;
		static const quint8 FTEST_DYNAMIC_Pause = 25;
		static const quint8 FTEST_DYNAMIC_Testing_SettingPositiveSpeed = 26;
		static const quint8 FTEST_DYNAMIC_Testing_SettingNegativeSpeed = 27;
		static const quint8 FTEST_DYNAMIC_Testing_Preparing = 28;
		static const quint8 FTEST_DYNAMIC_Testing_Moving = 29;
		static const quint8 FTEST_STATIC_Starting = 30;
		static const quint8 FTEST_STATIC_Homing_SettingPositiveSpeed = 31;
		static const quint8 FTEST_STATIC_Homing_SettingNegativeSpeed = 32;
		static const quint8 FTEST_STATIC_Homing_Preparing = 33;
		static const quint8 FTEST_STATIC_Homing_Moving = 34;
		static const quint8 FTEST_STATIC_Pause = 35;
		static const quint8 FTEST_STATIC_Testing = 36;
		static const quint8 EXERCISE_Starting = 37;
		static const quint8 EXERCISE_Homing_PreparingAux = 38;
		static const quint8 EXERCISE_Homing_MovingAux = 39;
		static const quint8 EXERCISE_StartingSet = 40;
		static const quint8 EXERCISE_Homing_SettingPositiveSpeed = 41;
		static const quint8 EXERCISE_Homing_SettingNegativeSpeed = 42;
		static const quint8 EXERCISE_Homing_PreparingMain = 43;
		static const quint8 EXERCISE_Homing_MovingMain = 44;
		static const quint8 EXERCISE_Pause = 45;
		static const quint8 EXERCISE_SettingPositiveSpeed = 46;
		static const quint8 EXERCISE_SettingNegativeSpeed = 47;
		static const quint8 EXERCISE_FirstMovement = 48;
		static const quint8 EXERCISE_FirstInterruption = 49;
		static const quint8 EXERCISE_SecondMovement = 50;
		static const quint8 EXERCISE_SecondInterruption = 51;


        static const quint8 GENERIC_SET_Starting = 52;
        static const quint8 GENERIC_SET_Homing_PreparingAux = 53;
        static const quint8 GENERIC_SET_Homing_MovingAux = 54;
        static const quint8 GENERIC_SET_Pause1 = 55;
		static const quint8 GENERIC_SET_Homing_SettingPositiveSpeed = 56;
		static const quint8 GENERIC_SET_Homing_SettingNegativeSpeed = 57;
		static const quint8 GENERIC_SET_Homing_PreparingMain = 58;
		static const quint8 GENERIC_SET_Homing_MovingMain = 59;		
		static const quint8 GENERIC_SET_Pause2 = 60;
		static const quint8 GENERIC_SET_Move_Preparing = 61;
		static const quint8 GENERIC_SET_Move_Moving = 62;
		static const quint8 GENERIC_SET_Move_Static = 63;



        static const quint8 ERROR_Error = 64;


    };



	//----------------------------------------------------------------------------------
    class Phase{
    private:
        static QMap<qint32,QString> titles;
 
    public:

        static const qint32 HOMING=0;
        static const qint32 PAUSE=1;
        static const qint32 TEST_CONCENTRIC=2;
        static const qint32 TEST_ECCENTRIC=3;
        static const qint32 ISOKINETIC_FIRSTMOVE=4;
        static const qint32 ISOKINETIC_FIRSTINTERRUPTION=5;
        static const qint32 ISOKINETIC_SECONDMOVE=6;
        static const qint32 ISOKINETIC_SECONDINTERRUPTION=7;
        static const qint32 TEST_STATIC=8;
        static const qint32 PAUSE1=9;
        static const qint32 PAUSE2=10;
        static const qint32 GENERIC_MOVE=11;

        static void init();
        static QString getTitle(qint32 index);
    };

	//----------------------------------------------------------------------------------
	struct TPersonalSettings{

	    qint32 positionMainA;
	    qint32	positionMainB;
	    qint32	positionMainParking;
	    qint32	positionAux1;
	    qint32	positionAux2;
	    qint32	positionAux3;
	    qint32	speedAbsMainParking;
	    qint32	speedRelMainTestEccentric;
	    qint32	speedRelMainTestConcentric;
	    qint32 forceCurveAB[21];
	    qint32 forceCurveBA[21];

	};

	//----------------------------------------------------------------------------------
	struct TMachineSettingsExtended{

		qint32 positionMainMax;
		qint32 positionMainMin;

		qint32 positionAux1Max;
		qint32 positionAux1Min;
		qint32 positionAux2Max;
		qint32 positionAux2Min;
		qint32 positionAux3Max;
		qint32 positionAux3Min;

		qint32 speedAbsMainMax;
		qint32 speedAbsMainPersonal;

		qint32 encoderBitCount;
		qint32 encoderDirection;
		qint32 encoderOffset;

		float forceSensorGain;
		float forceSensorOffset0;
		float forceSensorOffset1;
		float forceSensorOffset2;

		float minAbsServoFrequency;
		float maxAbsServoFrequency;

		qint32 concentricAccelerationLaw;
		qint32 concentricDecelerationLaw;
		qint32 eccentricAccelerationLaw;
		qint32 eccentricDecelerationLaw;
		
	};
	//----------------------------------------------------------------------------------
	struct TMachineSettings{

	    qint32	positionMainMax;
	    qint32	positionMainMin;
	    qint32	positionAux1Max;
	    qint32	positionAux1Min;
	    qint32	positionAux2Max;
	    qint32	positionAux2Min;
	    qint32	positionAux3Max;
	    qint32	positionAux3Min;
	    qint32 speedAbsMainMax;
	    qint32 speedAbsMainPersonal;

	};
	//----------------------------------------------------------------------------------
    
};

#endif // RTCU_H
