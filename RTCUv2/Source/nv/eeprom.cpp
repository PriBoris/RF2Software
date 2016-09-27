
#include "eeprom.h"

#include <stm32f4xx_conf.h>


void NV::Eeprom::init(){
	
	
	csDeassert();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	
	
	
	
	
	
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//42MHz APB1 /8 = 6MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	//SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
	//SPI_Cmd(SPI2, ENABLE);	
	




	
	





	
}

void NV::Eeprom::csAssert(){
	
	GPIOE->BSRRH = (1<<6);
	
}

void NV::Eeprom::csDeassert(){

	GPIOE->BSRRL = (1<<6);

	
}

extern "C"{void SPI2_IRQHandler(){
	
	if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET){
		//Encoder::getReply();
	}
	
}}

