
#include "RtcuV1Test1.h"

uint8_t blackBoxRxBuffer[BLACKBOX_RX_BUFFER_LENGTH];
uint32_t blackBoxRxBufferReceived;
uint32_t blackBoxRxBufferProcessed;

uint8_t blackBoxTxBuffer[BLACKBOX_TX_BUFFER_LENGTH];
uint32_t blackBoxTxBufferPending;
uint32_t blackBoxTxBufferTransmitted;

Protocol blackBoxProtocol;


TStuffState stuffState;
TProtocolState protocolState;
uint8_t rxValue[256];

TBlackBoxStrainGauge blackBoxStrainGauge;


//==============================================================================================

void blackBoxInit(void)
{
	
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
	
	
	GPIOA->BSRRH = (1<<12); //assert RTS

	blackBoxProtocol.init(
		blackBoxTxBuffer,
		BLACKBOX_TX_BUFFER_LENGTH,
		blackBoxRxBuffer,
		BLACKBOX_RX_BUFFER_LENGTH
		);	

	
	
	USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	
	



	memset(&blackBoxStrainGauge,0,sizeof(blackBoxStrainGauge));


	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //Received Data Ready to be Read


	
}
//==============================================================================================

extern "C" {void USART1_IRQHandler()
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		blackBoxProtocol.receiveByte(USART1->DR);
	}
}}
//==============================================================================================
void blackBoxProcess(void)
{
	if (
		(blackBoxProtocol.bytesPending()==true) && 
		(USART1->SR & USART_FLAG_TXE)
	)
	{
			USART1->DR = blackBoxProtocol.popTxByte();
	}


	if (blackBoxProtocol.processRx()==true)
	{
		
		switch(blackBoxProtocol.getRxTag())
		{
			//--------------------------------------------------------------------------------------
			case Protocol::TAG_BB_ReadStrainGauge:
			
				blackBoxProtocol.sendPacket(Protocol::TAG_BB_ReadStrainGauge,(uint8_t*)&blackBoxStrainGauge,(uint16_t)sizeof(blackBoxStrainGauge));
			
				break;
			//--------------------------------------------------------------------------------------
			case Protocol::TAG_BB_ReadDateTime:
			
			blackBoxProtocol.sendPacket(Protocol::TAG_BB_ReadDateTime,(uint8_t*)&OdometerRTC::currentDateTime,(uint16_t)sizeof(TCurrentDateTime));
			
				break;
			//--------------------------------------------------------------------------------------
				
			
		}		
		
		
		
	}
	
	
	


	
	
	
}
//==============================================================================================





