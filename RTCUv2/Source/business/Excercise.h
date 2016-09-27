
#ifndef EXCERCISE_H
#define EXCERCISE_H

#include <stdint.h>


class Excercise{

public:

	static const int32_t PHASE_HOMING;
	static const int32_t PHASE_PAUSE;
	static const int32_t PHASE_ISOKINETIC_FIRSTMOVE;
	static const int32_t PHASE_ISOKINETIC_FIRSTINTERRUPTION;
	static const int32_t PHASE_ISOKINETIC_SECONDMOVE;
	static const int32_t PHASE_ISOKINETIC_SECONDINTERRUPTION;

	static void excerciseStart();
	static void setStart();
	static void repetitionStart();

	static int32_t actualSet;
	static int32_t actualRepetition;

	static int32_t getPositionMainHoming();


	static int32_t setPauseCounterMsec;
	static bool isSetPauseDone();

	static float servoFrequencyPositive;
	static float servoFrequencyNegative;
	static void recalculateServoFrequency();

	static int32_t getPositionMainFirstMovement();
	static int32_t getPositionMainSecondMovement();



	static int32_t firstInterruptionCounterMsec;
	static int32_t secondInterruptionCounterMsec;

	static bool firstInterruptionEnabled();
	static bool secondInterruptionEnabled();

	static bool isFirstInterruptionDone();
	static bool isSecondInterruptionDone();

	static void repetitionDone();
	static bool isSetDone();

	static bool isExcerciseDone();

	static int32_t getActualSetIndex();
	static int32_t getActualepetitionIndex();
	static int32_t getActualTimeToSet();
	static int32_t getActualRepDirection(int32_t phase);
	static int32_t getActualTimeToMovement(int32_t phase);

	static int32_t getSpeedFirstMovement();
	static int32_t getSpeedSecondMovement();



};


#endif
