
#ifndef HMI_BROADCAST_H
#define HMI_BROADCAST_H


#include <stdint.h>

class Broadcast{
private:


public:

	static void sendPacket(uint8_t tag,uint8_t *data,uint32_t dataLen);


};




#endif //HMI_BROADCAST_H
