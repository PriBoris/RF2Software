
#include "Clock.h"

#include "stm32f0xx.h"


uint32_t Clock::heartbeat = 0;
bool Clock::mainTickStarted = false;
bool Clock::mainTickTriggered = false;
uint32_t Clock::mainTickCounter = 0;
uint32_t Clock::mainTickTop = 0;



//=================================================================================================
void Clock::start(){
	
	RCC->CR &= ~RCC_CR_HSEON;//  HSE clock enable
	RCC->CR |= RCC_CR_HSEBYP;// HSE crystal oscillator bypass
	RCC->CR |= RCC_CR_HSEON;//  HSE clock enable
	while(true){
		if((RCC->CR&RCC_CR_HSERDY)==RCC_CR_HSERDY){
			break;//1: HSE oscillator ready 
		}
	}
	
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSE;//HSE selected as system clock
	while(true){
		if((RCC->CFGR&RCC_CFGR_SWS)==RCC_CFGR_SWS_HSE){
			break;//01: HSE oscillator used as system clock
		}
	}
	
	//SystemCoreClockUpdate(); returns wrong result
	
	RCC->CFGR &= ~RCC_CFGR_HPRE;// AHB clock not divided
	RCC->CFGR &= ~RCC_CFGR_PPRE;// APB clock not divided
	
	
	
	
	
#define SYST_CSR (*(volatile uint32_t*)0xE000E010)	//	SysTick Control and Status Register
#define SYST_RVR (*(volatile uint32_t*)0xE000E014)	//SysTick Reload Value Register
#define SYST_CVR (*(volatile uint32_t*)0xE000E018)	//SysTick Current Value Register

	heartbeat = 0;
	SYST_RVR = Clock::SYSCLK_FREQUENCY/1000;
	SYST_CSR = 
		(1<<0)|//Counter enabled.
		(1<<1)|//Counting down to zero asserts the SysTick exception request.
		(1<<2)|//Processor clock.
		0;
	
}
//=================================================================================================
extern "C" {void SysTick_Handler(void){
	Clock::heartbeatTick();
}}
void Clock::heartbeatTick(){
	heartbeat++;
	if (mainTickStarted!=false){
		mainTickCounter++;
		if (mainTickCounter==mainTickTop){
			mainTickTriggered = true;
			mainTickCounter = 0;
		}

	}
}
//=================================================================================================
void Clock::pause(uint32_t ms){
	
	uint32_t now = heartbeat;
	uint32_t then = now + ms;
	while(true){
		uint32_t diff = heartbeat - then;
		if (*(int32_t*)&diff >=0){
			break;
		}
		__asm("	nop");
	}
	
}
//=================================================================================================
uint32_t Clock::getHeartbeat(){
	return heartbeat;
}
//=================================================================================================
void Clock::startMainTick(uint32_t top){
	mainTickCounter = 0;
	mainTickTop = top;
	mainTickTriggered = false;
	mainTickStarted = true;
}
//=================================================================================================
bool Clock::getMainTick(){
	if (mainTickTriggered==false){
		return false;
	}else{
		mainTickTriggered = false;
		return true;
	}
}
//=================================================================================================


