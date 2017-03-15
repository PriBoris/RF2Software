

#include "AuxMonitor.h"

#include <string.h>

uint16_t AuxMonitor::auxMonitorDataLen = 0;
AuxMonitorData AuxMonitor::auxMonitorData;

void AuxMonitor::setDataFromRxMessage(uint16_t messageDataLen, uint8_t *messageData){

	auxMonitorDataLen = messageDataLen;
	if (messageDataLen == sizeof(AuxMonitorData)){
		memcpy(&auxMonitorData, messageData, sizeof(auxMonitorData));
	} 



}
