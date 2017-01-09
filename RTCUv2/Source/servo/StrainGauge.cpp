
#include "StrainGauge.h"

#include <stm32f4xx_conf.h>

#include "business/MachineSettings.h"



uint32_t StrainGauge::readyCounter = 0;
uint32_t StrainGauge::valueCounter = 0;
uint_fast8_t StrainGauge::byteCounter1;
uint8_t StrainGauge::adcBytes[5];
int32_t StrainGauge::valueBuffer[StrainGauge::valueBufferLength];
uint32_t StrainGauge::valueBufferPtr = 0;
bool StrainGauge::fault;



//===================================================================================================
void StrainGauge::init(){

	fault = false;
	
	syncDeassert();
	cs1Deassert();
	cs2Deassert();
	
	//start MCLK
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//42Mhz
	TIM2->PSC = 0;
	TIM2->ARR = 17;
	TIM2->CCR1 = 8;
	TIM2->CCER = 
		(0<<1)+//0: OC1 active high 
		(1<<0)+//1: On - OC1 signal is output on the corresponding output pin
		0;
	TIM2->CCMR1 = 
		(6<<4)+//110: PWM mode 1 
		(0<<3)+//0: Preload register on TIMx_CCR1 disabled.
		0;
	TIM2->CR1 = 
		(0<<7)+//0: TIMx_ARR register is not buffered 
		(0<<5)+//00: Edge-aligned mode.
		(0<<4)+//0: Counter used as upcounter 
		(0<<3)+//0: Counter is not stopped at update event 
		(1<<0)+//1: Counter enabled
		0;

	// start SPI (max 5Mbit)
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_SPI3, ENABLE);//42Mhz
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_I2S_DeInit(SPI3);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI3, &SPI_InitStructure);
	SPI_SSOutputCmd(SPI3, ENABLE);
	SPI_Cmd(SPI3, ENABLE);
	
	
	
	while(true){//read ID register
		
		cs1Assert();
		
		pollByte(0xFF);		
		pollByte(0xFF);		
		pollByte(0xFF);		
		pollByte(0xFF);		
		pollByte(0xFF);		
		
		uint8_t idRegisterComm = 
			(0<<7)+ //~WEN
			(1<<6)+ //read
			(4<<3)+ //id register
			(0<<2)+ //single read
			0;
		pollByte(idRegisterComm);		
		uint8_t idRegister = pollByte(0xFF);
		cs1Deassert();
		if (idRegister==0xA0){
			break;
		}
	}

	
	{// write CONFIGURATION register
	
		cs1Assert();

		uint8_t confRegisterComm = 
			(0<<7)+ //~WEN
			(0<<6)+ //write
			(2<<3)+ //conf register
			(0<<2)+ //single write
			0;
		pollByte(confRegisterComm);		
	
		uint32_t confRegister = 
			(0<<23)+ //chop disabled
			(0<<20)+ //External reference applied between REFIN1(+) and REFIN1(-). 
			(1<<6)+ // reference detect enabled
			(0<<8)+ //Channel 0
			(1<<4)+ //Enable buffer
			(0<<3)+ //bipolar operation
			(7<<0)+ // Gain=128
			0;
		pollByte((confRegister>>16)&0xFF);		
		pollByte((confRegister>>8)&0xFF);		
		pollByte((confRegister>>0)&0xFF);		
		cs1Deassert();
	}
	
	{// write MODE register
		cs1Assert();

		uint8_t modeRegisterComm = 
			(0<<7)+ //~WEN
			(0<<6)+ //write
			(1<<3)+ //conf register
			(0<<2)+ //single write
			0;
		pollByte(modeRegisterComm);		

		uint32_t modeRegister = 
			(0<<21)+ //Continuous conversion mode
			(1<<20)+// enables the transmission of status register contents after each data register read
			//(2<<18)+//Internal 4.92 MHz clock. Pin MCLK2 is tristated. 
			(1<<18)+//External clock. The external clock is applied to the MCLK2 pin. 
			(0<<15)+ // sinc3 ???
			(1<<13)+//Enable parity bit
			(8<<0)+//rate select [Output Data Rate = (4.92MHz/1024)/FS ]
			0;
		pollByte((modeRegister>>16)&0xFF);		
		pollByte((modeRegister>>8)&0xFF);		
		pollByte((modeRegister>>0)&0xFF);		

		cs1Deassert();

	}
	
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, ENABLE);
	}
	
	{
		setRdyMode();
		

		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
		
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

	}
	
	
	
	cs1Assert();
}
//===================================================================================================
void StrainGauge::syncAssert(){
	GPIOB->BSRRH = (1<<6);
}
void StrainGauge::syncDeassert(){
	GPIOB->BSRRL = (1<<6);
}
//===================================================================================================
void StrainGauge::cs1Assert(){
	GPIOC->BSRRH = (1<<8);
}
void StrainGauge::cs1Deassert(){
	GPIOC->BSRRL = (1<<8);
}
void StrainGauge::cs2Assert(){
	GPIOC->BSRRH = (1<<9);
}
void StrainGauge::cs2Deassert(){
	GPIOC->BSRRL = (1<<9);
}
//===================================================================================================
uint8_t StrainGauge::pollByte(uint8_t txByte){
	SPI3->DR = txByte;
	while(1){
		if (SPI3->SR & SPI_I2S_FLAG_RXNE){
			break;
		}
	}
	return (SPI3->DR);

}
//===================================================================================================
void StrainGauge::rdyInterrupt(){

	//disable gpio interrupt
	EXTI_ClearITPendingBit(EXTI_Line4);
	readyCounter++;
	EXTI->IMR = 0;

	setMisoMode();

	SPI3->DR = 0x58;
	byteCounter1 = 0;
	
}
//===================================================================================================
void StrainGauge::misoInterrupt(){
	
	adcBytes[byteCounter1++] = SPI3->DR;
	if (byteCounter1!=5)
	{
		SPI3->DR = 0xFF;
	}
	else
	{
		setRdyMode();
		
		//reenable gpio interrupt
		EXTI_ClearITPendingBit(EXTI_Line4);
		EXTI->IMR = (1<<4);
		
		// TODO: check parity somehow
		
		
		int32_t newValue;
		uint8_t *pb = (uint8_t*)&(newValue);
		*pb++ = 0;
		*pb++ = adcBytes[3];
		*pb++ = adcBytes[2];
		*pb++ = adcBytes[1];
		*(uint32_t*)&(newValue) -= 0x80000000; // offset
		
		valueBuffer[valueBufferPtr] = newValue;
		if (adcBytes[4]&(1<<5)){
			fault = true;
		}
		//TODO: check overflow
		
		valueCounter++;
		valueBufferPtr = (valueBufferPtr+1)&(valueBufferLength-1);
		
	}
	
	//TODO: add 5 extra dummy reads for reliability	
}

//===================================================================================================
extern "C" {void EXTI4_IRQHandler(void){
	StrainGauge::rdyInterrupt();
}}
extern "C" {void SPI3_IRQHandler(void){
	StrainGauge::misoInterrupt();
}}
//===================================================================================================
void StrainGauge::setMisoMode(){
	// set MISO/PB4 as MISO
	GPIOB->MODER |= (uint32_t)(2<<(2*4));
}
void StrainGauge::setRdyMode(){
// set MISO/PB4 as GPIO
	GPIOB->MODER &= (uint32_t)~((uint32_t)(3<<(2*4)));	
}
//===================================================================================================
bool StrainGauge::getFault(){
	return fault;
}
//===================================================================================================
int32_t StrainGauge::getFilteredValue(void)
{
	uint32_t ptr = valueBufferPtr;
	
/*	int32_t accu = 0;
	for(uint32_t i=0;i<filterLength;i++)
	{
		ptr = (ptr-1)&(valueBufferLength-1);
		accu += valueBuffer[ptr]/filterLength;//TODO: smells bad
	}
*/

	int64_t accu = 0;
	for(uint32_t i=0;i<filterLength;i++)
	{
		ptr = (ptr-1)&(valueBufferLength-1);
		accu += (int64_t)valueBuffer[ptr];
	}
	accu /= (int64_t)filterLength;

	//static const int32_t VALUE_OFFSET = 13000;
	//return ((int32_t)accu/(int32_t)4096)+VALUE_OFFSET;
	
	
	if (MachineSettings::protocolStructExtendedValid==true){
		float forceSensorRawValue = (float)accu;
		float forceSensorValue = forceSensorRawValue* MachineSettings::protocolStructExtended.forceSensorGain;
		return (int32_t)forceSensorValue + MachineSettings::protocolStructExtended.forceSensorOffset;
	}else{
		return 0;
	}
	
	
}
//===================================================================================================
