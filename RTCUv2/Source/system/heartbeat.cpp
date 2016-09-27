
#include "heartbeat.h"

#include <misc.h>

#include "servo/encoder.h"
#include "servo/fieldbus.h"
#include "system/ports.h"
#include "actuators/actuators.h"

uint32_t Heartbeat::counter = 0;
uint8_t Heartbeat::mainTickCounter = 0;
bool Heartbeat::mainTickReady = false;
bool Heartbeat::mainMidtickReady = false;
bool Heartbeat::mainTickStarted = false;



#define SYST_CSR (*(volatile uint32_t*)0xE000E010)	//	SysTick Control and Status Register
#define SYST_CVR (*(volatile uint32_t*)0xE000E018)	//SysTick Current Value Register
#define SYST_RVR (*(volatile uint32_t*)0xE000E014)	//SysTick Reload Value Register
//=================================================================================================
void Heartbeat::init(){

	if (SystemCoreClock==168000000){

	} else {
		
	}
	
	counter = 0;

	SYST_RVR = 168000000/1000;//SYST_RVR	- SysTick Reload Value	
	SYST_CSR = 
		(1<<0)|//enable
		(1<<1)|//interrupt
		(1<<2)|//processor clock
		0;

	while(counter<200);	
	//while(counter<10000);	


}
//=================================================================================================
void Heartbeat::tick(){

	counter++;
	if (mainTickStarted==true){
		(mainTickCounter)++;
		if (mainTickCounter==100){
			mainTickCounter =0;
			mainTickReady = true;
		}else if (mainTickCounter==50){
			mainMidtickReady = true;
		}
	}
	
	Encoder::sendRequest();
	Actuators::feedbackStartAdc();

	Fieldbus::indication();
	
}
//=================================================================================================
extern "C" {void SysTick_Handler(void)//1000Hz
{
	Heartbeat::tick();

	
	
}}
//=================================================================================================
void Heartbeat::startMainTick(){

	mainTickStarted = true;


	
}
//=================================================================================================
bool Heartbeat::mainTick(){

	if (mainTickReady!=false){
		mainTickReady = false;
		return true;
	} else {
		return false;
	}
}
bool Heartbeat::mainMidtick(){

	if (mainMidtickReady!=false){
		mainMidtickReady = false;
		return true;
	} else {
		return false;
	}
}
//=================================================================================================
uint32_t Heartbeat::getCounterValue(){
	return counter;
}
//=================================================================================================


