
#include "Errors.h"


uint32_t Errors::allFlags=0;

//=================================================================================================
void Errors::init(){


	allFlags = 0;



}
//=================================================================================================
void Errors::setFlag(uint32_t flag){
	allFlags |= 1<<flag;
}
//=================================================================================================
bool Errors::asserted(){
	return (allFlags!=0);
}
//=================================================================================================
uint8_t Errors::getErrorCode(){

	if (allFlags&(1<<FLAG_ENCODER)){

		return ERROR_PositionMainSensorLost;
	}else if (allFlags&(1<<FLAG_USS_RESPONSE)){

		return ERROR_ServoConnectionLost;
	}else if (allFlags&(1<<FLAG_EMERGENCY_STOP)){

		return ERROR_StoppedManually;
	}else if (allFlags&(1<<FLAG_RFID)){

		return ERROR_RfidReaderLost;
	}else{
		return ERROR_Unknown;
	}





}
//=================================================================================================


