
#include "RxMessageQueue.h"

#include <string.h>

uint32_t RxMessageQueue::ptrReceived;
uint32_t RxMessageQueue::ptrProcessed;
RxMessage RxMessageQueue::messages[RxMessageQueue::MAX_QUEUE_LEN];


//=================================================================================================
void RxMessageQueue::init(){

	ptrReceived = ptrProcessed = 0;
	




}
//=================================================================================================
uint32_t RxMessageQueue::getMessageCount(){
	return (ptrReceived-ptrProcessed) % MAX_QUEUE_LEN;
}
bool RxMessageQueue::isEmpty(){
	return (ptrReceived==ptrProcessed);
}
bool RxMessageQueue::isFull(){
	return (getMessageCount()==(MAX_QUEUE_LEN-1));
}
void RxMessageQueue::flush(){
	ptrProcessed = ptrReceived;
}

//=================================================================================================
void RxMessageQueue::push(
	uint8_t tag,
	uint16_t valueLen,
	uint32_t id,
	bool fromHMI,
	uint8_t* value
){

	if (isFull()==false){

		RxMessage *message = &messages[ptrReceived];

		message->tag = tag;
		message->valueLen = valueLen;
		message->id = id;
		message->fromHMI = fromHMI;
		memcpy(message->value,value,valueLen);

		ptrReceived = (ptrReceived+1) % MAX_QUEUE_LEN;
		
	}

}
//=================================================================================================
RxMessage* RxMessageQueue::pop(){

	if (RxMessageQueue::isEmpty()){
		return NULL;
	}else{
		uint32_t ptrProcessedToPop = ptrProcessed;
		ptrProcessed = (ptrProcessed+1) % MAX_QUEUE_LEN;
		return &messages[ptrProcessedToPop];
	}
}
//=================================================================================================
bool RxMessageQueue::cancelMessageReceived(){

	while(true){
		RxMessage *message = RxMessageQueue::pop();
		if (message!=NULL){
			if(message->tag==Protocol::TAG_Cancel){
				return true;	
			}
		}else{
			break;
		}
	}	
	return false;

}
//=================================================================================================
bool RxMessageQueue::resetErrorMessageReceived(){

	while(true){
		RxMessage *message = RxMessageQueue::pop();
		if (message!=NULL){
			if(message->tag==Protocol::TAG_ResetError){
				return true;	
			}
		}else{
			break;
		}
	}	
	return false;	
}
//=================================================================================================









