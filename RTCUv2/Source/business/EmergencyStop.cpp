


#include "EmergencyStop.h"

#include "stm32f4xx_conf.h"

bool EmergencyStop::pressed(){
	return ((GPIOB->IDR&(1<<7))==0);
}





