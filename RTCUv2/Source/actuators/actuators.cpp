
#include "actuators.h"

#include "stm32f4xx_conf.h"

bool Actuators::enabled[actuatorCount];
int32_t Actuators::targetPosition[actuatorCount];
int32_t Actuators::maxPosition[actuatorCount];
int32_t Actuators::minPosition[actuatorCount];



uint_fast8_t Actuators::timeSlots[actuatorCount]; 
Actuators::Action Actuators::actions[actuatorCount];

const uint_fast8_t Actuators::TIME_SLOT_MAX_EXTEND[actuatorCount] = {20,7};
const uint_fast8_t Actuators::TIME_SLOT_MAX_CONTRACT[actuatorCount] = {5,7};
const uint_fast8_t Actuators::TIME_SLOT_MAX_HOLD = 6;



//======================================================================================
void Actuators::controlInit(){




	for(uint_fast8_t act=0;act<actuatorCount;act++){

		enabled[act] = false;
		bridgeDeassert(act);
		timeSlots[act] = 0;
		actions[act] = HOLD;
	}		

}
//======================================================================================
void Actuators::bridgeAssert(uint_fast8_t actuatorIndex,bool extend){
	
	
	
	
	if (extend==false){
		
		switch(actuatorIndex){
		case 0:
			GPIOB->BSRRH = (1<<13);// deassert HighL
			GPIOE->BSRRH = (1<<15);// deassert LowR
			GPIOA->BSRRL = (1<<4);// assert LowL
			GPIOD->BSRRL = (1<<9);// assert HighR
			break;
		case 1:
			
			GPIOB->BSRRH = (1<<12);// deassert HighL
			GPIOE->BSRRH = (1<<13);// deassert LowR
			GPIOC->BSRRL = (1<<4);// assert LowL
			GPIOD->BSRRL = (1<<8);// assert HighR
		
			break;
		}
		


	} else {

		switch(actuatorIndex){
		case 0:
			GPIOA->BSRRH = (1<<4);// deassert LowL
			GPIOD->BSRRH = (1<<9);// deassert HighR
			GPIOB->BSRRL = (1<<13);// assert HighL
			GPIOE->BSRRL = (1<<15);// assert LowR
			break;
		case 1:
			
			GPIOC->BSRRH = (1<<4);// deassert LowL
			GPIOD->BSRRH = (1<<8);// deassert HighR
			GPIOB->BSRRL = (1<<12);// assert HighL
			GPIOE->BSRRL = (1<<13);// assert LowR
		
			break;
		}

	}
}

//======================================================================================
void Actuators::bridgeDeassert(uint_fast8_t actuatorIndex){
	
	switch(actuatorIndex){
	case 0:
		GPIOA->BSRRH = (1<<4);// deassert LowL
		GPIOD->BSRRH = (1<<9);// deassert HighR
		GPIOB->BSRRH = (1<<13);// deassert HighL
		GPIOE->BSRRH = (1<<15);// deassert LowR
		break;
	case 1:

		GPIOB->BSRRH = (1<<12);// deassert HighL
		GPIOE->BSRRH = (1<<13);// deassert LowR
		GPIOC->BSRRH = (1<<4);// deassert LowL
		GPIOD->BSRRH = (1<<8);// deassert HighR
	
	
		break;
	}

}

//======================================================================================
void Actuators::bridgeBrake(uint_fast8_t actuatorIndex){

	switch(actuatorIndex){
	case 0:
		GPIOA->BSRRL = (1<<4);// assert LowL
		GPIOE->BSRRL = (1<<15);// assert LowR

		GPIOD->BSRRH = (1<<9);// deassert HighR
		GPIOB->BSRRH = (1<<13);// deassert HighL
	
		break;
	case 1:

		GPIOC->BSRRL = (1<<4);// assert LowL
		GPIOE->BSRRL = (1<<13);// assert LowR

		GPIOB->BSRRH = (1<<12);// deassert HighL
		GPIOD->BSRRH = (1<<8);// deassert HighR
	
		break;
	}
	

	
}
//======================================================================================
void Actuators::control(){

	for(uint_fast8_t act=0;act<actuatorCount;act++){
		
		if (enabled[act]==true){

			if (timeSlots[act]==0){

				actions[act] = HOLD;

				int32_t targetPositionHi = targetPosition[act]+FEEDBACK_VALUE_HYSTERESIS;
				int32_t targetPositionLo = targetPosition[act]-FEEDBACK_VALUE_HYSTERESIS;
				if (targetPositionLo<FEEDBACK_VALUE_MIN){
					targetPositionLo=FEEDBACK_VALUE_MIN;
				}
				if (targetPositionHi>FEEDBACK_VALUE_MAX){
					targetPositionHi=FEEDBACK_VALUE_MAX;
				}

				int32_t maxPositionHi = maxPosition[act] + FEEDBACK_VALUE_HYSTERESIS;
				int32_t minPositionLo = minPosition[act] - FEEDBACK_VALUE_HYSTERESIS;
				//TODO: more checks might be necessary


				if (feedbackValue[act]<=targetPositionLo){
					actions[act] = EXTEND;
				}
				if (feedbackValue[act]>=targetPositionHi){
					actions[act] = CONTRACT;
				}
				if (
					(feedbackValue[act]>targetPositionLo)&&(feedbackValue[act]<targetPositionHi)
				){
					actions[act] = HOLD;
				}
				
				if (feedbackValue[act]<=minPositionLo){
					actions[act] = EXTEND;
				}
				if (feedbackValue[act]>=maxPositionHi){
					actions[act] = CONTRACT;
				}

				if (actions[act]==EXTEND){
					bridgeAssert(act,true);
				}else if (actions[act]==CONTRACT){
					bridgeAssert(act,false);
				}else{
					bridgeDeassert(act);
				}

			}else{
				bridgeDeassert(act);
			}

			timeSlots[act]++;
			switch(actions[act]){
			case EXTEND:
				if (timeSlots[act]==TIME_SLOT_MAX_EXTEND[act]){
					timeSlots[act] = 0;
				}
				break;
			case CONTRACT:
				if (timeSlots[act]==TIME_SLOT_MAX_CONTRACT[act]){
					timeSlots[act] = 0;
				}
				break;
			default:
				if (timeSlots[act]==TIME_SLOT_MAX_HOLD){
					timeSlots[act] = 0;
				}
				break;
			}

			
		}else{
			bridgeDeassert(act);	
		}
		
	}
	
	
	
}
//======================================================================================
void Actuators::enable(
	uint_fast8_t actuatorIndex,
	int32_t newTargetPosition,
	int32_t newMinPosition,
	int32_t newMaxPosition
){

	if (actuatorIndex<actuatorCount){
		targetPosition[actuatorIndex] = newTargetPosition;
		minPosition[actuatorIndex] = newMinPosition;
		maxPosition[actuatorIndex] = newMaxPosition;
		enabled[actuatorIndex] = true;
		timeSlots[actuatorIndex] = 0;
	}




}
//======================================================================================
void Actuators::disable(uint_fast8_t actuatorIndex){

	if (actuatorIndex<actuatorCount){
		enabled[actuatorIndex] = false;
	}
}
//======================================================================================
bool Actuators::targetPositionReached(uint_fast8_t actuatorIndex){

	if (
		(feedbackValue[actuatorIndex]>targetPosition[actuatorIndex]-FEEDBACK_VALUE_HYSTERESIS)&&
		(feedbackValue[actuatorIndex]<targetPosition[actuatorIndex]+FEEDBACK_VALUE_HYSTERESIS)
	){
		return true;
	}else{
		return false;
	}

}
//======================================================================================
void Actuators::emergencyStop(){

	for(uint_fast8_t act=0;act<actuatorCount;act++){
		enabled[act] = false;
		bridgeDeassert(act);
	}

}
//======================================================================================


