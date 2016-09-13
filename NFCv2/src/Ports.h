



#include <stdint.h>

#include "stm32f0xx.h"

class Ports{
private:
	

public:


	enum GpioMode{
		GPIO_Mode_IN,//Input mode 
		GPIO_Mode_OUT,// General purpose output mode
		GPIO_Mode_AF,// Alternate function mode
		GPIO_Mode_AN// Analog mode 
	};	
	enum GpioOutputType{
		GPIO_OType_PP,// Output push-pull 
		GPIO_OType_OD  //Output open-drain
	};
	enum GpioPull{
		GPIO_PuPd_NOPULL,
		GPIO_PuPd_UP,
		GPIO_PuPd_DOWN 
	};
	enum GpioSpeed{
		GPIO_Low_Speed,//fall/rise time 100ns
		GPIO_Medium_Speed,//10ns
		GPIO_Fast_Speed,//6ns
		GPIO_High_Speed//4ns
	};
	enum GpioAF{
		GPIO_AF0_SPI1 = 0,
		GPIO_AF1_USART1 = 1,
		GPIO_AF1_USART2 = 1,
		GPIO_AF2_TIM1 = 2,
	};
	enum GpioValue{
		Reset = 0,
		Set = 1
	};


	static void start();
	
	static void initOutput(
		GPIO_TypeDef *  GPIOx,  
		uint32_t pin,
		GpioOutputType gpioOutputType,
		GpioSpeed gpioSpeed,
		GpioValue initValue
		);
	
	static void initAnalogInput(
		GPIO_TypeDef *  GPIOx,  
		uint32_t pin
		);	

	static void initInput(
		GPIO_TypeDef *  GPIOx,  
		uint32_t pin,
		GpioPull pull
		);	

	static void initAlternate(
		GPIO_TypeDef *  GPIOx,  
		uint32_t pin,
		GpioAF af,
		GpioOutputType gpioOutputType,
		GpioSpeed gpioSpeed,
		GpioPull pull
		);	
	
	static void gpioPinSet(GPIO_TypeDef *GPIOx,uint32_t pin);
	static void gpioPinReset(GPIO_TypeDef *GPIOx,uint32_t pin);	

};
	


