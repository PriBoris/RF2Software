#include "RtcuV1Test1.h"

uint8_t nfcRxBuffer[NFC_RX_BUFFER_LENGTH];
uint8_t nfcTxBuffer[NFC_TX_BUFFER_LENGTH];

uint16_t nfcPtrReceived;
uint16_t nfcPtrProcessed;

uint16_t nfcPtrPending;
uint16_t nfcPtrTransmitted;

uint8_t nfcState;

uint8_t pn532State;
enum
{
	PN532_Idle=0,
	PN532_WaitingPacketStart1,
	PN532_WaitingPacketStart2,
	PN532_WaitingLength,
	PN532_WaitingLengthChecksum,
	PN532_WaitingData,
	PN532_WaitingDataChecksum,

	PN532_WaitingPostamble,

	PN532_BlockState,


};


uint8_t nfcUIDLength;
uint8_t nfcUID[16];



void nfcInit(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART6, ENABLE);
	nfcPtrReceived = 0;
	nfcPtrProcessed = 0;
	nfcPtrPending = 0;
	nfcPtrTransmitted = 0;
	nfcState = 0;
	

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	hmiTxEnableAssert();

	USART_Cmd(USART6, ENABLE);
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); //Received Data Ready to be Read
	
	
	
	nfcTxEnableAssert();



	
}

//==============================================================================================
extern "C" {void USART6_IRQHandler()
{
	
	if (USART6->SR & USART_IT_RXNE)
	{
		nfcRxBuffer[nfcPtrReceived] = USART6->DR;
		nfcPtrReceived = (nfcPtrReceived+1) & (NFC_RX_BUFFER_LENGTH-1);	
	}

	
}}
//==============================================================================================
void nfcTxProcess(void)
{
	
	if (
		(nfcPtrPending!=nfcPtrTransmitted) && 
		(USART6->SR & USART_FLAG_TXE)
	)
	{
		USART6->DR = nfcTxBuffer[nfcPtrTransmitted];
		nfcPtrTransmitted = (nfcPtrTransmitted+1) & (NFC_TX_BUFFER_LENGTH-1);	

	}	
}
//==============================================================================================
bool nfcRxProcess(void)
{
	bool result = false;
	
	
	//----------------------------------------------------------------------------------
    // process rx data
    if (nfcState==1)
    {
        nfcPtrProcessed = nfcPtrReceived;
    }
    else
    {
		pn532State = PN532_Idle;
		
		uint16_t processLength = (nfcPtrReceived - nfcPtrProcessed) & (NFC_RX_BUFFER_LENGTH-1);
        uint8_t packetLEN;
        uint8_t packetLCS;
        static uint8_t packetData[64];
		uint8_t packetDataLen;
        uint8_t packetDCS;


        for(uint16_t i=0;i<processLength;i++)
        {
            uint8_t byte = nfcRxBuffer[nfcPtrProcessed];
			nfcPtrProcessed = (nfcPtrProcessed+1) & (NFC_RX_BUFFER_LENGTH-1);

            switch(pn532State)
            {
            //----------------------------------------------------------
            case PN532_BlockState:
                break;
            //----------------------------------------------------------
            default:
                pn532State = PN532_Idle;
            case PN532_Idle:
                if (byte==0x00)
                {
                    pn532State = PN532_WaitingPacketStart1;
                }
                break;
            //----------------------------------------------------------
            case PN532_WaitingPacketStart1:
                if (byte==0x00)
                {
                    pn532State = PN532_WaitingPacketStart2;
                }
                break;
            //----------------------------------------------------------
            case PN532_WaitingPacketStart2:
                if (byte==0xFF)
                {
                    pn532State = PN532_WaitingLength;
                }
                break;
            //----------------------------------------------------------
            case PN532_WaitingLength:
                packetLEN = byte;
                pn532State = PN532_WaitingLengthChecksum;
                break;
            //----------------------------------------------------------
            case PN532_WaitingLengthChecksum:
                packetLCS = byte;
                if (
                    (packetLEN>0) &&
                    ((uint8_t)(packetLEN+packetLCS)==0) && 
					(packetLEN <= sizeof(packetData))
                   )
                {
                    pn532State = PN532_WaitingData;
					packetDataLen = 0;
                }
                else
                {
                    pn532State = PN532_Idle;
                }
                break;
            //----------------------------------------------------------
            case PN532_WaitingData:
                packetData[packetDataLen++] = byte;
                if (packetDataLen==packetLEN)
                {
                    pn532State = PN532_WaitingDataChecksum;
                }
                break;
            //----------------------------------------------------------
            case PN532_WaitingDataChecksum:

				packetDCS = byte;
                for(uint8_t j=0;j<packetDataLen;j++)
                {
                    packetDCS += packetData[j];
                }
                if (packetDCS==0x00)
                {
                    if (
                        (packetData[0]==0xD5)&&
                        (packetData[1]==0x4B)&&
                        (packetData[2]==0x01)&&
                        (packetData[3]==0x01)&&
                        ((packetData[7]==4)||(packetData[7]==7))&&
                        (packetDataLen>=12)
                    )
                    {
                        nfcUIDLength = packetData[7];
						memcpy(nfcUID,&packetData[8],nfcUIDLength);
						result = true;
                    }
                }
                pn532State = PN532_WaitingPostamble;
                break;
            //----------------------------------------------------------
            case PN532_WaitingPostamble:
                pn532State = PN532_Idle;
                break;
            //----------------------------------------------------------
            }



       }

       nfcPtrProcessed = nfcPtrReceived;
 


	}


	//----------------------------------------------------------------------------------
	// send next request
    if (nfcState==0)
    {
        //sendSAMConfiguration();
		{
		
			nfcPushByte(0x55);
			nfcPushByte(0x55);
			nfcPushByte(0x00);
			nfcPushByte(0x00);
			nfcPushByte(0x00);
		
			nfcPushByte(0x00);//PREAMBLE
			nfcPushByte(0x00);//START CODE1
			nfcPushByte(0xFF);//START CODE2
		
			uint8_t dataLen = 3;
			uint8_t lcs = 0x00-dataLen;
 			nfcPushByte(dataLen);
			nfcPushByte(lcs);

			nfcPushByte(0xD4);//TFI = frame from the host controller to the PN532
			nfcPushByte(0x14);//PD0 = SAMConfiguration
			nfcPushByte(0x01);//PD1 = Normal mode

			uint8_t dcs = 0x00 - 0xD4 - 0x14 - 0x01;
			nfcPushByte(dcs);
			nfcPushByte(0x00);//POSTAMBLE
			
		}

    }
    else
    {
        //sendACK();
		nfcPushByte(0x00);//PREAMBLE
		nfcPushByte(0x00);//START CODE1
		nfcPushByte(0xFF);//START CODE2
		nfcPushByte(0x00);
		nfcPushByte(0xFF);
		nfcPushByte(0x00);//POSTAMBLE
		
        //sendInListPassiveTarget();
		{
		
			nfcPushByte(0x00);//PREAMBLE
			nfcPushByte(0x00);//START CODE1
			nfcPushByte(0xFF);//START CODE2
		
			uint8_t dataLen = 4;
			uint8_t lcs = 0x00-dataLen;
 			nfcPushByte(dataLen);
			nfcPushByte(lcs);

			nfcPushByte(0xD4);//TFI = frame from the host controller to the PN532
			nfcPushByte(0x4A);//PD0 = Command Code = InListPassiveTarget
			nfcPushByte(0x01);//MaxTg
			nfcPushByte(0x00);//BrTy

			uint8_t dcs = 0x00 - 0xD4 - 0x4A - 0x01 - 0x00;
			nfcPushByte(dcs);
			nfcPushByte(0x00);//POSTAMBLE
			
		}		
		
		
    }
	//----------------------------------------------------------------------------------
    nfcState++;
    if (nfcState==10)
    {
        nfcState = 0;
    }
	//----------------------------------------------------------------------------------
	
	
	
	return result;
}
//==============================================================================================
void nfcPushByte(uint8_t txByte)
{
	nfcTxBuffer[nfcPtrPending] = txByte;	
	nfcPtrPending = (nfcPtrPending+1) & (NFC_TX_BUFFER_LENGTH-1);
	
}
//==============================================================================================




