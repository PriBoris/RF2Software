#ifndef RTCU_H
#define RTCU_H

#include <QObject>
#include <QMap>

class RTCU{

public:

    static void init();


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




        static const quint8 ERROR_Error = 61;


    };


    
};

#endif // RTCU_H
