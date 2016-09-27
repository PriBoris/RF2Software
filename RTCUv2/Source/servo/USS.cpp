
#include "USS.h"


uint8_t USS::inverterReadyRequestBytes[USS::inverterReadyRequestByteCount];
USS::Request USS::inverterReadyRequest;

uint8_t USS::setFrequencyRequestBytes[USS::setFrequencyRequestByteCount];
USS::Request USS::setFrequencyRequest;


//===========================================================================================================
void USS::init(){
	
	{
		inverterReadyRequest.byteCount = inverterReadyRequestByteCount;
		
		uint8_t i=0;
		
		inverterReadyRequestBytes[i++] = STX;
		inverterReadyRequestBytes[i++] = (inverterReadyRequestByteCount-2);
		inverterReadyRequestBytes[i++] = ADR;
		
		uint16_t PKE = PARAMETER_InverterType + AK_Order_ReadParameterValue;
		inverterReadyRequestBytes[i++] = (PKE>>8);
		inverterReadyRequestBytes[i++] = (PKE>>0);

		uint16_t IND = 0;
		inverterReadyRequestBytes[i++] = (IND>>8);
		inverterReadyRequestBytes[i++] = (IND>>0);

		for(uint8_t j=0;j<6;j++){
			inverterReadyRequestBytes[i++] = 0x00;//PWE STW SW1
		}

		uint16_t BCC = 0;
		for(uint8_t j=0;j<(inverterReadyRequestByteCount-1);j++){
			BCC ^= inverterReadyRequestBytes[j];
		}
		inverterReadyRequestBytes[i++] = BCC;
		
		
		inverterReadyRequest.bytes = USS::inverterReadyRequestBytes;
	}


	{
		setFrequencyRequest.byteCount = setFrequencyRequestByteCount;
		
		uint8_t i=0;
		
		setFrequencyRequestBytes[i++] = STX;
		setFrequencyRequestBytes[i++] = (inverterReadyRequestByteCount-2);
		setFrequencyRequestBytes[i++] = ADR;
		
		uint16_t PKE = 0;
		setFrequencyRequestBytes[i++] = (PKE>>8);
		setFrequencyRequestBytes[i++] = (PKE>>8);

		uint16_t IND = 0;
		setFrequencyRequestBytes[i++] = (IND>>8);
		setFrequencyRequestBytes[i++] = (IND>>0);

		for(uint8_t j=0;j<6;j++){
			setFrequencyRequestBytes[i++] = 0x00;//PWE STW SW1
		}

		setFrequencyRequestBytes[i++] = 0xFF;//BCC
		
		setFrequencyRequest.bytes = USS::setFrequencyRequestBytes;

	}
	
}
//===========================================================================================================
USS::Request* USS::makeInverterReadyRequest(){
	
	return &inverterReadyRequest;
}
//===========================================================================================================
USS::Request* USS::makeSetFrequencyRequest(bool direction,float frequencyValueHertz){

	//TODO: check if frequencyIndex<=1


	uint16_t PKE;
	if (direction!=false){
		PKE = (PARAMETER_FixedFrequency1) + AK_Order_ChangeParameterValueWithoutEeprom16;
	}else{
		PKE = (PARAMETER_FixedFrequency2) + AK_Order_ChangeParameterValueWithoutEeprom16;
	}
	setFrequencyRequestBytes[PKE_HI_OFFSET] = (PKE>>8);
	setFrequencyRequestBytes[PKE_LO_OFFSET] = (PKE>>0);


    if (frequencyValueHertz>400.0f){
        frequencyValueHertz = 400.0f;
    }else if (frequencyValueHertz<-400.0f){
        frequencyValueHertz = -400.0f;
    }
	int16_t integerValue;
    if (frequencyValueHertz>=0.0f){
        integerValue = (int16_t)(10.0f*(frequencyValueHertz+0.05f));
    }else{
        integerValue = (int16_t)(10.0f*(frequencyValueHertz-0.05f));
    }
    uint16_t PWE = *(uint16_t*)&integerValue;
	setFrequencyRequestBytes[PWE_HI_OFFSET] = (PWE>>8);
	setFrequencyRequestBytes[PWE_LO_OFFSET] = (PWE>>0);


	uint16_t BCC = 0;
	for(uint8_t j=STX_OFFSET;j<=SW1_LO_OFFSET;j++){
		BCC ^= setFrequencyRequestBytes[j];
	}
	setFrequencyRequestBytes[BCC_OFFSET] = BCC;

	return &setFrequencyRequest;
}
//===========================================================================================================


