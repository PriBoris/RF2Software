

#include "heartbeat.h"

#include "stm32f2xx_conf.h"


uint32_t Heartbeat::counter = 0;
uint8_t Heartbeat::mainTickCounter = 0;
bool Heartbeat::mainTickReady = false;


//==============================================================================================
void Heartbeat::init(void){

	if (SystemCoreClock==120000000){
	}else{
	}
	
	
	
	counter = 0;
	mainTickCounter = 0;
	

	SysTick->LOAD = 120000000/1000;//SYST_RVR	- SysTick Reload Value	
	SysTick->CTRL = 
		(1<<0)|//enable
		(1<<1)|//interrupt
		(1<<2)|//processor clock
		0;//SYST_CSR
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC_PriorityGroup_2: 2 bits for pre-emption priority 2 bits for subpriority 
	while(counter<1000);
	
}
//==============================================================================================
extern "C" {void SysTick_Handler(void){//1000Hz
	Heartbeat::tick();
}}
void Heartbeat::tick(void){

	counter++;
	
	(mainTickCounter)++;
	if (mainTickCounter==100){
		mainTickCounter =0;
		mainTickReady = true;
	}

	
}
//==============================================================================================
uint32_t Heartbeat::getCounterValue(){
	return counter;
}
//==============================================================================================
