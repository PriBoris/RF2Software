
#include "actuators.h"

#include "stm32f4xx_conf.h"


bool Actuators::feedbackInitialized = false;
uint16_t Actuators::feedbackFilter[actuatorCount][filterLength];
uint32_t Actuators::feedbackDataCount = 0;
uint8_t Actuators::feedbackChannel = 0;
bool Actuators::feedbackValueReadyFlag = false;
int32_t Actuators::feedbackValue[actuatorCount];

//======================================================================================
void Actuators::feedbackInit(){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC1->CR1 = 
		(0<<24)+//12-bit (15 ADCCLK cycles)
		(0<<8)+//0: Scan mode disabled
		(1<<5)+//0: EOC interrupt disabled
		0;	
	
	ADC1->CR2 = 
		(0<<1)+//0: Single conversion mode
		(1<<0)+//1: Enable ADC
		0;
	
	ADC1->SMPR2 = 
		(1<<0)+//001: 15 cycles
		(1<<3)+
		(1<<6)+
		(1<<9)+
		0;
	
	ADC->CCR = 
		(3<<16)+//11: PCLK2 divided by 8
		0;//TODO: add Temperature and Vbat measurements
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	feedbackChannel = 0;
	feedbackInitialized = true;
	
}
//======================================================================================
/*
	Actuators::feedbackStartAdc() is called 1000 times per second, after every heartbeat
*/
void Actuators::feedbackStartAdc(){

	if (feedbackInitialized!=false){
		
		if (feedbackValueReadyFlag!=false){
			
			//TODO: error, actuators were not processed, deassert all
		}
		
		
		
		
		ADC1->SQR3 = (feedbackChannel<<0);
		ADC1->CR2 = 
			(1<<30)+//1: Starts conversion of regular channels
			(0<<1)+//0: Single conversion mode
			(1<<0)+//1: Enable ADC
			0;	
	}
}
//======================================================================================
extern "C" {void ADC_IRQHandler(){
	
	Actuators::feedbackReadAdc();
}}
//======================================================================================
void Actuators::feedbackReadAdc(){
	
	uint16_t adcValue = ADC1->DR;
	int filterIndex = feedbackDataCount % filterLength;
	
	if (feedbackDataCount==0){
		for(uint_fast8_t i=0;i<filterLength;i++){
			feedbackFilter[feedbackChannel][i] = adcValue;
		}
	} else {
		feedbackFilter[feedbackChannel][filterIndex] = adcValue;
	}
	
	feedbackChannel++;
	if (feedbackChannel==actuatorCount){
		feedbackChannel=0;
		feedbackDataCount++;
		feedbackValueReadyFlag = true;
	}



}
//======================================================================================
bool Actuators::feedbackValueReady(){
	
	if (feedbackValueReadyFlag!=false){
		
		// prepare filtered feedback value
		for(uint_fast8_t act=0;act<actuatorCount;act++){
			feedbackValue[act] = 0;
			for(uint_fast8_t i=0;i<filterLength;i++){
				feedbackValue[act] += (int32_t)feedbackFilter[act][i];
			}
			feedbackValue[act] /= (int32_t)filterLength;
		}
		feedbackValueReadyFlag = false;
		
		//TODO: check if disconnected
		
		return true;
	} else {
		return false;
	}
}
//======================================================================================
int32_t Actuators::getPosition(uint_fast8_t actuatorIndex){

	if (actuatorIndex<actuatorCount){

		return feedbackValue[actuatorIndex];


	}else{
		return -1;
	}



}
//======================================================================================


