
#include "ports.h"

#include <stdint.h>
#include <stm32f4xx_conf.h>


#define GPIO_AF_NotUsed (uint8_t)0
typedef struct
{
	GPIOMode_TypeDef GPIO_Mode; //GPIO_Mode_IN,GPIO_Mode_OUT,GPIO_Mode_AF,GPIO_Mode_AN
	GPIOOType_TypeDef GPIO_OType; //GPIO_OType_PP,GPIO_OType_OD  
	uint16_t GPIO_PinSource;
	GPIOPuPd_TypeDef GPIO_PuPd;//GPIO_PuPd_NOPULL,GPIO_PuPd_UP,GPIO_PuPd_DOWN  
	GPIOSpeed_TypeDef GPIO_Speed;//GPIO_Low_Speed GPIO_Medium_Speed GPIO_Fast_Speed GPIO_High_Speed
	GPIO_TypeDef *  GPIOx;  
	uint8_t  GPIO_AF;
	
} TGpioConfig;

/*
PC10 McuLed1
PC11 McuLed2
PC12 McuLed3
PD0 McuLed4
PD1 McuLed5

PA0 McuAct1Feedback
PA1 McuAct2Feedback
PA2 McuAct3Feedback
PA3 McuAct4Feedback

PA5 McuEthSclk
PA6 McuEthMiso
PA7 McuEthMosi
PD14 McuEthCS
PD15 McuEthRst

PA8 McuHmiPwrOn
PA9 McuHmiTx
PA10 McuHmiRx
PA11 McuHmiPwrBtn
PB7 McuHmiBtn0
PB8 McuHmiBtn1
PB9 McuHmiBtn2
PE0 McuHmiBtn3
PE1 McuHmiBtn4

PA15 McuSgMclk
PB3 McuSgSclk
PB4 McuSgDout
PB5 McuSgDin
PB6 McuSgSync
PC8 McuSgCS1
PC9 McuSgCS2

PB2 McuRelay

PB10 McuDiagTx
PB11 McuDiagRx


PA4 McuAct1SwLowL
PB0 McuAct4SwLowL
PB1 McuAct4SwLowR
PB12 McuAct2SwHighL
PB13 McuAct1SwHighL
PB14 McuAct4SwHighR
PB15 McuAct3SwHighR
PC4 McuAct2SwLowL
PC5 McuAct3SwLowL
PD8 McuAct2SwHighR
PD9 McuAct1SwHighR
PE11 McuAct3SwLowR
PE12 McuAct4SwHighL
PE13 McuAct2SwLowR
PE14 McuAct3SwHighL
PE15 McuAct1SwLowR

PC2 McuNvmemMISO
PC3 McuNvmemMOSI
PD3 McuNvmemSCLK
PE6 McuNvmemCS

PC6 McuNfcTx
PC7 McuNfcRx


PE5 McuSsiData
PE2 McuSsiClock

PE3 McuSrvOut1
PE4 McuSrvOut2
PE1 McuSrvOut3

PE7 McuMbRx
PE8 McuMbTx
PE9 McuMbDE
PE10 McuMbRE

*/


const TGpioConfig axGpioConfig[] = {

	
	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PC10 McuLed1
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		10,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOC,
		GPIO_AF_NotUsed,

	//---------------------------------------------------------------
	// PC11 McuLed2
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		11,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOC,
		GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PC12 McuLed3
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		12,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOC,
		GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PD0 McuLed4
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		0,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOD,
		GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PD1 McuLed5
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		1,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOD,
		GPIO_AF_NotUsed,

	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PB2 McuRelay
/*		GPIO_Mode_OUT,
		GPIO_OType_PP,
		2,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOB,
		GPIO_AF_NotUsed,
*/
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		14,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOD,
		GPIO_AF_NotUsed,

	
	//---------------------------------------------------------------
	// PE3 McuSrvOut1
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		3,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOE,
		GPIO_AF_NotUsed,
	
	//---------------------------------------------------------------
	// PE4 McuSrvOut2
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		4,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOE,
		GPIO_AF_NotUsed,

	//---------------------------------------------------------------
	// PE1 McuSrvOut3
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		1,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOE,
		GPIO_AF_NotUsed,

	
	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PA0 McuAct1Feedback
		GPIO_Mode_AN,
		GPIO_OType_PP,
		0,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOA,
		GPIO_AF_NotUsed,
	
	//---------------------------------------------------------------
	// PA1 McuAct2Feedback
		GPIO_Mode_AN,
		GPIO_OType_PP,
		1,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOA,
		GPIO_AF_NotUsed,
	
	//---------------------------------------------------------------
	// PA2 McuAct3Feedback
		GPIO_Mode_AN,
		GPIO_OType_PP,
		2,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOA,
		GPIO_AF_NotUsed,
	
	//---------------------------------------------------------------
	// PA3 McuAct4Feedback
		GPIO_Mode_AN,
		GPIO_OType_PP,
		3,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOA,
		GPIO_AF_NotUsed,

	
	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PB7 McuHmiBtn0
		GPIO_Mode_IN,
		GPIO_OType_PP,
		7,
		GPIO_PuPd_UP,
		GPIO_Low_Speed ,
		GPIOB,
		GPIO_AF_NotUsed,
	
	//---------------------------------------------------------------
	// PB8 McuHmiBtn1
		GPIO_Mode_IN,
		GPIO_OType_PP,
		8,
		GPIO_PuPd_UP,
		GPIO_Low_Speed ,
		GPIOB,
		GPIO_AF_NotUsed,
	
	//---------------------------------------------------------------
	// PB9 McuHmiBtn2
		GPIO_Mode_IN,
		GPIO_OType_PP,
		9,
		GPIO_PuPd_UP,
		GPIO_Low_Speed ,
		GPIOB,
		GPIO_AF_NotUsed,
	
	//---------------------------------------------------------------
	// PE0 McuHmiBtn3
		GPIO_Mode_IN,
		GPIO_OType_PP,
		0,
		GPIO_PuPd_UP,
		GPIO_Low_Speed ,
		GPIOE,
		GPIO_AF_NotUsed,
	
	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PA8 McuHmiPwrOn
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		8,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOA,
		GPIO_AF_NotUsed,
	
	//---------------------------------------------------------------
	// PA9 McuHmiTx [USART1]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		9,
		GPIO_PuPd_NOPULL,
		GPIO_Medium_Speed ,
		GPIOA,
		GPIO_AF_USART1,
	
	//---------------------------------------------------------------
	// PA10 McuHmiRx [USART1]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		10,
		GPIO_PuPd_UP,
		GPIO_Medium_Speed ,
		GPIOA,
		GPIO_AF_USART1,
	
	//---------------------------------------------------------------
	// PA11 McuHmiPwrBtn
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		11,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOA,
		GPIO_AF_NotUsed,

	
	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PB10 McuDiagTx [USART3]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		10,
		GPIO_PuPd_NOPULL,
		GPIO_Medium_Speed ,
		GPIOB,
		GPIO_AF_USART3,
	
	//---------------------------------------------------------------
	// PB11 McuDiagRx [USART3]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		11,
		GPIO_PuPd_UP,
		GPIO_Medium_Speed ,
		GPIOB,
		GPIO_AF_USART3,

	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PC6 McuNfcTx [USART6]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		6,
		GPIO_PuPd_NOPULL,
		GPIO_Medium_Speed ,
		GPIOC,
		GPIO_AF_USART6,
	//---------------------------------------------------------------
	// PC7 McuNfcRx [USART6]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		7,
		GPIO_PuPd_UP,
		GPIO_Medium_Speed ,
		GPIOC,
		GPIO_AF_USART6,


	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PE8 McuMbTx
		GPIO_Mode_AF,
		GPIO_OType_PP,
		8,
		GPIO_PuPd_NOPULL,
		GPIO_Medium_Speed ,
		GPIOE,
		GPIO_AF_UART7,
	//---------------------------------------------------------------
	// PE7 McuMbRx
		GPIO_Mode_AF,
		GPIO_OType_PP,
		7,
		GPIO_PuPd_UP,
		GPIO_Medium_Speed ,
		GPIOE,
		GPIO_AF_UART7,
	//--------------------------------------------------------------
	// PE9 McuMbDE
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		9,
		GPIO_PuPd_NOPULL,
		GPIO_Medium_Speed ,
		GPIOE,
		GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PE10 McuMbRE
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		10,
		GPIO_PuPd_NOPULL,
		GPIO_Medium_Speed ,
		GPIOE,
		GPIO_AF_NotUsed,

	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PE5 McuSsiData
		GPIO_Mode_AF,
		GPIO_OType_PP,
		5,
		GPIO_PuPd_NOPULL,
		GPIO_High_Speed ,
		GPIOE,
		GPIO_AF_SPI4,
	//---------------------------------------------------------------
	// PE2 McuSsiClock
		GPIO_Mode_AF,
		GPIO_OType_PP,
		2,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOE,
		GPIO_AF_SPI4,

	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PC2 McuNvmemMISO
		GPIO_Mode_AF,
		GPIO_OType_PP,
		2,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOC,
		GPIO_AF_SPI2,
	//---------------------------------------------------------------
	// PC3 McuNvmemMOSI
		GPIO_Mode_AF,
		GPIO_OType_PP,
		3,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOC,
		GPIO_AF_SPI2,
	//---------------------------------------------------------------
	// PD3 McuNvmemSCLK
		GPIO_Mode_AF,
		GPIO_OType_PP,
		3,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOD,
		GPIO_AF_SPI2,
	//---------------------------------------------------------------
	// PE6 McuNvmemCS
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		6,
		GPIO_PuPd_NOPULL,
		GPIO_High_Speed ,
		GPIOE,
		GPIO_AF_NotUsed,

	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PA5 McuEthSclk
		GPIO_Mode_AF,
		GPIO_OType_PP,
		5,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOA,
		GPIO_AF_SPI1,
	//---------------------------------------------------------------
	// PA6 McuEthMiso
		GPIO_Mode_AF,
		GPIO_OType_PP,
		6,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOA,
		GPIO_AF_SPI1,
	//---------------------------------------------------------------
	// PA7 McuEthMosi
		GPIO_Mode_AF,
		GPIO_OType_PP,
		7,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOA,
		GPIO_AF_SPI1,
	//---------------------------------------------------------------
	// PD14 McuEthCS
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		14,
		GPIO_PuPd_NOPULL,
		GPIO_High_Speed ,
		GPIOD,
		GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PD15 McuEthRst
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		15,
		GPIO_PuPd_NOPULL,
		GPIO_Low_Speed ,
		GPIOD,
		GPIO_AF_NotUsed,

	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PA15 McuSgMclk
		GPIO_Mode_AF,
		GPIO_OType_PP,
		15,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOA,
		GPIO_AF_TIM2,
	//---------------------------------------------------------------
	// PB3 McuSgSclk
		GPIO_Mode_AF,
		GPIO_OType_PP,
		3,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOB,
		GPIO_AF_SPI3,
	//---------------------------------------------------------------
	// PB4 McuSgDout
		GPIO_Mode_AF,
		GPIO_OType_PP,
		4,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOB,
		GPIO_AF_SPI3,
	//---------------------------------------------------------------
	// PB5 McuSgDin
		GPIO_Mode_AF,
		GPIO_OType_PP,
		5,
		GPIO_PuPd_UP,
		GPIO_High_Speed ,
		GPIOB,
		GPIO_AF_SPI3,
	//---------------------------------------------------------------
	// PB6 McuSgSync
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		6,
		GPIO_PuPd_NOPULL,
		GPIO_High_Speed ,
		GPIOB,
		GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PC8 McuSgCS1
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		8,
		GPIO_PuPd_NOPULL,
		GPIO_High_Speed ,
		GPIOC,
		GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PC9 McuSgCS2
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		9,
		GPIO_PuPd_NOPULL,
		GPIO_High_Speed ,
		GPIOC,
		GPIO_AF_NotUsed,
	
	//---------------------------------------------------------------
	// PD9 McuAct1SwHighR
		GPIO_Mode_OUT,GPIO_OType_PP,9,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOD,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PE15 McuAct1SwLowR
		GPIO_Mode_OUT,GPIO_OType_PP,15,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOE,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PB13 McuAct1SwHighL
		GPIO_Mode_OUT,GPIO_OType_PP,13,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOB,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PA4 McuAct1SwLowL
		GPIO_Mode_OUT,GPIO_OType_PP,4,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOA,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PD8 McuAct2SwHighR
		GPIO_Mode_OUT,GPIO_OType_PP,8,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOD,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PE13 McuAct2SwLowR
		GPIO_Mode_OUT,GPIO_OType_PP,13,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOE,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PB12 McuAct2SwHighL
		GPIO_Mode_OUT,GPIO_OType_PP,12,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOB,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PC4 McuAct2SwLowL
		GPIO_Mode_OUT,GPIO_OType_PP,4,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOC,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PB15 McuAct3SwHighR
		GPIO_Mode_OUT,GPIO_OType_PP,15,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOB,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PE11 McuAct3SwLowR
		GPIO_Mode_OUT,GPIO_OType_PP,11,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOE,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PE14 McuAct3SwHighL
		GPIO_Mode_OUT,GPIO_OType_PP,14,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOE,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PC5 McuAct3SwLowL
		GPIO_Mode_OUT,GPIO_OType_PP,5,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOC,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PB14 McuAct4SwHighR
		GPIO_Mode_OUT,GPIO_OType_PP,14,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOB,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PB1 McuAct4SwLowR
		GPIO_Mode_OUT,GPIO_OType_PP,1,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOB,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PE12 McuAct4SwHighL
		GPIO_Mode_OUT,GPIO_OType_PP,12,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOE,GPIO_AF_NotUsed,
	//---------------------------------------------------------------
	// PB0 McuAct4SwLowL
		GPIO_Mode_OUT,GPIO_OType_PP,0,GPIO_PuPd_NOPULL,GPIO_Low_Speed,GPIOB,GPIO_AF_NotUsed,
	
	
	
	
};	



void ports::init(void)
{
	int gpioCount = sizeof(axGpioConfig)/sizeof(axGpioConfig[0]);
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA1 , ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2 , ENABLE);

	
	
	for(int gpioIndex=0;gpioIndex<gpioCount;gpioIndex++)
	{
		TGpioConfig *pGpioConfig = (TGpioConfig*)&axGpioConfig[gpioIndex];
		
		if ((pGpioConfig->GPIO_Mode)==GPIO_Mode_AF)
		{
			GPIO_PinAFConfig(
				pGpioConfig->GPIOx,
				pGpioConfig->GPIO_PinSource,
				pGpioConfig->GPIO_AF);
		}
		GPIO_InitTypeDef  GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = (pGpioConfig->GPIO_Mode);
		GPIO_InitStructure.GPIO_Speed = (pGpioConfig->GPIO_Speed);
		GPIO_InitStructure.GPIO_OType = (pGpioConfig->GPIO_OType);
		GPIO_InitStructure.GPIO_PuPd = (pGpioConfig->GPIO_PuPd);
		GPIO_InitStructure.GPIO_Pin = 1<<(pGpioConfig->GPIO_PinSource);
		GPIO_Init((pGpioConfig->GPIOx), &GPIO_InitStructure);
	}
	
	led1off();
	led2off();
	led3off();
	led4off();
	led5off();
	
}


void ports::led1on(){
	GPIOC->BSRRL = (1<<10);
};
void ports::led2on(){
	GPIOC->BSRRL = (1<<11);
};
void ports::led3on(){
	GPIOC->BSRRL = (1<<12);
};
void ports::led4on(){
	GPIOD->BSRRL = (1<<0);
};
void ports::led5on(){
	GPIOD->BSRRL = (1<<1);
};
void ports::led1off(){
	GPIOC->BSRRH = (1<<10);
};
void ports::led2off(){
	GPIOC->BSRRH = (1<<11);
};
void ports::led3off(){
	GPIOC->BSRRH = (1<<12);
};
void ports::led4off(){
	GPIOD->BSRRH = (1<<0);
};
void ports::led5off(){
	GPIOD->BSRRH = (1<<1);
};





