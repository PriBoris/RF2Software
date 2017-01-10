
#include "heartbeat.h"

#include <misc.h>

#include "servo/encoder.h"
#include "servo/fieldbus.h"
#include "system/ports.h"
#include "actuators/actuators.h"

uint32_t Heartbeat::counter = 0;
uint8_t Heartbeat::mainTickCounter = 0;
bool Heartbeat::mainTickReady = false;
bool Heartbeat::mainTickStarted = false;

//=================================================================================================
void Heartbeat::init(){

	if (SystemCoreClock==168000000){

	} else {
		
	}
	
	counter = 0;


	SysTick->LOAD = 168000000/1000;//SYST_RVR	- SysTick Reload Value	
	SysTick->CTRL = 
		(1<<0)|//enable
		(1<<1)|//interrupt
		(1<<2)|//processor clock
		0;

	while(counter<200);	

}
//=================================================================================================
extern "C" {void SysTick_Handler(void){
	Heartbeat::tick();
}}
void Heartbeat::tick(){

	counter++;
	if (mainTickStarted==true){
		(mainTickCounter)++;
		if (mainTickCounter==100){
			mainTickCounter =0;
			mainTickReady = true;
		}
	}
	
	Encoder::sendRequest();
	Actuators::feedbackStartAdc();

	Fieldbus::indication();
	
}
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
//=================================================================================================
uint32_t Heartbeat::getCounterValue(){
	return counter;
}
//=================================================================================================


