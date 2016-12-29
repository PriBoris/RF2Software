
#include "encoder.h"

#include <stm32f4xx_conf.h>

#include "business/Errors.h"
#include "business/MachineSettings.h"


uint32_t Encoder::requestCounter = 0;
uint32_t Encoder::replyCounter = 0;
uint16_t Encoder::rawValue = 0;
//uint16_t Encoder::value = 0;

//=================================================================================================
void Encoder::init(){
	
//spi4

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);

	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//84MHz APB2 /128 = 0.65MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI4, &SPI_InitStructure);

	
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	SPI_I2S_ITConfig(SPI4, SPI_I2S_IT_RXNE, ENABLE);
	SPI_Cmd(SPI4, ENABLE);
	

}
//=================================================================================================
void Encoder::sendRequest(){
	
	requestCounter++;
	SPI_I2S_SendData(SPI4,0x0000);

	
}
//=================================================================================================
void Encoder::getReply(){

	rawValue = SPI_I2S_ReceiveData(SPI4);
	replyCounter++;
	
/*	if (VALUE_SIGN>0){
		value = (rawValue+VALUE_ADDEND-VALUE_SUBTRAHEND) & VALUE_MASK;
	}else{
		value = (VALUE_MASK+1-rawValue+VALUE_ADDEND-VALUE_SUBTRAHEND) & VALUE_MASK;
	}*/
	
}
//=================================================================================================
extern "C"{void SPI4_IRQHandler(){
	
	if (SPI_I2S_GetITStatus(SPI4, SPI_I2S_IT_RXNE) == SET){
		Encoder::getReply();
	}
	
}}
//=================================================================================================
int32_t Encoder::getValue(){
	
	//TODO: unit test this

	if (MachineSettings::protocolStructExtendedValid==true){

		int32_t valueMask = 0;
		switch(MachineSettings::protocolStructExtended.encoderBitCount){
		default:
			valueMask = 0;
			break;
		case 15:
			valueMask = 32767;
			break;
		}

		if (MachineSettings::protocolStructExtended.encoderDirection>=0){

			int32_t value = (int32_t)rawValue;	
			value += MachineSettings::protocolStructExtended.encoderOffset;
			value &= valueMask;
			return value;


		}else{

			int32_t value = -(int32_t)rawValue;	
			value += MachineSettings::protocolStructExtended.encoderOffset;
			value &= valueMask;
			return value;
		}
		
	}else{
		return 0;
	}
	
	
	
	//return value;
}
//=================================================================================================
