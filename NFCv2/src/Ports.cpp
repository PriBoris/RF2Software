
#include "Ports.h"

//=================================================================================================
void Ports::start(){
	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	
	
	
}
//=================================================================================================
void Ports::initOutput(
	GPIO_TypeDef *  GPIOx,  
	uint32_t pin,
	GpioOutputType gpioOutputType,
	GpioSpeed gpioSpeed,
	GpioValue initValue
	){
		
	GPIOx->MODER &= ~((uint32_t)(3<<(2*pin)));
	GPIOx->MODER |= ((uint32_t)(1<<(2*pin)));
		
		
	GPIOx->OTYPER &= ~((uint32_t)(1<<pin));
	switch(gpioOutputType){
	case GPIO_OType_PP:
		break;
	case GPIO_OType_OD:
		GPIOx->OTYPER |= ((uint32_t)(1<<pin));
		break;
	}

	GPIOx->OSPEEDR &= ~((uint32_t)(3<<(2*pin)));		
	switch(gpioSpeed){
	case GPIO_Low_Speed:
		break;
	case GPIO_Medium_Speed:
		GPIOx->OSPEEDR |= ((uint32_t)(1<<(2*pin)));
		break;
	case GPIO_Fast_Speed:
		GPIOx->OSPEEDR |= ((uint32_t)(2<<(2*pin)));
		break;
	case GPIO_High_Speed:
		GPIOx->OSPEEDR |= ((uint32_t)(3<<(2*pin)));
		break;
	}

	if (initValue==Reset){
		gpioPinReset(GPIOx,pin);		
	}else{
		gpioPinSet(GPIOx,pin);		
	}
	
	
}
//=================================================================================================
void Ports::initAnalogInput(
	GPIO_TypeDef *  GPIOx,  
	uint32_t pin
	){

	GPIOx->MODER &= ~((uint32_t)(3<<(2*pin)));
	GPIOx->MODER |= ((uint32_t)(3<<(2*pin)));
}		
//=================================================================================================
void Ports::initInput(
	GPIO_TypeDef *  GPIOx,  
	uint32_t pin,
	GpioPull pull
	){

	GPIOx->MODER &= ~((uint32_t)(3<<(2*pin)));
		

	GPIOx->PUPDR &= ~((uint32_t)(3<<(2*pin)));
	switch(pull){
	case GPIO_PuPd_NOPULL:
		break;
	case GPIO_PuPd_UP:
		GPIOx->PUPDR |= ((uint32_t)(1<<(2*pin)));
		break;
	case GPIO_PuPd_DOWN: 
		GPIOx->PUPDR |= ((uint32_t)(2<<(2*pin)));
		break;
	}
		
		

}		
//=================================================================================================
void Ports::initAlternate(
	GPIO_TypeDef *  GPIOx,  
	uint32_t pin,
	GpioAF af,
	GpioOutputType gpioOutputType,
	GpioSpeed gpioSpeed,
	GpioPull pull
	){

	GPIOx->MODER &= ~((uint32_t)(3<<(2*pin)));
	GPIOx->MODER |= ((uint32_t)(2<<(2*pin)));

	GPIOx->OTYPER &= ~((uint32_t)(1<<pin));
	switch(gpioOutputType){
	case GPIO_OType_PP:
		break;
	case GPIO_OType_OD:
		GPIOx->OTYPER |= ((uint32_t)(1<<pin));
		break;
	}

	GPIOx->OSPEEDR &= ~((uint32_t)(3<<(2*pin)));		
	switch(gpioSpeed){
	case GPIO_Low_Speed:
		break;
	case GPIO_Medium_Speed:
		GPIOx->OSPEEDR |= ((uint32_t)(1<<(2*pin)));
		break;
	case GPIO_Fast_Speed:
		GPIOx->OSPEEDR |= ((uint32_t)(2<<(2*pin)));
		break;
	case GPIO_High_Speed:
		GPIOx->OSPEEDR |= ((uint32_t)(3<<(2*pin)));
		break;
	}	

	if(pin<8){
		GPIOx->AFR[0] &= ~((uint32_t)(15<<(4*pin)));
		GPIOx->AFR[0] |= ((uint32_t)((af)<<(4*pin)));
	}else{
		GPIOx->AFR[1] &= ~((uint32_t)(15<<(4*(pin-8))));
		GPIOx->AFR[1] |= ((uint32_t)((af)<<(4*(pin-8))));
	}

	
		
}
//=================================================================================================
void Ports::gpioPinSet(GPIO_TypeDef *GPIOx,uint32_t pin){
	GPIOx->BSRR |= (1<<pin);
}
//=================================================================================================
void Ports::gpioPinReset(GPIO_TypeDef *GPIOx,uint32_t pin){
	GPIOx->BSRR |= (1<<(16+pin));
}
//=================================================================================================


