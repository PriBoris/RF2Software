

#include "Clock.h"
#include "Ports.h"



int main(){


/*
PA0 = LED_I 
PA2 = RS485_TX
PA3 = RS485_TX
PA4 = RFID_CS
PA5 = RFID_SCK
PA6 = RFID_MISO
PA7 = RFID_MOSI
PA8 = LED_PWM
*/

	//clock
	//heartbeat

	Clock::start();
	
	
	Ports::start();


	Ports::initOutput(GPIOA,4,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::Set);//RFID_CS
	Ports::initOutput(GPIOA,8,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::Set);//LED_PWM before timer initialization
	Ports::initAnalogInput(GPIOA,0);//LED_I
	
	
	Ports::initAlternate(GPIOA,2,Ports::GPIO_AF1_USART1,Ports::GPIO_OType_PP,Ports::GPIO_Low_Speed,Ports::GPIO_PuPd_NOPULL);
	Ports::initAlternate(GPIOA,3,Ports::GPIO_AF1_USART1,Ports::GPIO_OType_PP,Ports::GPIO_Low_Speed,Ports::GPIO_PuPd_NOPULL);
	Ports::initAlternate(GPIOA,5,Ports::GPIO_AF0_SPI1,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::GPIO_PuPd_NOPULL);
	Ports::initAlternate(GPIOA,6,Ports::GPIO_AF0_SPI1,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::GPIO_PuPd_NOPULL);
	Ports::initAlternate(GPIOA,7,Ports::GPIO_AF1_USART1,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::GPIO_PuPd_NOPULL);
	
	// timer PWM
	// rfid
	//uart
	
	//Ports::initAlternate(GPIOA,8,Ports::GPIO_AF2_TIM1,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::GPIO_PuPd_NOPULL);

	
	
	
	
	while(true){
		__asm("	nop");
		
	}

	
	
}
