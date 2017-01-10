#include "RTCU.h"

QMap<quint8,QString> RTCU::Mode::titles;


void RTCU::init(){

	Mode::titles[Mode::INITIALIZING] = "INITIALIZING";
	Mode::titles[Mode::IDLE] = "IDLE";
	Mode::titles[Mode::WAITING] = "WAITING";
	Mode::titles[Mode::PARKING] = "PARKING";
	Mode::titles[Mode::PERSONAL] = "PERSONAL";
	Mode::titles[Mode::TEST_CONCENTRIC] = "TEST_CONCENTRIC";
	Mode::titles[Mode::TEST_ECCENTRIC] = "TEST_ECCENTRIC";
	Mode::titles[Mode::EXCERCISE_ISOKINETIC] = "EXCERCISE_ISOKINETIC";
	Mode::titles[Mode::FAULT] = "FAULT";
	Mode::titles[Mode::TEST_STATIC] = "TEST_STATIC";
	Mode::titles[Mode::GENERIC_EXERCISE_ISOKINETIC] = "GENERIC_EXERCISE_ISOKINETIC";

}

QString RTCU::Mode::getTitle(quint8 index){

	return titles[index];

}


