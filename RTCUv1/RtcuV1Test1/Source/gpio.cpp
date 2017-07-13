
#include "stm32f2xx_conf.h"


#define GPIO_PuPd_DontCare (GPIOPuPd_TypeDef)0
#define GPIO_AF_DontCare (uint8_t)0
#define GPIO_Speed_DontCare (GPIOSpeed_TypeDef)0

typedef struct
{
		GPIOMode_TypeDef GPIO_Mode; //GPIO_Mode_IN,GPIO_Mode_OUT,GPIO_Mode_AF,GPIO_Mode_AN
		GPIOOType_TypeDef GPIO_OType; //GPIO_OType_PP,GPIO_OType_OD  
		uint16_t GPIO_PinSource;
		GPIOPuPd_TypeDef GPIO_PuPd;//GPIO_PuPd_NOPULL,GPIO_PuPd_UP,GPIO_PuPd_DOWN  
		GPIOSpeed_TypeDef GPIO_Speed;//GPIO_Speed_2MHz,GPIO_Speed_25MHz,GPIO_Speed_50MHz,GPIO_Speed_100MHz  
		GPIO_TypeDef *  GPIOx;  
		uint8_t  GPIO_AF;



	
} TGpioConfig;
const TGpioConfig axGpioConfig[] = {
	
/*
PA9 USB_TX
PA10 USB_RX
PA11 USB_CTS
PA12 USB_RTS
	
PA2	HMI_TX [USART2_TX]
PA3	HMI_RX [USART2_RX]
PE6 HMI_TXE 	

PB6 ADC_CS
PA5 ADC_SCLK [SPI1_SCK]
PA6 ADC_MISO [SPI1_MISO]
PA7 ADC_MOSI [SPI1_MOSI]
	
	
PA1 EthCS
PB10 EthSCLK
PC2 EthMISO
PC3 EthMOSI
PC5 EthRST
PC4 EthInt
	
	
PB5 RTCMEM_MOSI [ SPI3_MOSI]
PB3 RTCMEM_SCLK [SPI3_SCK]
PB4 RCMEM_MISO [SPI3_MISO]
PD14 RTC_CS
pd15 MEM_CS
	
*/	
	
	
	//---------------------------------------------------------------
	// PA9 USB_TX [USART1_TX]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		9,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOA,
		GPIO_AF_USART1,	
	
	//---------------------------------------------------------------
	// PA10 USB_RX [USART1_RX]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		10,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOA,
		GPIO_AF_USART1,	

	//---------------------------------------------------------------
	// PA11 USB_CTS [USART1_CTS]
		GPIO_Mode_IN,
		GPIO_OType_PP,
		11,
		GPIO_PuPd_UP,
		GPIO_Speed_2MHz ,
		GPIOA,
		GPIO_AF_DontCare,
	
	/*
		GPIO_Mode_AF,
		GPIO_OType_PP,
		11,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOA,
		GPIO_AF_USART1,	
	*/
	
	//---------------------------------------------------------------
	// 	PA12 USB_RTS [USART1_RTS]
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		12,
		GPIO_PuPd_UP,
		GPIO_Speed_2MHz ,
		GPIOA,
		GPIO_AF_DontCare,
	
/*	
		GPIO_Mode_AF,
		GPIO_OType_PP,
		12,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOA,
		GPIO_AF_USART1,	
*/	
	
	//---------------------------------------------------------------
	// 	PA2	HMI_TX [USART2_TX]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		2,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOA,
		GPIO_AF_USART2,	
	
	//---------------------------------------------------------------
	// 	PA3	HMI_RX [USART2_RX]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		3,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOA,
		GPIO_AF_USART2,	
	
	//---------------------------------------------------------------
	// 	PE6 HMI_TXE 	
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		6,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz ,
		GPIOE,
		GPIO_AF_DontCare,
	
	//---------------------------------------------------------------
	// 	PC6	NFC_TX [USART6_TX]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		6,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOC,
		GPIO_AF_USART6,	
	
	//---------------------------------------------------------------
	// 	PC7	NFC_RX [USART6_RX]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		7,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOC,
		GPIO_AF_USART6,	
	
	//---------------------------------------------------------------
	// 	PB7 NFC_TXE 	
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		7,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz ,
		GPIOB,
		GPIO_AF_DontCare,
		
	//---------------------------------------------------------------
	// PB6 ADC_CS
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		6,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz ,
		GPIOB,
		GPIO_AF_DontCare,
	
	//---------------------------------------------------------------
	// PA5 ADC_SCLK [SPI1_SCK]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		5,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOA,
		GPIO_AF_SPI1,	
	
	//---------------------------------------------------------------
	// PA6 ADC_MISO [SPI1_MISO]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		6,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOA,
		GPIO_AF_SPI1,	
	
	//---------------------------------------------------------------
	// PA7 ADC_MOSI [SPI1_MOSI]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		7,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOA,
		GPIO_AF_SPI1,	
	
	
	//---------------------------------------------------------------
	// PA1 EthCS
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		1,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_50MHz ,
		GPIOA,
		GPIO_AF_DontCare,
	
	//---------------------------------------------------------------
	// PB10 EthSCLK [SPI2_SCK]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		10,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_50MHz  ,
		GPIOB,
		GPIO_AF_SPI2,	
	
	//---------------------------------------------------------------
	// PC2 EthMISO [SPI2_MISO]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		2,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_50MHz  ,
		GPIOC,
		GPIO_AF_SPI2,	
	
	//---------------------------------------------------------------
	// PC3 EthMOSI [SPI2_MOSI]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		3,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_50MHz  ,
		GPIOC,
		GPIO_AF_SPI2,	
	
	//---------------------------------------------------------------
	// PC5 EthRST
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		5,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_50MHz ,
		GPIOC,
		GPIO_AF_DontCare,
	
	//---------------------------------------------------------------
	// PC4 EthInt	
		GPIO_Mode_IN,
		GPIO_OType_PP,
		4,
		GPIO_PuPd_UP,
		GPIO_Speed_50MHz ,
		GPIOC,
		GPIO_AF_DontCare,
	
	
	//---------------------------------------------------------------
	// PB5 RTCMEM_MOSI [ SPI3_MOSI]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		5,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOB,
		GPIO_AF_SPI3,	
	
	//---------------------------------------------------------------
	// PB3 RTCMEM_SCLK [SPI3_SCK]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		3,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz  ,
		GPIOB,
		GPIO_AF_SPI3,	

	//---------------------------------------------------------------
	// PB4 RCMEM_MISO [SPI3_MISO]
		GPIO_Mode_AF,
		GPIO_OType_PP,
		4,
		GPIO_PuPd_DOWN,
		GPIO_Speed_2MHz  ,
		GPIOB,
		GPIO_AF_SPI3,	
	
	//---------------------------------------------------------------
	// PD14 RTC_CS
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		14,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz ,
		GPIOD,
		GPIO_AF_DontCare,
	
	//---------------------------------------------------------------
	// PD15 MEM_CS
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		15,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz ,
		GPIOD,
		GPIO_AF_DontCare,
		
	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// PC14 SRV_STOP
		GPIO_Mode_IN,
		GPIO_OType_PP,
		14,
		GPIO_PuPd_UP,
		GPIO_Speed_2MHz,
		GPIOC,
		GPIO_AF_DontCare,	

	//---------------------------------------------------------------
	// PB12
	// PB13
	// PB14
	// PB15
		GPIO_Mode_OUT,
		GPIO_OType_PP,
		12,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz ,
		GPIOB,
		GPIO_AF_DontCare,

		GPIO_Mode_OUT,
		GPIO_OType_PP,
		13,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz ,
		GPIOB,
		GPIO_AF_DontCare,

		GPIO_Mode_OUT,
		GPIO_OType_PP,
		14,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz ,
		GPIOB,
		GPIO_AF_DontCare,

		GPIO_Mode_OUT,
		GPIO_OType_PP,
		15,
		GPIO_PuPd_NOPULL,
		GPIO_Speed_2MHz ,
		GPIOB,
		GPIO_AF_DontCare,


	
	
	
	
};
//==============================================================================================
void gpioInit(void)
{
	int gpioCount = sizeof(axGpioConfig)/sizeof(axGpioConfig[0]);
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	

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
	


/*	
	if (SystemCoreClock==120000000)
	{
	}
	else
	{
	}
*/
	
}



