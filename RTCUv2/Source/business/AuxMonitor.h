
#ifndef AUX_MONITOR_H
#define AUX_MONITOR_H

#include <stdint.h>

class AuxMonitorData{
public:
	int32_t eccentricEnable;
	int32_t eccentricCurve[21];
	int32_t eccentricWindow;
	int32_t concentricEnable;
	int32_t concentricCurve[21];
	int32_t concentricWindow;
};

class AuxMonitor{
private:
	static uint16_t auxMonitorDataLen;
	static AuxMonitorData auxMonitorData;

public:

	static void setDataFromRxMessage(uint16_t messageDataLen, uint8_t *messageData);

};


#endif
