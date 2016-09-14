

#include "Clock.h"
#include "Ports.h"
#include "MFRC522.h"
#include "Indicator.h"


int main(){

	Clock::start();
	
	Ports::start();
	Ports::initOutput(GPIOA,4,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::Set);//RFID_CS
	Ports::initOutput(GPIOA,8,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::Set);//LED_PWM before timer initialization
	Ports::initAnalogInput(GPIOA,0);//LED_I
	Ports::initAlternate(GPIOA,2,Ports::GPIO_AF1_USART1,Ports::GPIO_OType_PP,Ports::GPIO_Low_Speed,Ports::GPIO_PuPd_NOPULL);//RS485_TX
	Ports::initAlternate(GPIOA,3,Ports::GPIO_AF1_USART1,Ports::GPIO_OType_PP,Ports::GPIO_Low_Speed,Ports::GPIO_PuPd_NOPULL);//RS485_TX
	Ports::initAlternate(GPIOA,5,Ports::GPIO_AF0_SPI1,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::GPIO_PuPd_NOPULL);//RFID_SCK
	Ports::initAlternate(GPIOA,6,Ports::GPIO_AF0_SPI1,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::GPIO_PuPd_NOPULL);//RFID_MISO
	Ports::initAlternate(GPIOA,7,Ports::GPIO_AF1_USART1,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::GPIO_PuPd_NOPULL);//RFID_MOSI
	
	//uart::start()
	
	MFRC522::start();

	
	Indicator::start();
	Ports::initAlternate(GPIOA,8,Ports::GPIO_AF2_TIM1,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::GPIO_PuPd_NOPULL);//LED_PWM 
	
	
	while(true){
		__asm("	nop");
		
	}

	
	
}
