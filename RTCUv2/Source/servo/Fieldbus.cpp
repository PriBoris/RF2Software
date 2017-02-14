

#include "Fieldbus.h"

#include <string.h>


#include "system/ports.h"
#include "servo/uss.h"
#include "stm32f4xx_conf.h"
#include "servo/servo.h"


uint8_t Fieldbus::rxBuffer[RX_BUFFER_LENGTH];
uint8_t Fieldbus::txBuffer[TX_BUFFER_LENGTH];

uint32_t Fieldbus::pendingPtr;
uint32_t Fieldbus::transmittedPtr;
uint32_t Fieldbus::receivedPtr;
uint32_t Fieldbus::processedPtr;

bool Fieldbus::transmitting;

uint8_t Fieldbus::indicationCounter;

Fieldbus::RxState Fieldbus::rxState;
uint8_t Fieldbus::rxBCCCalculated;
uint16_t Fieldbus::rxPKEValue;
uint16_t Fieldbus::rxPWEValue;
uint16_t Fieldbus::rxINDValue;

uint32_t Fieldbus::rxGoodPacketCounter;
uint32_t Fieldbus::rxBadPacketCounter;

bool Fieldbus::responseReceived;



//==============================================================================================
void Fieldbus::receiver(){
//	GPIOE->BSRRL = (1<<9);//transmitEnable
	GPIOE->BSRRH = (1<<9);//transmitDisable
	GPIOE->BSRRH = (1<<10);//readEnable
}
void Fieldbus::transmitter(){
//	GPIOE->BSRRH = (1<<10);//readEnable
	GPIOE->BSRRL = (1<<10);//readDisable
	GPIOE->BSRRL = (1<<9);//transmitEnable
}
//==============================================================================================
void Fieldbus::init(){

	receiver();

	memset(rxBuffer,0,sizeof(rxBuffer));
	memset(txBuffer,0,sizeof(txBuffer));

	pendingPtr = 0;
	transmittedPtr = 0;
	receivedPtr = 0;
	processedPtr = 0;

	transmitting = false;

	indicationCounter = 0;
	ports::led2off();

	rxState = STX_EXPECTED;
	rxGoodPacketCounter = 0;
	rxBadPacketCounter = 0;
	responseReceived = false;



	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART7, &USART_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(UART7, ENABLE);
	UART7->SR = 0;
	USART_ITConfig(UART7, USART_IT_RXNE, ENABLE); //Received Data Ready to be Read

}
//==============================================================================================
extern "C" {void UART7_IRQHandler()
{
	Fieldbus::irqHandler();
}}
void Fieldbus::irqHandler(){

	if (USART_GetITStatus(UART7,USART_IT_RXNE)==SET){

		rxBuffer[receivedPtr] = UART7->DR;
		receivedPtr = (receivedPtr+1)&(RX_BUFFER_LENGTH-1);
		
		indicationCounter = indicationCounterMax;
		ports::led2on();
		
	}
	
	
	if (USART_GetITStatus(UART7,USART_IT_TC)==SET){

		if (pendingPtr!=transmittedPtr){

			UART7->DR = txBuffer[transmittedPtr];
			transmittedPtr = (transmittedPtr+1)&(TX_BUFFER_LENGTH-1);

		} else {

			receiver();	
			transmitting = false;
			USART_ITConfig(UART7, USART_IT_TC, DISABLE); //Transmission Complete
		}

	}

}
//==============================================================================================
void Fieldbus::pushByte(uint8_t byte){

	txBuffer[pendingPtr] = byte;
	pendingPtr = (pendingPtr+1)&(TX_BUFFER_LENGTH-1);

}
//==============================================================================================
void Fieldbus::pushBytes(uint8_t* buf,uint32_t bufLen){

	transmitter();
	transmitting = true;

	for(uint32_t i=0;i<bufLen;i++){
		pushByte(buf[i]);
	}

	UART7->DR = txBuffer[transmittedPtr];
	transmittedPtr = (transmittedPtr+1)&(TX_BUFFER_LENGTH-1);
	USART_ITConfig(UART7, USART_IT_TC, ENABLE); //Transmission Complete




}
//==============================================================================================
void Fieldbus::pushUSSRequest(USS::Request *request){
	
	responseReceived = false;
	pushBytes(request->bytes,request->byteCount);
	
	
}
//==============================================================================================
void Fieldbus::processRx(){


	while(receivedPtr!=processedPtr){

		uint8_t byte = rxBuffer[processedPtr];
		processedPtr = (processedPtr+1)&(RX_BUFFER_LENGTH-1);

		switch(rxState){

		case STX_EXPECTED:

			if (byte==USS::STX){
				rxState = BYTE_COUNT_EXPECTED;
				rxBCCCalculated = byte;
			}
			break;

		case BYTE_COUNT_EXPECTED:
			rxBCCCalculated ^= byte;
			if (byte==12){
				rxState = ADDRESS_EXPECTED;
			}else{
				rxState = STX_EXPECTED;
			}
			break;

		case ADDRESS_EXPECTED:
			rxBCCCalculated ^= byte;
			if (byte==0){
				rxState = PKE_HI_EXPECTED;
			}else{
				rxState = STX_EXPECTED;
			}
			break;

		case PKE_HI_EXPECTED:
			rxBCCCalculated ^= byte;
			rxPKEValue = ((uint16_t)byte)<<8;
			rxState = PKE_LO_EXPECTED;

			break;
		case PKE_LO_EXPECTED:
			rxBCCCalculated ^= byte;
			rxPKEValue |= (uint16_t)byte;
			rxState = IND_HI_EXPECTED;
			break;

		case IND_HI_EXPECTED:
			rxBCCCalculated ^= byte;
			rxINDValue = ((uint16_t)byte)<<8;
			rxState = IND_LO_EXPECTED;
			break;
		case IND_LO_EXPECTED:
			rxBCCCalculated ^= byte;
			rxINDValue |= (uint16_t)byte;
			rxState = PWE_HI_EXPECTED;
			break;
		case PWE_HI_EXPECTED:
			rxBCCCalculated ^= byte;
			rxPWEValue = ((uint16_t)byte)<<8;
			rxState = PWE_LO_EXPECTED;
			break;
		case PWE_LO_EXPECTED:
			rxBCCCalculated ^= byte;
			rxPWEValue |= (uint16_t)byte;
			rxState = STW_HI_EXPECTED;
			break;
		case STW_HI_EXPECTED:
			rxBCCCalculated ^= byte;
			rxState = STW_LO_EXPECTED;
			break;
		case STW_LO_EXPECTED:
			rxBCCCalculated ^= byte;
			rxState = SW1_HI_EXPECTED;
			break;
		case SW1_HI_EXPECTED:
			rxBCCCalculated ^= byte;
			rxState = SW1_LO_EXPECTED;
			break;
		case SW1_LO_EXPECTED:
			rxBCCCalculated ^= byte;
			rxState = BCC_EXPECTED;
			break;
		case BCC_EXPECTED:
			if (rxBCCCalculated==byte){
				rxGoodPacketCounter++;
				responseReceived = true;

				{
					if ((rxPKEValue & USS::PARAMETER_MASK)==USS::PARAMETER_HeatsinkTemperature){

						Servo::heatsinkTemperature = (int32_t)rxPWEValue;

					}



				}


			}else{
				rxBadPacketCounter++;
			}
			rxState = STX_EXPECTED;
			break;
		}

	}

	


}
//==============================================================================================
void Fieldbus::indication(){

	if (indicationCounter!=0){
			indicationCounter--;
			if (indicationCounter==0){
					ports::led2off();
			}
	}
	
	
	



}
//==============================================================================================
bool Fieldbus::checkSetFrequencyResponse(bool direction,float frequencyValueHertz){

	uint16_t expectedPKEValue;
	//uint16_t expectedPWEValue;

	if (direction!=false){
		expectedPKEValue = (USS::PARAMETER_FixedFrequency1) + USS::AK_Response_TransferParameterValue16;
	}else{
		expectedPKEValue = (USS::PARAMETER_FixedFrequency2) + USS::AK_Response_TransferParameterValue16;
	}

	if (rxPKEValue==expectedPKEValue){
		return true;
	}else{
		return false;
	}

}
//==============================================================================================
bool Fieldbus::responseIsValid(){


	return responseReceived;

}
//==============================================================================================




