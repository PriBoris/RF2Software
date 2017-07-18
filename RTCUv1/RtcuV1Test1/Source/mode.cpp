
#include "mode.h"

#include "hmi/hmi.h"
#include "hmi/RxMessageQueue.h"


Mode mode;
Submode submode;
extern void modeProcess();


uint16_t modbusTxTransactionIdentifier = 0;



FhppInputData fhppInputData;

FhppOutputData fhppOutputData;



ErrorType errorType;


uint8_t modbusRxMessage[17];

PersonalSettings personalSettings;
PersonalSettingsEx personalSettingsEx;

bool personalSettingsOk;

MachineSettings machineSettings;


bool excerciseSettingsOk;
int32_t isokineticSetCount;
int32_t isokineticSetIndex;
IsokineticSetSettings isokineticSetSettings[MAX_SET_COUNT];


int32_t motionCompletePause = 0;
bool personalIncrement;
int32_t personalPause;
uint8_t personalActiveButton;
int32_t timeToTest;
int32_t timeToSet;
int32_t timeFirstInterruption;
int32_t timeSecondInterruption;
int32_t repsIndex;
bool testIsConcentric;

Phase phase;




int32_t int32Abs(int32_t arg)
{
	if (arg>=0)
	{
		return arg;
	}
	else
	{
		return -arg;
	}
}

int32_t int32Reversed(int32_t arg)
{
	int32_t result;
	uint8_t *pSrc = (uint8_t*)&arg;
	uint8_t *pDst = (uint8_t*)&result;
	pDst[0] = pSrc[3];
	pDst[1] = pSrc[2];
	pDst[2] = pSrc[1];
	pDst[3] = pSrc[0];
	
	
	return result;
}


//=================================================================================================================
void modeInit()
{
	mode = MODE_INITIALIZING;
	submode = SUBMODE_INITIALIZING_Start;
	
	{
		personalSettingsOk = false;
		memset(&personalSettings,0,sizeof(personalSettings));
		memset(&personalSettingsEx,0,sizeof(personalSettingsEx));
	}
	
	{	
		excerciseSettingsOk = false;
		isokineticSetCount = 0;
	}
	
	{
		memset(&machineSettings,0,sizeof(machineSettings));
//		machineSettings.positionMainMin = 100;//steps
//		machineSettings.positionMainMax = 20000;//steps
		machineSettings.positionMainMin = 11000;//steps
		machineSettings.positionMainMax = 20000;//steps

		machineSettings.positionAux1Max = 99;
		machineSettings.positionAux1Min = 1;
		machineSettings.positionAux2Max = 99;
		machineSettings.positionAux2Min = 1;
		machineSettings.positionAux3Max = 99;
		machineSettings.positionAux3Min = 1;

		int32_t dX = machineSettings.positionMainMax - machineSettings.positionMainMin;
		/*
			424.00 mm/s > 42400 steps/s > 42.400 steps/ms
		*/
		double speedAbsMainMax = (double)dX/(double)42.400;
		machineSettings.speedAbsMainMax = (int32_t)(speedAbsMainMax + 0.5);	
		machineSettings.speedAbsMainPersonal = (int32_t)(speedAbsMainMax*20 + 0.5);
	}
	
	memset(&isokineticSetSettings[0],0,sizeof(isokineticSetSettings));
	
	{
		memset(&fhppOutputData,0,sizeof(fhppOutputData));
		fhppOutputData.cdir = CDIR_POSITION_MODE;
		
	}
}
//=================================================================================================================
void modeProcess()
{
	
	if (strainGaugeFault==true)
	{
		mode = MODE_ERROR;
		submode = SUBMODE_ERROR;
		errorType = ERROR_StrainGaugeLost;
	}
	
	if ((GPIOC->IDR&(1<<14))==0)//stop button
	{
		mode = MODE_ERROR;
		submode = SUBMODE_ERROR;
		errorType = ERROR_StoppedManually;
	}
	
	
	
	
	if (true==nfcRxProcess()){ 
		// TODO: move this section to the bottom of modeProcess()	
	
		uint8_t messageToHmi[4+7];
		int32_t uidLength = (int32_t)nfcUIDLength;
		memcpy(&messageToHmi[0],&uidLength,sizeof(uidLength));
		memcpy(&messageToHmi[4],&nfcUID[0],uidLength);
		HMI::protocol.sendPacket(Protocol::TAG_RfidProximity,messageToHmi,4+uidLength);										
	}
		
	
	
	//cancel
	switch(submode){
	case SUBMODE_PARKING_Running1:
	case SUBMODE_PARKING_Running2:
	case SUBMODE_PARKING_Running3:
	case SUBMODE_TEST_Parking1:
	case SUBMODE_TEST_Parking2:
	case SUBMODE_TEST_Parking3:
	case SUBMODE_TEST_Pause:
	case SUBMODE_TEST_Running1:
	case SUBMODE_TEST_Running2:
	case SUBMODE_TEST_Running3:
	case SUBMODE_EXCERCISE_ISOKINETIC_Start:
	case SUBMODE_EXCERCISE_ISOKINETIC_Parking1:
	case SUBMODE_EXCERCISE_ISOKINETIC_Parking2:
	case SUBMODE_EXCERCISE_ISOKINETIC_Parking3:
	case SUBMODE_EXCERCISE_ISOKINETIC_Pause:
	case SUBMODE_EXCERCISE_ISOKINETIC_PrepareFirstMovement:
	case SUBMODE_EXCERCISE_ISOKINETIC_StartFirstMovement1:
	case SUBMODE_EXCERCISE_ISOKINETIC_StartFirstMovement2:
	case SUBMODE_EXCERCISE_ISOKINETIC_KeepFirstMovement:
	case SUBMODE_EXCERCISE_ISOKINETIC_FirstInterruption:
	case SUBMODE_EXCERCISE_ISOKINETIC_StartSecondMovement1:
	case SUBMODE_EXCERCISE_ISOKINETIC_StartSecondMovement2:
	case SUBMODE_EXCERCISE_ISOKINETIC_KeepSecondMovement:
	case SUBMODE_EXCERCISE_ISOKINETIC_SecondInterruption:		

		if (true==RxMessageQueue::cancelMessageReceived()){
			
			flushServoRx();
			
			fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE/* + CCON_STOP*/;
			fhppOutputData.cpos = CPOS_HALT;
			servoPresetMultipleRegisters(fhppOutputData);
			submode = SUBMODE_CANCEL_1;
			return;
		}

		break;
		
	}
	
	
	
	
	uint16_t servoResponseLength = getServoRxLenght();
	switch(submode)
	{
		//----submode----------------------------------------------------------
		default:
			setErrorMode(ERROR_RtcuLogicError);
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_INITIALIZING_Start:
			
			servoDisable();
			RxMessageQueue::flush();
		
			submode = SUBMODE_INITIALIZING_WaitingServoConnection;
		
			{
				uint8_t messageToHmi[1+sizeof(TCurrentDateTime)+8];
				messageToHmi[0] = mode;
				memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

				uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
				memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

				uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
				memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
			}
		
		
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_INITIALIZING_WaitingServoConnection:
			
			servoDisable();
			RxMessageQueue::flush();

			{
				uint8_t messageToHmi[1+sizeof(TCurrentDateTime)+8];
				messageToHmi[0] = mode;
				memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

				uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
				memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

				uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
				memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
			}

		
			{
				if (servoResponseLength!=MODBUS_READ_RESPONSE_LENGTH)
				{
					// continue waiting
					servoRxPointerProcessed = servoRxPointerReceived;
					servoReadHoldingRegisters();
				}
				else
				{
					servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
					servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);
					
					//check fault
					if (fhppInputData.scon & SCON_FAULT)
					{
							setErrorMode(ERROR_ServoInternal);
							return;
					}
					
					//wait load voltage and fieldbus access
					if (
							((fhppInputData.scon & SCON_VLOAD)!=0) &&
							((fhppInputData.scon & SCON_FCT)==0)
					)
					{
							memset(&fhppOutputData,0,sizeof(fhppOutputData));
							fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK;
							servoPresetMultipleRegisters(fhppOutputData);
							submode = SUBMODE_INITIALIZING_DisablingSoftwareAccess;
							return;
					}
					
					// continue waiting
					servoReadHoldingRegisters();
					
					
					
				}
				

		
			}
		
		
		
		
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_INITIALIZING_DisablingSoftwareAccess:
			
			servoDisable();
			RxMessageQueue::flush();

			{
				uint8_t messageToHmi[1+sizeof(TCurrentDateTime)+8];
				messageToHmi[0] = mode;
				memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

				uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
				memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));


				uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
				memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

				HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
			}

		
			{
				if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
				{
						servoRxPointerProcessed = servoRxPointerReceived;
						mode = MODE_IDLE;
						submode = SUBMODE_IDLE;
						servoReadHoldingRegisters();
				}
				else
				{
						servoRxPointerProcessed = servoRxPointerReceived;
						setErrorMode(ERROR_ServoConnectionLost);
				}
			}


			break;
		//----submode----------------------------------------------------------
		case SUBMODE_IDLE:
			
			servoDisable();
		
			{
				if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
				{
						servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
						servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								

						
						if (fhppInputData.scon & SCON_FAULT)//check fault
						{
								setErrorMode(ERROR_ServoInternal);
								return;
						}
						if ((fhppInputData.scon & SCON_VLOAD)==0)
						{
								setErrorMode(ERROR_ServoVoltageLost);
								return;
						}
						if (fhppInputData.scon & SCON_FCT)
						{
								setErrorMode(ERROR_ServoControlLost);
								return;
						}

						{
							uint8_t messageToHmi[1+sizeof(TCurrentDateTime)+8+4*4];
							messageToHmi[0] = mode;
							memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

							uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
							memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

							uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
							memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

							memcpy(
									&messageToHmi[1+sizeof(TCurrentDateTime)+8],
									&fhppInputData.actualValue2,
									sizeof(fhppInputData.actualValue2)
									); //position
							memset(&messageToHmi[1+sizeof(TCurrentDateTime)+8+4],0,12);
							HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
						}

						{
							RxMessage *message = RxMessageQueue::pop();
							if (message!=NULL){
								if (
									(message->tag==Protocol::TAG_EnableServo)&&
									(message->valueLen==0)
									){
									mode = MODE_WAITING;
									submode = SUBMODE_WAITING_HaltingServo;
									RxMessageQueue::flush();
									
									servoEnable();
									memset(&fhppOutputData,0,sizeof(fhppOutputData));
									fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
									fhppOutputData.cpos = CPOS_HALT;
									servoPresetMultipleRegisters(fhppOutputData);
									
									return;


								}else{
									__asm("	nop");
								}
							}
							
							
						}
						
						

						
						
						
						mode = MODE_IDLE;
						servoReadHoldingRegisters();
						
						
						
				}
				else
				{
						servoRxPointerProcessed = servoRxPointerReceived;
						setErrorMode(ERROR_ServoConnectionLost);
				}
			}
		
		
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_WAITING_HaltingServo:	

			servoEnable();
			RxMessageQueue::flush();
				
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					servoReadHoldingRegisters();

					mode = MODE_WAITING;
					submode = SUBMODE_WAITING_ReceivingSettings;
				
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}


		
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_WAITING_ReceivingSettings:
			
			servoEnable();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (fhppInputData.scon & SCON_FAULT)//check fault
				{
						setErrorMode(ERROR_ServoInternal);
						return;
				}
				if ((fhppInputData.scon & SCON_VLOAD)==0)
				{
						setErrorMode(ERROR_ServoVoltageLost);
						return;
				}
				if (fhppInputData.scon & SCON_FCT)
				{
						setErrorMode(ERROR_ServoControlLost);
						return;
				}							

				{
					uint8_t messageToHmi[1+sizeof(TCurrentDateTime)+8+16];
					messageToHmi[0] = mode;
					memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

					uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

					uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

					memcpy(
							&messageToHmi[1+sizeof(TCurrentDateTime)+8],
							&fhppInputData.actualValue2,
							sizeof(fhppInputData.actualValue2)
							); //position
					memset(&messageToHmi[1+sizeof(TCurrentDateTime)+8+4],0,12);
					HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
				}
				
				{
					// send settings once a second
					static uint8_t settingsReportTimeout = 0;
					settingsReportTimeout++;
					if (settingsReportTimeout==9)
					{
						settingsReportTimeout=0;
						if (personalSettingsOk==false)
						{
							HMI::protocol.sendPacket(Protocol::TAG_ReportPersonalSettings,0,0);										
						}
						else
						{
							HMI::protocol.sendPacket(Protocol::TAG_ReportPersonalSettings,(uint8_t*)&personalSettings,sizeof(PersonalSettings));										
						}
						
						HMI::protocol.sendPacket(Protocol::TAG_ReportMachineSettings,(uint8_t*)&machineSettings,sizeof(MachineSettings));										
						
						if (excerciseSettingsOk==false)
						{
							HMI::protocol.sendPacket(Protocol::TAG_ReportExcerciseSettings,0,0);										
						}
						else
						{
							HMI::protocol.sendPacket(Protocol::TAG_ReportExcerciseSettings,(uint8_t*)&isokineticSetSettings[0],sizeof(IsokineticSetSettings)*isokineticSetCount);										
						}
					}
				}

				RxMessage *message = RxMessageQueue::pop();
				if (message!=NULL){
					
						//-------------------------------------------------------------------------------
						if (
							(message->tag==Protocol::TAG_LoadPersonalSettings)&&
							(message->valueLen==sizeof(PersonalSettings))
						)
						{
							PersonalSettings newPersonalSettings;
							memcpy(&newPersonalSettings,message->value,sizeof(PersonalSettings));
							
							// A,B settings check
							if (newPersonalSettings.positionMainA > machineSettings.positionMainMax){
								newPersonalSettings.positionMainA = machineSettings.positionMainMax;
							} else if (newPersonalSettings.positionMainA < machineSettings.positionMainMin){
								newPersonalSettings.positionMainA = machineSettings.positionMainMin;
							}
							if (newPersonalSettings.positionMainB > machineSettings.positionMainMax){
								newPersonalSettings.positionMainB = machineSettings.positionMainMax;
							} else if (newPersonalSettings.positionMainB < machineSettings.positionMainMin){
								newPersonalSettings.positionMainB = machineSettings.positionMainMin;
							}
							int32_t minABDistance = (machineSettings.positionMainMax - machineSettings.positionMainMin)/20;
							int32_t absABDistance = newPersonalSettings.positionMainA - newPersonalSettings.positionMainB;
							if (absABDistance<0){
								absABDistance = -absABDistance;
							}
							if (absABDistance<minABDistance){
								setErrorMode(ERROR_HmiParametersError);
								return;
							}
							{
								double maxSpeedRel = 
									(double)machineSettings.speedAbsMainMax 
									* (double)absABDistance 
									/ (double)(machineSettings.positionMainMax - machineSettings.positionMainMin);
								double minSpeedRel = maxSpeedRel*100;
								personalSettingsEx.maxSpeedRel = (int32_t)(maxSpeedRel+0.5);
								personalSettingsEx.minSpeedRel = (int32_t)(minSpeedRel+0.5);
								personalSettingsEx.speedRelToAbsConversionCoeff = 
									100.0
									* (double)machineSettings.speedAbsMainMax
									/ (double)(machineSettings.positionMainMax - machineSettings.positionMainMin)
									* (double)absABDistance
									;
							}
							
							//test speed check
							if (newPersonalSettings.speedRelMainTestEccentric>personalSettingsEx.minSpeedRel){
								newPersonalSettings.speedRelMainTestEccentric = personalSettingsEx.minSpeedRel;								
							} else if (newPersonalSettings.speedRelMainTestEccentric<personalSettingsEx.maxSpeedRel){
								newPersonalSettings.speedRelMainTestEccentric = personalSettingsEx.maxSpeedRel;								
							}
							if (newPersonalSettings.speedRelMainTestConcentric>personalSettingsEx.minSpeedRel){
								newPersonalSettings.speedRelMainTestConcentric = personalSettingsEx.minSpeedRel;								
							} else if (newPersonalSettings.speedRelMainTestConcentric<personalSettingsEx.maxSpeedRel){
								newPersonalSettings.speedRelMainTestConcentric = personalSettingsEx.maxSpeedRel;								
							}
							
							// parking settings check
							if (newPersonalSettings.positionMainParking > machineSettings.positionMainMax){
								newPersonalSettings.positionMainParking = machineSettings.positionMainMax;
							} else if (newPersonalSettings.positionMainParking < machineSettings.positionMainMin){
								newPersonalSettings.positionMainParking = machineSettings.positionMainMin;
							}
							if (newPersonalSettings.speedAbsMainParking<machineSettings.speedAbsMainMax){
								newPersonalSettings.speedAbsMainParking = machineSettings.speedAbsMainMax;
							} else if (newPersonalSettings.speedAbsMainParking>(machineSettings.speedAbsMainMax*100)){
								newPersonalSettings.speedAbsMainParking = machineSettings.speedAbsMainMax*100;
							}
							
							
							
							//aux settings check
							if (newPersonalSettings.positionAux1 > machineSettings.positionAux1Max){
								newPersonalSettings.positionAux1 = machineSettings.positionAux1Max;
							} else if (newPersonalSettings.positionAux1 < machineSettings.positionAux1Min){
								newPersonalSettings.positionAux1 = machineSettings.positionAux1Min;
							}
							if (newPersonalSettings.positionAux2 > machineSettings.positionAux2Max){
								newPersonalSettings.positionAux2 = machineSettings.positionAux2Max;
							} else if (newPersonalSettings.positionAux2 < machineSettings.positionAux2Min){
								newPersonalSettings.positionAux2 = machineSettings.positionAux2Min;
							}
							if (newPersonalSettings.positionAux3 > machineSettings.positionAux3Max){
								newPersonalSettings.positionAux3 = machineSettings.positionAux3Max;
							} else if (newPersonalSettings.positionAux3 < machineSettings.positionAux3Min){
								newPersonalSettings.positionAux3 = machineSettings.positionAux3Min;
							}
							
							
							
							memcpy(&personalSettings,&newPersonalSettings,sizeof(PersonalSettings));
							personalSettingsOk = true;
						}
						//-------------------------------------------------------------------------------
						else if (
							(message->tag==Protocol::TAG_LoadMachineSettings)&&
							(message->valueLen==sizeof(MachineSettings))
						)
						{
							MachineSettings newMachineSettings;
							memcpy(&newMachineSettings,message->value,sizeof(MachineSettings));
							
							if (newMachineSettings.speedAbsMainPersonal<machineSettings.speedAbsMainMax){
								machineSettings.speedAbsMainPersonal = machineSettings.speedAbsMainMax;
							} else if (newMachineSettings.speedAbsMainPersonal>(machineSettings.speedAbsMainMax*100)){
								machineSettings.speedAbsMainPersonal = machineSettings.speedAbsMainMax*100;
							} else {
								machineSettings.speedAbsMainPersonal = newMachineSettings.speedAbsMainPersonal;
							}
							
						}
						//-------------------------------------------------------------------------------
						else if (
							(message->tag==Protocol::TAG_Parking)&&
							(message->valueLen==0)&&
							(personalSettingsOk==true)
						)
						{
							fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
							fhppOutputData.cpos = CPOS_HALT+CPOS_START;
							fhppOutputData.setpointValue1 = getParkingSpeed();
							fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainParking);
							servoPresetMultipleRegisters(fhppOutputData);
						
							mode = MODE_PARKING;
							submode = SUBMODE_PARKING_Running1;
							return;
						}
						//-------------------------------------------------------------------------------
						else if (
							(message->tag==Protocol::TAG_Personal)&&
							(message->valueLen==0)
						)
						{
							mode = MODE_PERSONAL;
							submode = SUBMODE_PERSONAL_Waiting;
							servoReadHoldingRegisters();
							return;
						}
						//-------------------------------------------------------------------------------
						else if (
							(message->tag==Protocol::TAG_TestConcentric)&&
							(message->valueLen==4)&&
							(personalSettingsOk==true)
						)
						{
							
							memcpy(&timeToTest,&(message->value[0]),4);//TODO: check timeToTest value
							fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
							fhppOutputData.cpos = CPOS_HALT+CPOS_START;
							fhppOutputData.setpointValue1 = getParkingSpeed();
							fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainB);
							servoPresetMultipleRegisters(fhppOutputData);
							
							testIsConcentric = true;
							mode = MODE_TEST_CONCENTRIC;
							submode = SUBMODE_TEST_Parking1;
							return;
						}
						//-------------------------------------------------------------------------------
						else if (
							(message->tag==Protocol::TAG_TestEccentric)&&
							(message->valueLen==4)&&
							(personalSettingsOk==true)
						)
						{
							
							memcpy(&timeToTest,&(message->value[0]),4);//TODO: check timeToTest value
							fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
							fhppOutputData.cpos = CPOS_HALT+CPOS_START;
							fhppOutputData.setpointValue1 = getParkingSpeed();
							fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainA);
							servoPresetMultipleRegisters(fhppOutputData);
							
							testIsConcentric = false;
							mode = MODE_TEST_ECCENTRIC;
							submode = SUBMODE_TEST_Parking1;
							return;
						}
						//-------------------------------------------------------------------------------
						else if (
							(personalSettingsOk==true) &&
							(message->tag==Protocol::TAG_LoadExcerciseSettings)&&
							(message->valueLen!=0)&&
							((message->valueLen%sizeof(IsokineticSetSettings))==0)
						)
						{
							// TODO: check all parameters
							isokineticSetCount = message->valueLen / sizeof(IsokineticSetSettings);
							if (isokineticSetCount>MAX_SET_COUNT){
								isokineticSetCount = MAX_SET_COUNT;
							}

							memcpy(&isokineticSetSettings[0],message->value,sizeof(IsokineticSetSettings)*isokineticSetCount);
							
							for(int32_t set=0;set<isokineticSetCount;set++){
							
								if (isokineticSetSettings[set].speedAB > personalSettingsEx.minSpeedRel){
									isokineticSetSettings[set].speedAB = personalSettingsEx.minSpeedRel;								
								} else if (isokineticSetSettings[set].speedAB < personalSettingsEx.maxSpeedRel){
									isokineticSetSettings[set].speedAB = personalSettingsEx.maxSpeedRel;								
								}

								if (isokineticSetSettings[set].speedBA > personalSettingsEx.minSpeedRel){
									isokineticSetSettings[set].speedBA = personalSettingsEx.minSpeedRel;								
								} else if (isokineticSetSettings[set].speedBA < personalSettingsEx.maxSpeedRel){
									isokineticSetSettings[set].speedBA = personalSettingsEx.maxSpeedRel;								
								}
								
								if (isokineticSetSettings[set].pauseTime < 1000){
									isokineticSetSettings[set].pauseTime = 1000;
								}

								if (isokineticSetSettings[set].firstInterruptionTime < 0){
									isokineticSetSettings[set].firstInterruptionTime = 0;
								}
								if (isokineticSetSettings[set].secondInterruptionTime < 0){
									isokineticSetSettings[set].secondInterruptionTime = 0;
								}

								if (isokineticSetSettings[set].startPoint>1){
									isokineticSetSettings[set].startPoint = 1;
								}
								if (isokineticSetSettings[set].startPoint<0){
									isokineticSetSettings[set].startPoint = 0;
								}
								
								if (isokineticSetSettings[set].repsCount<1){
									isokineticSetSettings[set].repsCount = 1;
								}
								
								//TODO: other checks here
								
							}
							
							
							
							
							
							
							excerciseSettingsOk = true;
						}
						//-------------------------------------------------------------------------------
						else if (
							(message->tag==Protocol::TAG_ExcerciseIsokinetic)&&
							(excerciseSettingsOk==true)&&
							(message->valueLen==0)
						)
						{
							isokineticSetIndex=0;
							mode = MODE_EXCERCISE_ISOKINETIC;
							submode = SUBMODE_EXCERCISE_ISOKINETIC_Start;
							servoReadHoldingRegisters();
							return;
							
						}
						//-------------------------------------------------------------------------------
						
				}
										
				mode = MODE_WAITING;
				submode = SUBMODE_WAITING_ReceivingSettings;
				servoReadHoldingRegisters();
				
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
				
			}
				
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_PARKING_Running1:
						
			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
					servoRxPointerProcessed = servoRxPointerReceived;
				
					fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
					fhppOutputData.cpos = CPOS_HALT;
					//fhppOutputData.setpointValue1 = DEFAULT_PARKING_SPEED;
					//fhppOutputData.setpointValue2 = int32Reversed(DEFAULT_PARKING_POSITION);
					servoPresetMultipleRegisters(fhppOutputData);
				
					mode = MODE_PARKING;
					submode = SUBMODE_PARKING_Running2;
				
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}

			break;
		//----submode----------------------------------------------------------
		case SUBMODE_PARKING_Running2:
			servoEnable();
			RxMessageQueue::flush();
					
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					servoReadHoldingRegisters();
					mode = MODE_PARKING;
					submode = SUBMODE_PARKING_Running3;
					motionCompletePause = 0;
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}
	
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_PARKING_Running3:
			servoEnable();
			RxMessageQueue::flush();
					
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
					servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
					servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
					if (false==servoCheckFhppInputData(&fhppInputData))
					{
						setErrorMode(ERROR_ServoInternal);
						return;
					}

					{
						uint8_t messageToHmi[1+sizeof(TCurrentDateTime)+8+16];
						messageToHmi[0] = mode;
						memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

						uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

						uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

						memcpy(
								&messageToHmi[1+sizeof(TCurrentDateTime)+8],
								&fhppInputData.actualValue2,
								sizeof(fhppInputData.actualValue2)
								); //position
						memset(&messageToHmi[1+sizeof(TCurrentDateTime)+8+4],0,12);
						HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
					}
				
					
					if (fhppInputData.spos & SPOS_MC)
					{
						motionCompletePause++;
						if (motionCompletePause==5)
						{
							mode = MODE_WAITING;
							submode = SUBMODE_WAITING_ReceivingSettings;
						}
					}
					else
					{
						mode = MODE_PARKING;
						submode = SUBMODE_PARKING_Running3;
						motionCompletePause = 0;
					}

					
					servoReadHoldingRegisters();
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}
	
	
	
	
			break;
		//----submode----------------------------------------------------------
		//----submode----------------------------------------------------------
		case SUBMODE_PERSONAL_Waiting:

			servoEnable();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}

				{
					uint8_t messageToHmi[1+sizeof(TCurrentDateTime)+8+16];
					messageToHmi[0] = mode;
					memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

					uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));


					uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

					memcpy(
							&messageToHmi[1+sizeof(TCurrentDateTime)+8],
							&fhppInputData.actualValue2,
							sizeof(fhppInputData.actualValue2)
							); //position
					memset(&messageToHmi[1+sizeof(TCurrentDateTime)+8+4],0,12);
					HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
				}

				RxMessage *message = RxMessageQueue::pop();
				if (message!=NULL){

					if (
						(message->tag==Protocol::TAG_PersonalExit)&&
						(message->valueLen==0)
					)
					{
						servoReadHoldingRegisters();
						mode = MODE_WAITING;
						submode = SUBMODE_WAITING_ReceivingSettings;
						return;
						
					}
					else if (
						(message->tag==Protocol::TAG_PersonalButtonPressed)&&
						(message->valueLen==1)
					)
					{
						switch(message->value[0])//buttonID
						{
							case BUTTONID_MAIN_INCREMENT:

								fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
								fhppOutputData.cpos = CPOS_HALT+CPOS_START;
								fhppOutputData.setpointValue1 = getPersonalSpeed();
								fhppOutputData.setpointValue2 = int32Reversed(machineSettings.positionMainMax);
								servoPresetMultipleRegisters(fhppOutputData);
							
								personalIncrement = true;
								submode = SUBMODE_PERSONAL_StartMovement1;
								personalActiveButton = BUTTONID_MAIN_INCREMENT;
							
								return;
							case BUTTONID_MAIN_DECREMENT:

								fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
								fhppOutputData.cpos = CPOS_HALT+CPOS_START;
								fhppOutputData.setpointValue1 = getPersonalSpeed();
								fhppOutputData.setpointValue2 = int32Reversed(machineSettings.positionMainMin);
								servoPresetMultipleRegisters(fhppOutputData);
							
								personalIncrement = false;
								submode = SUBMODE_PERSONAL_StartMovement1;
								
								personalActiveButton = BUTTONID_MAIN_DECREMENT;
							
								return;
							case BUTTONID_AUX1_INCREMENT:
							case BUTTONID_AUX1_DECREMENT:
							case BUTTONID_AUX2_INCREMENT:
							case BUTTONID_AUX2_DECREMENT:
							case BUTTONID_AUX3_INCREMENT:
							case BUTTONID_AUX3_DECREMENT:
								break;
						}
						
						
						
					}
				
				}
				
				
				
				
				servoReadHoldingRegisters();
			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}
			break;
		//----submode----------------------------------------------------------
		//----submode----------------------------------------------------------
		case SUBMODE_PERSONAL_StartMovement1:

			servoEnable();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				
				fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
				fhppOutputData.cpos = CPOS_HALT;
				servoPresetMultipleRegisters(fhppOutputData);
				submode = SUBMODE_PERSONAL_StartMovement2;
				
			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}

	
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_PERSONAL_StartMovement2:	

			servoEnable();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				submode = SUBMODE_PERSONAL_KeepMovement;
				personalPause = 2;
				servoReadHoldingRegisters();
			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_PERSONAL_KeepMovement:	

			servoEnable();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);		
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}
				{
					uint8_t messageToHmi[1+sizeof(TCurrentDateTime)+8+16];
					messageToHmi[0] = mode;
					memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

					uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

					uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

					memcpy(
							&messageToHmi[1+sizeof(TCurrentDateTime)+8],
							&fhppInputData.actualValue2,
							sizeof(fhppInputData.actualValue2)
							); //position
					memset(&messageToHmi[1+sizeof(TCurrentDateTime)+8+4],0,12);
					HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
				}
				
				RxMessage *message = RxMessageQueue::pop();
				if (message!=NULL){
					
					if (
						(message->tag==Protocol::TAG_PersonalButtonReleased)&&
						(message->valueLen==1)&&
						(message->value[0]==personalActiveButton)
					)
					{
						fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE/* + CCON_STOP*/;
						fhppOutputData.cpos = CPOS_HALT;
						servoPresetMultipleRegisters(fhppOutputData);
						submode = SUBMODE_PERSONAL_StopMovement1;
						return;

					}
					else if (
						(message->tag==Protocol::TAG_PersonalButtonHold)&&
						(message->valueLen==1)&&
						(message->value[0]==personalActiveButton)
					)
					{
						personalPause=0;
					}
				}
		
				
				personalPause++;
				if (personalPause>2)
				{
					fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE/* + CCON_STOP*/;
					fhppOutputData.cpos = CPOS_HALT;

					servoPresetMultipleRegisters(fhppOutputData);
					submode = SUBMODE_PERSONAL_StopMovement1;
					return;
				}
				servoReadHoldingRegisters();
				
			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_PERSONAL_StopMovement1:

			servoEnable();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
				fhppOutputData.cpos = CPOS_HALT;

				servoPresetMultipleRegisters(fhppOutputData);
				submode = SUBMODE_PERSONAL_StopMovement2;
				
			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}
		
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_PERSONAL_StopMovement2:

			servoEnable();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				submode = SUBMODE_PERSONAL_Waiting;
				servoReadHoldingRegisters();
			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}
		
			break;
		//----submode----------------------------------------------------------
		//----submode----------------------------------------------------------
		case SUBMODE_TEST_Parking1:

			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
					fhppOutputData.cpos = CPOS_HALT;
					servoPresetMultipleRegisters(fhppOutputData);
				
					submode = SUBMODE_TEST_Parking2;
				
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}

			break;
		//----submode----------------------------------------------------------
		case SUBMODE_TEST_Parking2:	
			
			servoEnable();
			RxMessageQueue::flush();
					
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				servoReadHoldingRegisters();
				submode = SUBMODE_TEST_Parking3;
				motionCompletePause = 0;
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}
			
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_TEST_Parking3:

			servoEnable();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
					servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
					servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
					if (false==servoCheckFhppInputData(&fhppInputData))
					{
						setErrorMode(ERROR_ServoInternal);
						return;
					}

					{
						uint8_t messageToHmi[27+8];
						
						messageToHmi[0] = mode;
						
						memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

						uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

						uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

						memcpy(
								&messageToHmi[7+8],
								&fhppInputData.actualValue2,
								4); //positionMain
						memset(&messageToHmi[11+8],0,12);//positionAux

						int32_t phase = PHASE_HOMING;
						memcpy(&messageToHmi[23+8],&phase,4); //phase

						HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
					}
				
					motionCompletePause++;
					if (
						(motionCompletePause>2)&&
						(fhppInputData.spos & SPOS_MC)
					)
					{
						submode = SUBMODE_TEST_Pause;
						servoReadHoldingRegisters();
						return;
					}
					else
					{
						servoReadHoldingRegisters();
					}
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}
	

			break;
		//----submode----------------------------------------------------------
		case SUBMODE_TEST_Pause:
			
			servoEnable();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}
	
				timeToTest -=100;
				if (timeToTest>=0)
				{
					{
						uint8_t messageToHmi[31+8];
						messageToHmi[0] = mode;
						
						memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,6);

						uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

						uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));
						
						memcpy(
								&messageToHmi[7+8],
								&fhppInputData.actualValue2,
								4); //positionMain
						memset(&messageToHmi[11+8],0,12);//positionAux

						int32_t phase = PHASE_PAUSE;
						memcpy(&messageToHmi[23+8],&phase,4); //phase
						
						memcpy(
								&messageToHmi[27+8],
								&timeToTest,
								4
								); 
						HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
					}
					servoReadHoldingRegisters();			
							
				}
				else
				{
					
					
					fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
					fhppOutputData.cpos = CPOS_HALT+CPOS_START;
					if (testIsConcentric==true)
					{
						fhppOutputData.setpointValue1 = getSpeedRel(personalSettings.speedRelMainTestConcentric);
						fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainA);
					}
					else
					{
						fhppOutputData.setpointValue1 = getSpeedRel(personalSettings.speedRelMainTestEccentric);
						fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainB);
					}
					servoPresetMultipleRegisters(fhppOutputData);
					
					submode = SUBMODE_TEST_Running1;
				
				}
				
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}

				
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_TEST_Running1:
			
			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
					servoRxPointerProcessed = servoRxPointerReceived;
				
					{
						uint8_t messageToHmi[35+8];
						
						messageToHmi[0] = mode;
						memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

						uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

						uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

						memcpy(
								&messageToHmi[7+8],
								&fhppInputData.actualValue2,
								4); //position
						memset(&messageToHmi[11+8],0,12);//positionAux

						if (testIsConcentric==true){
							int32_t phase = PHASE_TEST_CONCENTRIC ;
							memcpy(&messageToHmi[23+8],&phase,4); //phase
						} else {
							int32_t phase = PHASE_TEST_ECCENTRIC ;
							memcpy(&messageToHmi[23+8],&phase,4); //phase
						}
						
						int32_t positionRel = 10000;
						memcpy(
								&messageToHmi[27+8],
								&positionRel,
								4
								);
						int32_t force = getStrainGaugeFilteredResult();
						memcpy(
								&messageToHmi[31+8],
								&force,
								4
								);
						HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
					}
				
					servoReadHoldingRegisters();
				
					motionCompletePause = 0;
					submode = SUBMODE_TEST_Running2;
				
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}



			break;
		//----submode----------------------------------------------------------
		case SUBMODE_TEST_Running2:
			
			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}

				{
						uint8_t messageToHmi[35+8];
						
						messageToHmi[0] = mode;
						memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

						uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

						uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

						memcpy(
								&messageToHmi[7+8],
								&fhppInputData.actualValue2,
								4); //position
						memset(&messageToHmi[11+8],0,12);//positionAux

						if (testIsConcentric==true){
							int32_t phase = PHASE_TEST_CONCENTRIC ;
							memcpy(&messageToHmi[23+8],&phase,4); //phase
						} else {
							int32_t phase = PHASE_TEST_ECCENTRIC ;
							memcpy(&messageToHmi[23+8],&phase,4); //phase
						}
						
						int32_t positionRel = getRelativePosition(fhppInputData.actualValue2);
						memcpy(
								&messageToHmi[27+8],
								&positionRel,
								4
								);
						int32_t force = getStrainGaugeFilteredResult();
						memcpy(
								&messageToHmi[31+8],
								&force,
								4
								);
						HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										



					
				}
				
				motionCompletePause++;
				if (
					(motionCompletePause>2)&&
					(fhppInputData.spos & SPOS_MC)
				)
				{
					fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
					fhppOutputData.cpos = CPOS_HALT;
					servoPresetMultipleRegisters(fhppOutputData);
					
					submode = SUBMODE_TEST_Running3;
				}
				else
				{
					servoReadHoldingRegisters();
				}				
				
				
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}

				
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_TEST_Running3:
			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;

				mode = MODE_WAITING;
				submode = SUBMODE_WAITING_ReceivingSettings;
				servoReadHoldingRegisters();

			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}

			break;
		//----submode----------------------------------------------------------
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_Start:
			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}

				{
					uint8_t messageToHmi[31+8];
					messageToHmi[0] = mode;
					memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

					uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

					uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

					memcpy(
							&messageToHmi[7+8],
							&fhppInputData.actualValue2,
							sizeof(fhppInputData.actualValue2)
							); //positionMain
					memset(&messageToHmi[11+8],0,12);//positionAux
					int32_t phase = PHASE_HOMING;
					memcpy(&messageToHmi[23+8],&phase,4); //phase
					memcpy(&messageToHmi[27+8],&isokineticSetIndex,sizeof(isokineticSetIndex));
					HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
				}


				fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
				fhppOutputData.cpos = CPOS_HALT+CPOS_START;
				fhppOutputData.setpointValue1 = getParkingSpeed();
				if (isokineticSetSettings[isokineticSetIndex].startPoint==0)
				{
					fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainA);
				}
				else
				{
					fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainB);
				}
				servoPresetMultipleRegisters(fhppOutputData);
				mode = MODE_EXCERCISE_ISOKINETIC;
				submode = SUBMODE_EXCERCISE_ISOKINETIC_Parking1;

			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_Parking1:

			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
				fhppOutputData.cpos = CPOS_HALT;
				servoPresetMultipleRegisters(fhppOutputData);
				mode = MODE_EXCERCISE_ISOKINETIC;
				submode = SUBMODE_EXCERCISE_ISOKINETIC_Parking2;
				
			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}


			break;
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_Parking2:

			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					servoReadHoldingRegisters();
					mode = MODE_EXCERCISE_ISOKINETIC;
					submode = SUBMODE_EXCERCISE_ISOKINETIC_Parking3;
					motionCompletePause = 0;
			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}


			break;
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_Parking3:

			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}

				{
					uint8_t messageToHmi[31+8];
					messageToHmi[0] = mode;
					memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

					uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

					uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

					memcpy(
							&messageToHmi[7+8],
							&fhppInputData.actualValue2,
							sizeof(fhppInputData.actualValue2)
							); //positionMain
					memset(&messageToHmi[11+8],0,12);//positionAux
					int32_t phase = PHASE_HOMING;
					memcpy(&messageToHmi[23+8],&phase,4); //phase
					memcpy(&messageToHmi[27+8],&isokineticSetIndex,sizeof(isokineticSetIndex));
					HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
				}

				if (fhppInputData.spos & SPOS_MC)
				{
					motionCompletePause++;
					if (motionCompletePause==3)
					{
						mode = MODE_EXCERCISE_ISOKINETIC;
						submode = SUBMODE_EXCERCISE_ISOKINETIC_Pause;
						timeToSet = isokineticSetSettings[isokineticSetIndex].pauseTime;
					}
				}
				else
				{
					mode = MODE_EXCERCISE_ISOKINETIC;
					submode = SUBMODE_EXCERCISE_ISOKINETIC_Parking3;
					motionCompletePause = 0;
				}



				servoReadHoldingRegisters();
			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}

			break;

		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_Pause:

			servoEnable();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}
	
				timeToSet -=100;
				if (timeToSet>=0)
				{
					{
						uint8_t messageToHmi[35+8];
						messageToHmi[0] = mode;
						memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

						uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

						uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

						memcpy(
								&messageToHmi[7+8],
								&fhppInputData.actualValue2,
								sizeof(fhppInputData.actualValue2)
								); //positionMain
						memset(&messageToHmi[11+8],0,12);//PositionAux
						int32_t phase = PHASE_PAUSE;
						memcpy(&messageToHmi[23+8],&phase,4); //phase
						memcpy(&messageToHmi[27+8],&isokineticSetIndex,sizeof(isokineticSetIndex));
						memcpy(&messageToHmi[31+8],&timeToSet,sizeof(timeToSet));
						HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
					}


				}
				else
				{
					
					submode = SUBMODE_EXCERCISE_ISOKINETIC_PrepareFirstMovement;
					repsIndex = 0;
				}

				servoReadHoldingRegisters();			

			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}



			break;

		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_PrepareFirstMovement:


			servoEnable();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}

				fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
				fhppOutputData.cpos = CPOS_HALT+CPOS_START;
				if (isokineticSetSettings[isokineticSetIndex].startPoint==0)
				{
					fhppOutputData.setpointValue1 = getSpeedRel(isokineticSetSettings[isokineticSetIndex].speedAB);
					fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainB);
				}
				else
				{
					fhppOutputData.setpointValue1 = getSpeedRel(isokineticSetSettings[isokineticSetIndex].speedBA);
					fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainA);
				}
				servoPresetMultipleRegisters(fhppOutputData);
				submode = SUBMODE_EXCERCISE_ISOKINETIC_StartFirstMovement1;

			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}


			break;
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_StartFirstMovement1:

			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;

				fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
				fhppOutputData.cpos = CPOS_HALT;
				servoPresetMultipleRegisters(fhppOutputData);

				submode = SUBMODE_EXCERCISE_ISOKINETIC_StartFirstMovement2;
				motionCompletePause = 0;

			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}
			break;

		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_StartFirstMovement2:

			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;

				servoReadHoldingRegisters();

				submode = SUBMODE_EXCERCISE_ISOKINETIC_KeepFirstMovement;
				motionCompletePause = 0;

			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}


			break;
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_KeepFirstMovement:

			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}

				{
					//TAG_ReportCurrentMode
					uint8_t messageToHmi[47+8];
					messageToHmi[0] = mode;
					memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

					uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

					uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

					memcpy(
							&messageToHmi[7+8],
							&fhppInputData.actualValue2,
							sizeof(fhppInputData.actualValue2)
							); //position
					memset(&messageToHmi[11+8],0,12);//positionAux
					int32_t phase = PHASE_ISOKINETIC_FIRSTMOVE;
					memcpy(&messageToHmi[23+8],&phase,4); //phase
					memcpy(&messageToHmi[27+8],&isokineticSetIndex,sizeof(isokineticSetIndex));
					memcpy(&messageToHmi[31+8],&repsIndex,sizeof(repsIndex));
					int32_t repDirection = isokineticSetSettings[isokineticSetIndex].startPoint;
					memcpy(&messageToHmi[35+8],&repDirection,sizeof(repDirection));
					int32_t positionRel = getRelativePosition(fhppInputData.actualValue2);
					memcpy(&messageToHmi[39+8],&positionRel,sizeof(positionRel));
					int32_t force = getStrainGaugeFilteredResult();
					memcpy(&messageToHmi[43+8],&force,sizeof(force));

					HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
				}


				motionCompletePause++;

				if (
					(motionCompletePause>2)&&
					(fhppInputData.spos & SPOS_MC)
				)
				{
					timeFirstInterruption = isokineticSetSettings[isokineticSetIndex].firstInterruptionTime;
					if (timeFirstInterruption>0){
						
						servoReadHoldingRegisters();
						submode = SUBMODE_EXCERCISE_ISOKINETIC_FirstInterruption;
						
						
					} else {

						fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
						fhppOutputData.cpos = CPOS_HALT+CPOS_START;
						if (isokineticSetSettings[isokineticSetIndex].startPoint==0)
						{
							fhppOutputData.setpointValue1 = getSpeedRel(isokineticSetSettings[isokineticSetIndex].speedBA);
							fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainA);
						}
						else
						{
							fhppOutputData.setpointValue1 = getSpeedRel(isokineticSetSettings[isokineticSetIndex].speedAB);
							fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainB);
						}
						servoPresetMultipleRegisters(fhppOutputData);
						submode = SUBMODE_EXCERCISE_ISOKINETIC_StartSecondMovement1;
						
					}
					OdometerRTC::addMovement(personalSettings.positionMainA,personalSettings.positionMainB);

					
					
				}
				else
				{
					servoReadHoldingRegisters();
				}				



			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}



			break;
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_FirstInterruption:
			
			servoEnable();
			RxMessageQueue::flush();
		
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}

				timeFirstInterruption -=100;
				if (timeFirstInterruption>=0)
				{
					{
						uint8_t messageToHmi[35+8];
						messageToHmi[0] = mode;
						memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

						uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

						uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));
						memcpy(
								&messageToHmi[7+8],
								&fhppInputData.actualValue2,
								sizeof(fhppInputData.actualValue2)
								); //positionMain
						memset(&messageToHmi[11+8],0,12);//PositionAux
						int32_t phase = PHASE_ISOKINETIC_FIRSTINTERRUPTION;
						memcpy(&messageToHmi[23+8],&phase,sizeof(phase)); //phase
						memcpy(&messageToHmi[27+8],&isokineticSetIndex,sizeof(isokineticSetIndex));
						memcpy(&messageToHmi[31+8],&timeFirstInterruption,sizeof(timeFirstInterruption));
						HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
					}
					
					servoReadHoldingRegisters();
					

				}
				else
				{
					
					fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
					fhppOutputData.cpos = CPOS_HALT+CPOS_START;
					if (isokineticSetSettings[isokineticSetIndex].startPoint==0)
					{
						fhppOutputData.setpointValue1 = getSpeedRel(isokineticSetSettings[isokineticSetIndex].speedBA);
						fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainA);
					}
					else
					{
						fhppOutputData.setpointValue1 = getSpeedRel(isokineticSetSettings[isokineticSetIndex].speedAB);
						fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainB);
					}
					servoPresetMultipleRegisters(fhppOutputData);
					submode = SUBMODE_EXCERCISE_ISOKINETIC_StartSecondMovement1;
						
				}

			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}

		
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_StartSecondMovement1:
			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;

				fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
				fhppOutputData.cpos = CPOS_HALT;
				servoPresetMultipleRegisters(fhppOutputData);

				submode = SUBMODE_EXCERCISE_ISOKINETIC_StartSecondMovement2;
				motionCompletePause = 0;

			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}

			break;
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_StartSecondMovement2:

			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				servoRxPointerProcessed = servoRxPointerReceived;

				servoReadHoldingRegisters();

				submode = SUBMODE_EXCERCISE_ISOKINETIC_KeepSecondMovement;
				motionCompletePause = 0;

			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}
		
			break;
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_KeepSecondMovement:

			servoEnable();
			RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}

				{
					uint8_t messageToHmi[47+8];
					messageToHmi[0] = mode;
					memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

					uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

					uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

					memcpy(
							&messageToHmi[7+8],
							&fhppInputData.actualValue2,
							sizeof(fhppInputData.actualValue2)
							); //position
					memset(&messageToHmi[11+8],0,12);//positionAux
					int32_t phase = PHASE_ISOKINETIC_SECONDMOVE;
					memcpy(&messageToHmi[23+8],&phase,4); //phase
					memcpy(&messageToHmi[27+8],&isokineticSetIndex,sizeof(isokineticSetIndex));
					memcpy(&messageToHmi[31+8],&repsIndex,sizeof(repsIndex));
					int32_t repDirection = isokineticSetSettings[isokineticSetIndex].startPoint ^1;
					memcpy(&messageToHmi[35+8],&repDirection,sizeof(repDirection));
					int32_t positionRel = getRelativePosition(fhppInputData.actualValue2);
					memcpy(&messageToHmi[39+8],&positionRel,sizeof(positionRel));
					int32_t force = getStrainGaugeFilteredResult();
					memcpy(&messageToHmi[43+8],&force,sizeof(force));

					HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
				}

				motionCompletePause++;

				if (
					(motionCompletePause>2)&&
					(fhppInputData.spos & SPOS_MC)
				)
				{
					timeSecondInterruption = isokineticSetSettings[isokineticSetIndex].secondInterruptionTime;
					
					if (timeSecondInterruption>0){
						
						servoReadHoldingRegisters();
						submode = SUBMODE_EXCERCISE_ISOKINETIC_SecondInterruption;
						
						
					} else {
					
						repsIndex++;
						if (repsIndex!=isokineticSetSettings[isokineticSetIndex].repsCount)
						{
							fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
							fhppOutputData.cpos = CPOS_HALT+CPOS_START;
							if (isokineticSetSettings[isokineticSetIndex].startPoint==0)
							{
								fhppOutputData.setpointValue1 = getSpeedRel(isokineticSetSettings[isokineticSetIndex].speedAB);
								fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainB);
							}
							else
							{
								fhppOutputData.setpointValue1 = getSpeedRel(isokineticSetSettings[isokineticSetIndex].speedBA);
								fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainA);
							}
							servoPresetMultipleRegisters(fhppOutputData);
							submode = SUBMODE_EXCERCISE_ISOKINETIC_StartFirstMovement1;

						}
						else
						{
							isokineticSetIndex++;
							if (isokineticSetIndex!=isokineticSetCount)
							{
								submode = SUBMODE_EXCERCISE_ISOKINETIC_Start;
								servoReadHoldingRegisters();
							}
							else
							{
								mode = MODE_WAITING;
								submode = SUBMODE_WAITING_ReceivingSettings;
								servoReadHoldingRegisters();
							}

						}
						
					}
					OdometerRTC::addMovement(personalSettings.positionMainA,personalSettings.positionMainB);



				}
				else
				{
					servoReadHoldingRegisters();
				}				


			}
			else
			{
					servoRxPointerProcessed = servoRxPointerReceived;
					setErrorMode(ERROR_ServoConnectionLost);
			}

			break;
		//----submode----------------------------------------------------------
		case SUBMODE_EXCERCISE_ISOKINETIC_SecondInterruption:
			
			servoEnable();
			RxMessageQueue::flush();
		
			if (servoResponseLength==MODBUS_READ_RESPONSE_LENGTH)
			{
				servoPopBytes(modbusRxMessage,MODBUS_READ_RESPONSE_LENGTH);
				servoGetFhppInputData(&fhppInputData,&modbusRxMessage[0]);								
				if (false==servoCheckFhppInputData(&fhppInputData))
				{
					setErrorMode(ERROR_ServoInternal);
					return;
				}

				timeSecondInterruption -=100;
				if (timeSecondInterruption>=0)
				{
					{
						uint8_t messageToHmi[35+8];
						messageToHmi[0] = mode;
						memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

						uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

						uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
						memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));
		
						memcpy(
								&messageToHmi[7+8],
								&fhppInputData.actualValue2,
								sizeof(fhppInputData.actualValue2)
								); //positionMain
						memset(&messageToHmi[11+8],0,12);//PositionAux
						int32_t phase = PHASE_ISOKINETIC_SECONDINTERRUPTION;
						memcpy(&messageToHmi[23+8],&phase,sizeof(phase)); //phase
						memcpy(&messageToHmi[27+8],&isokineticSetIndex,sizeof(isokineticSetIndex));
						memcpy(&messageToHmi[31+8],&timeFirstInterruption,sizeof(timeFirstInterruption));
						HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
					}
					
					servoReadHoldingRegisters();
					

				}
				else
				{
					
					repsIndex++;
					if (repsIndex!=isokineticSetSettings[isokineticSetIndex].repsCount)
					{
						fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
						fhppOutputData.cpos = CPOS_HALT+CPOS_START;
						if (isokineticSetSettings[isokineticSetIndex].startPoint==0)
						{
							fhppOutputData.setpointValue1 = getSpeedRel(isokineticSetSettings[isokineticSetIndex].speedAB);
							fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainB);
						}
						else
						{
							fhppOutputData.setpointValue1 = getSpeedRel(isokineticSetSettings[isokineticSetIndex].speedBA);
							fhppOutputData.setpointValue2 = int32Reversed(personalSettings.positionMainA);
						}
						servoPresetMultipleRegisters(fhppOutputData);
						submode = SUBMODE_EXCERCISE_ISOKINETIC_StartFirstMovement1;

					}
					else
					{
						isokineticSetIndex++;
						if (isokineticSetIndex!=isokineticSetCount)
						{
							submode = SUBMODE_EXCERCISE_ISOKINETIC_Start;
							servoReadHoldingRegisters();
						}
						else
						{
							mode = MODE_WAITING;
							submode = SUBMODE_WAITING_ReceivingSettings;
							servoReadHoldingRegisters();
						}

					}



					
				}

			}
			else
			{
				servoRxPointerProcessed = servoRxPointerReceived;
				setErrorMode(ERROR_ServoConnectionLost);
			}
		
			break;
		//----submode----------------------------------------------------------
		//----submode----------------------------------------------------------
		case SUBMODE_CANCEL_1:

			servoEnable();
			//RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				
				flushServoRx();;
				fhppOutputData.ccon = CCON_OPM_DIRECT + CCON_LOCK + CCON_ENABLE + CCON_STOP;
				fhppOutputData.cpos = CPOS_HALT;
				servoPresetMultipleRegisters(fhppOutputData);
				submode = SUBMODE_CANCEL_2;
				
				
				

			}
			else
			{
				flushServoRx();
				setErrorMode(ERROR_ServoConnectionLost);
			}
			break;
		case SUBMODE_CANCEL_2:
			servoEnable();
			//RxMessageQueue::flush();
			if (servoResponseLength==MODBUS_WRITE_RESPONSE_LENGTH)
			{
				flushServoRx();;
				servoReadHoldingRegisters();
				mode = MODE_WAITING;
				submode = SUBMODE_WAITING_ReceivingSettings;

			}
			else
			{
				flushServoRx();
				setErrorMode(ERROR_ServoConnectionLost);
			}
			break;
		//----submode----------------------------------------------------------
		//----submode----------------------------------------------------------
		case SUBMODE_ERROR:

			servoDisable();
		
			switch(errorType)
			{

				default:
					uint8_t messageToHmi[1+sizeof(TCurrentDateTime)+8+1];
					messageToHmi[0] = mode;
					memcpy(&messageToHmi[1],&OdometerRTC::currentDateTime,sizeof(TCurrentDateTime));

					uint32_t odometerTimeMinutes = OdometerRTC::getOdometerTimeMinutes();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)],&odometerTimeMinutes,sizeof(odometerTimeMinutes));

					uint32_t odometerDistanceMeters = OdometerRTC::getOdometerDistanceMeters();
					memcpy(&messageToHmi[1+sizeof(TCurrentDateTime)+sizeof(uint32_t)],&odometerDistanceMeters,sizeof(odometerDistanceMeters));

					messageToHmi[1+sizeof(TCurrentDateTime)+8] = errorType;
					HMI::protocol.sendPacket(Protocol::TAG_ReportCurrentMode,messageToHmi,sizeof(messageToHmi));										
					break;
				
			}
			
			
			RxMessage *message = RxMessageQueue::pop();
			if (message!=NULL){
				
				if (message->tag==Protocol::TAG_ResetError)
				{
					modeInit();
					return;
				}
			}
			

		
			break;
		//----mode----------------------------------------------------------
	}
	
}
//=================================================================================================================
void setErrorMode(ErrorType type)
{
	servoDisable();
	mode = MODE_ERROR;
	submode = SUBMODE_ERROR;
	errorType = type;
}
//=================================================================================================================
uint8_t convertSpeedHmiToServo(uint32_t hmiSpeed)
{
		uint8_t servoSpeed;
		if (hmiSpeed>100)
		{
			servoSpeed=100;
		}
		else
		{
			servoSpeed=(uint8_t)hmiSpeed;
		}
		return servoSpeed;
}
uint32_t convertSpeedServoToHmi(uint8_t servoSpeed)
{
		return (uint32_t)servoSpeed;
}

//=================================================================================================================
int32_t getRelativePosition(int32_t absolutePosition)
{
	int32_t temp = (absolutePosition-personalSettings.positionMainA)*10000;
	return temp/(personalSettings.positionMainB-personalSettings.positionMainA);
}
//=================================================================================================================
uint8_t getPersonalSpeed(void){

	int32_t personalSpeed = machineSettings.speedAbsMainMax * 100;
	personalSpeed /= machineSettings.speedAbsMainPersonal;
	if (personalSpeed<1){
		personalSpeed = 1;
	} else if (personalSpeed>100){
		personalSpeed = 100;
	}
	return (uint8_t)personalSpeed;
	
}
//=================================================================================================================
uint8_t getParkingSpeed(void){
	
	int32_t parkingSpeed = machineSettings.speedAbsMainMax * 100;
	parkingSpeed /= personalSettings.speedAbsMainParking;
	if (parkingSpeed<1){
		parkingSpeed = 1;
	} else if (parkingSpeed>100){
		parkingSpeed = 100;
	}
	return (uint8_t)parkingSpeed;
	
}
//=================================================================================================================
uint8_t latestSpeedRel;
uint8_t getSpeedRel(int32_t speedRel){
	
	double dSpeedRel = personalSettingsEx.speedRelToAbsConversionCoeff / (double)speedRel;
	if (dSpeedRel<1.0){
		dSpeedRel = 1;
	} else if (dSpeedRel>100.0){
		dSpeedRel = 100.0;
	}
	latestSpeedRel = (uint8_t)(dSpeedRel+0.5);
	return latestSpeedRel;//(uint8_t)(dSpeedRel+0.5);
	
}
//=================================================================================================================

