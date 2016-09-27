

#include <string.h>

#include "Clock.h"
#include "Ports.h"
#include "MFRC522.h"
#include "Indicator.h"
#include "RTCU.h"

//#define LED_BACKLIGHT_ENABLED

uint32_t profileDetection = 0;
uint32_t profileTimeout = 0;

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
	Ports::initAlternate(GPIOA,7,Ports::GPIO_AF0_SPI1,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::GPIO_PuPd_NOPULL);//RFID_MOSI
	
	MFRC522::start();

	Indicator::start();
#ifdef LED_BACKLIGHT_ENABLED	
	Ports::initAlternate(GPIOA,8,Ports::GPIO_AF2_TIM1,Ports::GPIO_OType_PP,Ports::GPIO_Medium_Speed,Ports::GPIO_PuPd_NOPULL);//LED_PWM 
#endif
	
	RTCU::init();
	
	Clock::startMainTick(50);
	
	while(true){

		__asm("	nop");

		if (Clock::getMainTick()==true){

			uint32_t profileStart = Clock::getHeartbeat();
			if ((true==MFRC522::Tag::isDetected())&&(true==MFRC522::Tag::readSerial())){
				profileDetection = Clock::getHeartbeat() - profileStart;
				Indicator::flash();
				
				uint8_t uidMessage[4+10];	
				memset(uidMessage,0,sizeof(uidMessage));
				
				uidMessage[0] = MFRC522::uid.size;
				memcpy(&uidMessage[4],MFRC522::uid.uidByte,MFRC522::uid.size);
				if (uidMessage[0]!=0){
					RTCU::protocol.sendPacket(Protocol::TAG_RfidProximity,uidMessage,4+MFRC522::uid.size);
				}
				
				
				
				__asm("	nop");
			}else{
				profileTimeout = Clock::getHeartbeat() - profileStart;
				__asm("	nop");
			}
			Indicator::tick();
			
		}
		
		
		RTCU::process();

		
	}

	
	
}
