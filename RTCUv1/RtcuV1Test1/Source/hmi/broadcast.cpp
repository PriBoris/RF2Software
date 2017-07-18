

#include "broadcast.h"

#include "hmi/hmi.h"
#include "hmi/diagnostics.h"


void Broadcast::sendPacket(uint8_t tag,uint8_t *data,uint32_t dataLen){


	HMI::protocol.sendPacket(tag, data, dataLen);										
	Diagnostics::protocol.sendPacket(tag, data, dataLen);										



}

