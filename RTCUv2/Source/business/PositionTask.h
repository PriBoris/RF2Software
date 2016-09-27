
#ifndef POSITION_TASK_H
#define POSITION_TASK_H

#include <stdint.h>

class PositionTask{

public:

	static bool checkPosition(int32_t taskPosition);
	static bool checkPosition(int32_t taskPosition,int32_t taskSpeed);


	static bool getDirection(int32_t taskPosition);

	static int32_t getRelativePosition(bool saturate = true);

	static int32_t relPositionToAbsPosition(int32_t relPosition);

};



#endif
