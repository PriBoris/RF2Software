
#ifndef USS_H
#define USS_H

#include <stdint.h>


class USS{
	
public:

	struct Request{
		
		uint32_t byteCount;
		uint8_t *bytes;
		
		
	};

	
	static const uint8_t STX = 0x02;


private:

	static const uint8_t ADR = 0x00;

	static const uint32_t STX_OFFSET = 0; 
	static const uint32_t BYTE_COUNT_OFFSET = 1; 
	static const uint32_t ADR_OFFSET = 2; 
	static const uint32_t PKE_HI_OFFSET = 3; 
	static const uint32_t PKE_LO_OFFSET = 4;
	static const uint32_t IND_HI_OFFSET = 5; 
	static const uint32_t IND_LO_OFFSET = 6;
	static const uint32_t PWE_HI_OFFSET = 7; 
	static const uint32_t PWE_LO_OFFSET = 8;
	static const uint32_t STW_HI_OFFSET = 9; 
	static const uint32_t STW_LO_OFFSET = 10;
	static const uint32_t SW1_HI_OFFSET = 11; 
	static const uint32_t SW1_LO_OFFSET = 12;
	static const uint32_t BCC_OFFSET = 13;



		
	




	




	static const uint32_t inverterReadyRequestByteCount=14;
	static uint8_t inverterReadyRequestBytes[inverterReadyRequestByteCount];
	static Request inverterReadyRequest;

	static const uint32_t setFrequencyRequestByteCount=14;
	static uint8_t setFrequencyRequestBytes[setFrequencyRequestByteCount];
	static Request setFrequencyRequest;



public:

	static void init();
	
	static Request* makeInverterReadyRequest();
	static Request* makeSetFrequencyRequest(bool direction,float frequencyValueHertz);


	static const uint16_t PARAMETER_InverterType = 743;//0x2E7
	static const uint16_t PARAMETER_HeatsinkTemperature = 739;//0x2E3

	static const uint16_t PARAMETER_FixedFrequency1 = 429;
	static const uint16_t PARAMETER_FixedFrequency2 = 430;

	static const uint16_t AK_Order_ReadParameterValue = (1<<12);
	static const uint16_t AK_Order_ChangeParameterValueWithoutEeprom16 = (14<<12);

	static const uint16_t AK_Response_TransferParameterValue16 = (1<<12);
	static const uint16_t AK_Response_OrderCannotBeExecuted = (7<<12);

	static const uint16_t AK_MASK = (0x000F<<12);
	static const uint16_t PARAMETER_MASK = 0x07FF;


};



#endif

