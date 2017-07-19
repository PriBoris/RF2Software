
#include "servo.h"

#include "stm32f2xx_conf.h"
#include <string.h>
#include "mode.h"

#include "system/heartbeat.h"


Servo::TState Servo::state;

uint8_t Servo::ipAddressClient[4] = {10,0,0,2};
uint8_t Servo::ipAddressServer[4] = {10,0,0,10};
uint8_t Servo::ipAddressSubnet[4] = {255,0,0,0};
uint8_t Servo::ipAddressGateway[4] = {10,0,0,0};
uint8_t Servo::macAddressClient[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
uint8_t Servo::macAddressServer[6];


uint8_t Servo::spi2TxData[Servo::SPI_MAX_LENGTH];
uint8_t Servo::spi2RxData[Servo::SPI_MAX_LENGTH];
uint32_t Servo::spi2RxTxLength;
uint32_t Servo::spi2RxTxCounter;
bool Servo::spi2RxTxActive;

uint32_t Servo::connectEstablishedCounter;

uint8_t Servo::txBuffer[Servo::TX_BUFFER_LENGTH];
uint8_t Servo::rxBuffer[Servo::RX_BUFFER_LENGTH];
uint32_t Servo::txPointerPending;
uint32_t Servo::txPointerTransmitted;
uint32_t Servo::rxPointerReceived;
uint32_t Servo::rxPointerProcessed;

uint32_t Servo::txHeartbeat;
uint32_t Servo::rxHeartbeat;

//===============================================================================================
void Servo::init(){

	disable();
	
	wizCsDeassert();

	RCC_APB1PeriphClockCmd (RCC_APB1Periph_SPI2, ENABLE); //30MHz
	//RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_DMA1, ENABLE);
	//RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_DMA2, ENABLE);

	SPI_InitTypeDef  SPI_InitStructure;
	SPI_I2S_DeInit(SPI2);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;/* data is always latched in on the rising edge of SCLK and always output on the falling edge of SCLK.  */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_4; /* SPI_BaudRatePrescaler_2 ... SPI_BaudRatePrescaler_256 */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_SSOutputCmd(SPI2, ENABLE);
	SPI_Cmd(SPI2, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);


	state = SRV_Idle;
	spi2RxTxActive = false;
	connectEstablishedCounter = 0;
	
	
	txPointerPending = 0;
	txPointerTransmitted = 0;
	rxPointerReceived = 0;
	rxPointerProcessed = 0;
	
	
	enable();
	disable();
		
}
//==============================================================================================
void Servo::spi2StartRxTx(){
		wizCsAssert();
		spi2RxTxActive = true;
		spi2RxTxCounter = 0;
		SPI2->DR = spi2TxData[0];
}
//==============================================================================================
extern "C" {void SPI2_IRQHandler(void){
	Servo::spiInterruptHandler();
}}
void Servo::spiInterruptHandler(){

	spi2RxData[spi2RxTxCounter++] = SPI2->DR;
	if (spi2RxTxCounter<spi2RxTxLength){
			SPI2->DR = spi2TxData[spi2RxTxCounter];
	}else{
			spi2RxTxActive = false;
			wizCsDeassert();
	}
	
}
//==============================================================================================
void Servo::process(){

	static uint8_t socketIndex;
	static uint32_t waitCounter;
	
	
	switch(state)
	{
			//-----state-----------------------------------------------------------------------
			case SRV_Idle:
				
				wizResetAssert();
				state = SRV_BootWait;
				waitCounter = 0;
			
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_BootWait:

				if (spi2RxTxActive==false)
				{
						if (waitCounter==0)
						{
							wizResetDeassert();
						}
						else
						{
							if (spi2RxData[3] == 0x04) //version OK
							{
								state =SRV_SetUpAddresses;
								break;
							}
							else
							{
								state =SRV_BootWait;
								
							}
						}
						// read version register until 0x04 received
						spi2TxData[0] = (uint8_t)(COMREG_VERSIONR >> 8); // offset high byte
						spi2TxData[1] = (uint8_t)(COMREG_VERSIONR); // offset low byte
						spi2TxData[2] = ACCESS_READ + BSB_CommonRegister + OM_VDM;//command
						spi2TxData[3] = 0; 
						spi2RxTxLength = 4;
						spi2StartRxTx();
						waitCounter++;
				}
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SetUpAddresses:

				if (spi2RxTxActive==false)
				{
						txPointerTransmitted = txPointerPending;
						spi2TxData[0] = (uint8_t)(COMREG_GAR0 >> 8); // offset high byte
						spi2TxData[1] = (uint8_t)(COMREG_GAR0); // offset low byte
						spi2TxData[2] = ACCESS_WRITE + BSB_CommonRegister + OM_VDM;//command
						
						memcpy(&spi2TxData[3],ipAddressGateway,4);
						memcpy(&spi2TxData[3+4],ipAddressSubnet,4);
						memcpy(&spi2TxData[3+4+4],macAddressClient,6);
						memcpy(&spi2TxData[3+4+4+6],ipAddressClient,4);
						spi2RxTxLength = 3+3+4+4+6+4;
						spi2StartRxTx();
						state = SRV_SetUpSockets1to7Memory;socketIndex = 1;
				}
			
			
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SetUpSockets1to7Memory: // setting all other sockets  Receive &Transmit Buffer Size to 0

				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;
					spi2TxData[0] = (uint8_t)(Sn_RXBUF_SIZE >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_RXBUF_SIZE); // offset low byte
					
					switch(socketIndex)
					{
						default:
						case 1:
							spi2TxData[2] = ACCESS_WRITE + BSB_Socket1Register + OM_VDM;//command
							break;
						case 2:
							spi2TxData[2] = ACCESS_WRITE + BSB_Socket2Register + OM_VDM;//command
							break;
						case 3:
							spi2TxData[2] = ACCESS_WRITE + BSB_Socket3Register + OM_VDM;//command
							break;
						case 4:
							spi2TxData[2] = ACCESS_WRITE + BSB_Socket4Register + OM_VDM;//command
							break;
						case 5:
							spi2TxData[2] = ACCESS_WRITE + BSB_Socket5Register + OM_VDM;//command
							break;
						case 6:
							spi2TxData[2] = ACCESS_WRITE + BSB_Socket6Register + OM_VDM;//command
							break;
						case 7:
							spi2TxData[2] = ACCESS_WRITE + BSB_Socket7Register + OM_VDM;//command
							break;
					}
					
					spi2TxData[3] = 0;
					spi2TxData[4] = 0;
					spi2RxTxLength = 3+2;
					spi2StartRxTx();
					if ((++socketIndex)==8)
					{
						state = SRV_SetUpSocket0Memory;
					}
				}

				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SetUpSocket0Memory: // setting first socket  Receive &Transmit  Buffer Size to 16

				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;
					spi2TxData[0] = (uint8_t)(Sn_RXBUF_SIZE >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_RXBUF_SIZE); // offset low byte
					spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
					spi2TxData[3] = 16;
					spi2TxData[4] = 16;
					spi2RxTxLength = 3+2;
					spi2StartRxTx();
					state = SRV_SetUpRetryParameters;
				}

				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SetUpRetryParameters:	

				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;
						uint16_t retryTime = 0x07D1;
						uint8_t retryCount = 0x03;
					
						spi2TxData[0] = (uint8_t)(COMREG_RTR0 >> 8); // offset high byte
						spi2TxData[1] = (uint8_t)(COMREG_RTR0); // offset low byte
						spi2TxData[2] = ACCESS_WRITE + BSB_CommonRegister + OM_VDM;//command
						spi2TxData[3] = (uint8_t)(retryTime >> 8);
						spi2TxData[4] = (uint8_t)(retryTime);
						spi2TxData[5] = retryCount;
						spi2RxTxLength = 3+3;
						spi2StartRxTx();
						state = SRV_SetTcpMode;


				}
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SetTcpMode:

				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;
					spi2TxData[0] = (uint8_t)(Sn_MR >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_MR); // offset low byte
					spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
					spi2TxData[3] = (0<<5)+(1<<0); // TCP mode
					spi2RxTxLength = 3+1;
					spi2StartRxTx();
					state = SRV_SetClientPort;

					
				}
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SetClientPort:
				
				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;
					spi2TxData[0] = (uint8_t)(Sn_PORT0 >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_PORT0); // offset low byte
					spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
					spi2TxData[3] = (uint8_t)(portClient >> 8); // port high byte
					spi2TxData[4] = (uint8_t)(portClient); // port low byte
					spi2RxTxLength = 3+2;
					spi2StartRxTx();
					state = SRV_SetServerAddressPort;
				}
				
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SetServerAddressPort:
				
				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;
					
					spi2TxData[0] = (uint8_t)(Sn_DIPR0 >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_DIPR0); // offset low byte
					spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
					memcpy(&spi2TxData[3],ipAddressServer,4);
					spi2TxData[3+4+0] = (uint8_t)(portServer >> 8); // port high byte
					spi2TxData[3+4+1] = (uint8_t)(portServer); // port low byte
					spi2RxTxLength = 3+4+2;
					spi2StartRxTx();
					state = SRV_SocketInitCommand;
				}
			
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SocketInitCommand:

				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;
					
					spi2TxData[0] = (uint8_t)(Sn_CR >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_CR); // offset low byte
					spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
					spi2TxData[3] = COMMAND_OPEN;
					spi2RxTxLength = 3+1;
					spi2StartRxTx();
					state = SRV_SocketInitWait;
					waitCounter = 0;
					
				}
				
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SocketInitWait:
				
				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;
					if (waitCounter==0)
					{
						
					}
					else
					{
							if (spi2RxData[3] == STATUS_SOCK_INIT)
							{
								state = SRV_SocketConnectCommand;
								break;
							}
					}
					
					spi2TxData[0] = (uint8_t)(Sn_SR >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_SR); // offset low byte
					spi2TxData[2] = ACCESS_READ + BSB_Socket0Register + OM_VDM;//command
					spi2TxData[3] = 0x00;
					spi2RxTxLength = 3+1;
					spi2StartRxTx();
					state = SRV_SocketInitWait;
					waitCounter++;
					
				}

				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SocketConnectCommand:
				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;
					spi2TxData[0] = (uint8_t)(Sn_CR >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_CR); // offset low byte
					spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
					spi2TxData[3] = COMMAND_CONNECT;
					spi2RxTxLength = 3+1;
					spi2StartRxTx();
					state = SRV_SocketConnectWait;
					waitCounter = 0;
	
					
					
				}
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SocketConnectWait:
				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;

					
					
					if (waitCounter==0)
					{
						
					}
					else
					{
							if (spi2RxData[3]==STATUS_SOCK_ESTABLISHED)
							{
								state = SRV_SocketEstablished;
								waitCounter = 0;
								connectEstablishedCounter++;
								break;
							}
							if (spi2RxData[3]==STATUS_SOCK_CLOSED)
							{
								state = SRV_SocketInitCommand;
								break;
							}
							if (spi2RxData[3]==STATUS_SOCK_CLOSE_WAIT)
							{
								state = SRV_SocketDisconnectCommand;
								
							}
							
							
							
					}
					spi2TxData[0] = (uint8_t)(Sn_SR >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_SR); // offset low byte
					spi2TxData[2] = ACCESS_READ + BSB_Socket0Register + OM_VDM;//command
					spi2TxData[3] = 0x00;
					spi2RxTxLength = 3+1;
					spi2StartRxTx();
					state = SRV_SocketConnectWait;
					waitCounter++;
	
					
					
				}
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SocketEstablished:

				if (spi2RxTxActive==false)
				{
					enum SocketEstablishedSubstate
					{
							SES_Idle,
							SES_GettingTxPointer,
							SES_WritingTxData,
							SES_SettingTxPointer,
							SES_SendCommand,
							SES_ReadingStatus,
							SES_GettingRxSize,
							SES_GettingRxPointer,
							SES_ReadingRxData,
							SES_SettingRxPointer,
							
						
					};
					static SocketEstablishedSubstate socketEstablishedSubstate = SES_Idle;
					if (waitCounter==0)
					{
						socketEstablishedSubstate = SES_Idle;// TODO: migth be dangerous if waitCounter overflows
					}				
					static uint16_t txPointer;
					static uint16_t txLength;
					static uint16_t rxLength;
					static uint16_t rxPointer;
					
					
					switch(socketEstablishedSubstate)
					{
							//--socketEstablishedSubstate-----------------------------------------------
							default:
							case SES_Idle:
								if (txPointerPending!=txPointerTransmitted)
								{

									spi2TxData[0] = (uint8_t)(Sn_TX_WR0 >> 8); // offset high byte
									spi2TxData[1] = (uint8_t)(Sn_TX_WR0); // offset low byte
									spi2TxData[2] = ACCESS_READ + BSB_Socket0Register + OM_VDM;//command
									spi2TxData[3] = 0x00;
									spi2TxData[4] = 0x00;
									spi2RxTxLength = 3+2;
									spi2StartRxTx();
									socketEstablishedSubstate = SES_GettingTxPointer;		
									waitCounter++;									
								}
								else
								{
									spi2TxData[0] = (uint8_t)(Sn_SR >> 8); // offset high byte
									spi2TxData[1] = (uint8_t)(Sn_SR); // offset low byte
									spi2TxData[2] = ACCESS_READ + BSB_Socket0Register + OM_VDM;//command
									spi2TxData[3] = 0x00;
									spi2RxTxLength = 3+1;
									spi2StartRxTx();
									socketEstablishedSubstate = SES_ReadingStatus;
									waitCounter++;
								}
								break;
							//--socketEstablishedSubstate-----------------------------------------------
							case SES_GettingTxPointer:
						
								txPointer = (spi2RxData[3]<<8)+spi2RxData[4];

								spi2TxData[0] = (uint8_t)(txPointer >> 8); // offset high byte
								spi2TxData[1] = (uint8_t)(txPointer); // offset low byte
								spi2TxData[2] = ACCESS_WRITE + BSB_Socket0TxBuffer + OM_VDM;//command
								txLength = (txPointerPending-txPointerTransmitted)&(TX_BUFFER_LENGTH-1);
								for(uint16_t i=0;i<txLength;i++)
								{
										spi2TxData[3+i] = txBuffer[txPointerTransmitted];
										txPointerTransmitted = (txPointerTransmitted+1)&(TX_BUFFER_LENGTH-1);
								}
								spi2RxTxLength = 3+txLength;
								spi2StartRxTx();
								socketEstablishedSubstate = SES_WritingTxData;		
								waitCounter++;									
								break;
							//--socketEstablishedSubstate-----------------------------------------------
							case SES_WritingTxData:
								
								txPointer+=txLength;
							
								spi2TxData[0] = (uint8_t)(Sn_TX_WR0 >> 8); // offset high byte
								spi2TxData[1] = (uint8_t)(Sn_TX_WR0); // offset low byte
								spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
								spi2TxData[3] = (uint8_t)(txPointer>>8);
								spi2TxData[4] = (uint8_t)(txPointer);
								spi2RxTxLength = 3+2;
								spi2StartRxTx();
								socketEstablishedSubstate = SES_SettingTxPointer;
								waitCounter++;
							
						
								break;
							//--socketEstablishedSubstate-----------------------------------------------
							case SES_SettingTxPointer:
								
								spi2TxData[0] = (uint8_t)(Sn_CR >> 8); // offset high byte
								spi2TxData[1] = (uint8_t)(Sn_CR); // offset low byte
								spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
								spi2TxData[3] = COMMAND_SEND;
								spi2RxTxLength = 3+1;
								spi2StartRxTx();
								socketEstablishedSubstate = SES_Idle;
								waitCounter++;
							
								txHeartbeat = Heartbeat::getCounterValue();
							
								break;
							//--socketEstablishedSubstate-----------------------------------------------
							case SES_ReadingStatus:
								if (spi2RxData[3]==STATUS_SOCK_CLOSE_WAIT)
								{
									state = SRV_SocketDisconnectCommand;
								}
								else if (spi2RxData[3]==STATUS_SOCK_ESTABLISHED)
								{
									spi2TxData[0] = (uint8_t)(Sn_RX_RSR0 >> 8); // offset high byte
									spi2TxData[1] = (uint8_t)(Sn_RX_RSR0); // offset low byte
									spi2TxData[2] = ACCESS_READ + BSB_Socket0Register + OM_VDM;//command
									spi2TxData[3] = 0x00;
									spi2TxData[3] = 0x00;
									spi2RxTxLength = 3+2;
									spi2StartRxTx();
									socketEstablishedSubstate = SES_GettingRxSize;// first, get the received size
									waitCounter++;
								}
								else
								{
									__asm("	nop");
									state = SRV_SocketInitCommand;
								}
						
								break;
							//--socketEstablishedSubstate-----------------------------------------------
							case SES_GettingRxSize:
				
								rxLength = (spi2RxData[3]<<8)+spi2RxData[4];
								if (rxLength==0)
								{
									socketEstablishedSubstate = SES_Idle;
								}
								else
								{
									spi2TxData[0] = (uint8_t)(Sn_RX_RD0 >> 8); // offset high byte
									spi2TxData[1] = (uint8_t)(Sn_RX_RD0); // offset low byte
									spi2TxData[2] = ACCESS_READ + BSB_Socket0Register + OM_VDM;//command
									spi2TxData[3] = 0x00;
									spi2TxData[4] = 0x00;
									spi2RxTxLength = 3+2;
									spi2StartRxTx();
									socketEstablishedSubstate = SES_GettingRxPointer;// Get offset address		
									waitCounter++;									
									
								}
							
							
							
							
								break;
							//--socketEstablishedSubstate-----------------------------------------------
							case SES_GettingRxPointer:
				
								rxPointer = (spi2RxData[3]<<8)+spi2RxData[4];
								
								spi2TxData[0] = (uint8_t)(rxPointer >> 8); // offset high byte
								spi2TxData[1] = (uint8_t)(rxPointer); // offset low byte
								spi2TxData[2] = ACCESS_READ + BSB_Socket0RxBuffer + OM_VDM;//command
								spi2RxTxLength = 3+rxLength;
								spi2StartRxTx();
								socketEstablishedSubstate = SES_ReadingRxData;		
								waitCounter++;									
							
								break;
							//--socketEstablishedSubstate-----------------------------------------------
							case SES_ReadingRxData:

								for(uint16_t i=0;i<rxLength;i++)
								{
										rxBuffer[rxPointerReceived] = spi2RxData[3+i];
										rxPointerReceived = (rxPointerReceived+1)&(RX_BUFFER_LENGTH-1);
								}

								rxPointer+=rxLength;
							
								spi2TxData[0] = (uint8_t)(Sn_RX_RD0 >> 8); // offset high byte
								spi2TxData[1] = (uint8_t)(Sn_RX_RD0); // offset low byte
								spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
								spi2TxData[3] = (uint8_t)(rxPointer>>8);
								spi2TxData[4] = (uint8_t)(rxPointer);
								spi2RxTxLength = 3+2;
								spi2StartRxTx();
								socketEstablishedSubstate = SES_SettingRxPointer;
								waitCounter++;
							
								break;
							//--socketEstablishedSubstate-----------------------------------------------
							case SES_SettingRxPointer:
				
								spi2TxData[0] = (uint8_t)(Sn_CR >> 8); // offset high byte
								spi2TxData[1] = (uint8_t)(Sn_CR); // offset low byte
								spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
								spi2TxData[3] = COMMAND_RECV;
								spi2RxTxLength = 3+1;
								spi2StartRxTx();
								socketEstablishedSubstate = SES_Idle;
								waitCounter++;

								rxHeartbeat = Heartbeat::getCounterValue();
								
								break;
							//--socketEstablishedSubstate-----------------------------------------------
								
	
					}
					
	
					
					
					
					
				}
			
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SocketDisconnectCommand:
				
				if (spi2RxTxActive==false)
				{
					txPointerTransmitted = txPointerPending;
					
					spi2TxData[0] = (uint8_t)(Sn_CR >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_CR); // offset low byte
					spi2TxData[2] = ACCESS_WRITE + BSB_Socket0Register + OM_VDM;//command
					spi2TxData[3] = COMMAND_DISCON;
					spi2RxTxLength = 3+1;
					spi2StartRxTx();
					state = SRV_SocketDisconnectWait;
					waitCounter = 0;
	
					
					
				}
		
			
				break;
			//-----state-----------------------------------------------------------------------
			case SRV_SocketDisconnectWait:
	
				if (spi2RxTxActive==false)
				{
					if (waitCounter==0)
					{
						
					}
					else
					{
							if (spi2RxData[3]==STATUS_SOCK_CLOSED)
							{
								state = SRV_SocketInitCommand;
								break;
							}
					}
					spi2TxData[0] = (uint8_t)(Sn_SR >> 8); // offset high byte
					spi2TxData[1] = (uint8_t)(Sn_SR); // offset low byte
					spi2TxData[2] = ACCESS_READ + BSB_Socket0Register + OM_VDM;//command
					spi2TxData[3] = 0x00;
					spi2RxTxLength = 3+1;
					spi2StartRxTx();
					state = SRV_SocketDisconnectWait;
					waitCounter++;
	
					
					
				}			
				break;
			//-----state-----------------------------------------------------------------------
			//-----state-----------------------------------------------------------------------
	}


	
	
}
//==============================================================================================
void Servo::pushByte(uint8_t byte){

	txBuffer[txPointerPending] = byte;
	txPointerPending = (txPointerPending+1)&(TX_BUFFER_LENGTH-1);
}
void Servo::pushWordBigEndian(uint16_t word){

	pushByte((uint8_t)(word>>8));
	pushByte((uint8_t)(word));
}
void Servo::popBytes(uint8_t *dst,uint16_t byteCount){

	for(uint16_t i=0;i<byteCount;i++){

		*dst++ = rxBuffer[rxPointerProcessed++];
		rxPointerProcessed &= (RX_BUFFER_LENGTH-1);
	}
}



//==============================================================================================
void Servo::readHoldingRegisters(){
	
	pushWordBigEndian(++modbusTxTransactionIdentifier);
	pushWordBigEndian(0);//protocol ID
	pushWordBigEndian(1+1+2+2);//length
	pushByte(0);//unit ID
	pushByte(3);//Read Holding Registers
	pushWordBigEndian(0);//The Data Address of the first register requested
	pushWordBigEndian(4);//The total number of registers requested
}
//===============================================================================================
void Servo::presetMultipleRegisters(FhppOutputData &data){

	pushWordBigEndian(++modbusTxTransactionIdentifier);
	pushWordBigEndian(0);//protocol ID
	pushWordBigEndian(1+1+2+2+1+8);//length
	pushByte(0);//unit ID
	pushByte(16);//command
	pushWordBigEndian(0);//The Data Address of the first register requested
	pushWordBigEndian(4);//The total number of registers to write
	pushByte(8);//The number of data bytes to follow
	
	uint8_t *pb = (uint8_t*)&data;
	for(uint16_t i = 0; i< 8;i++){
		pushByte(*pb++);
	}
	
	
}
//===============================================================================================
void Servo::getFhppInputData(FhppInputData *data,uint8_t *rxMessage){

	memcpy(&(*data),&rxMessage[9],4);
	uint8_t *pbDst = (uint8_t*)&(data->actualValue2);
	uint8_t *pbSrc = (uint8_t*)&(rxMessage[9+7]);
	for(uint8_t i=0;i<4;i++)
	{
			*pbDst++ = *pbSrc--;
	}
}
//===============================================================================================
bool Servo::checkFhppInputData(FhppInputData *data){

	if (data->scon & SCON_FAULT)//check fault
	{
		return false;
	}
	if ((data->scon & SCON_VLOAD)==0)
	{
		return false;
	}
	if (data->scon & SCON_FCT)
	{
		return false;
	}
	return true;
}
//===============================================================================================
uint16_t Servo::getRxLenght(){
	return (uint16_t)((rxPointerReceived-rxPointerProcessed)&(RX_BUFFER_LENGTH-1));
}
//===============================================================================================
void Servo::flushRx(){
	rxPointerProcessed = rxPointerReceived;
}
//===============================================================================================
void Servo::wizCsAssert(){
	GPIOA->BSRRH = (1<<1);
}
void Servo::wizCsDeassert(){
	GPIOA->BSRRL = (1<<1);
}
//===============================================================================================
void Servo::wizResetAssert(){
	GPIOC->BSRRH = (1<<5);
}
void Servo::wizResetDeassert(){
	GPIOC->BSRRL = (1<<5);
}
//===============================================================================================
void Servo::enable(){
	GPIOB->BSRRL = (1<<12)+(1<<13)+(1<<14)+(1<<15);
}
void Servo::disable(){
	GPIOB->BSRRH = (1<<12)+(1<<13)+(1<<14)+(1<<15);
}
//===============================================================================================


