
#include "GenericSetSettings.h"

#include <string.h>

#include "hmi/hmi.h"
#include "hmi/diagnostics.h"



GenericSetSettings::Set GenericSetSettings::set;	
bool GenericSetSettings::valid;

//=================================================================================================
void GenericSetSettings::init(){

	memset(&set,0,sizeof(set));
	valid = false;

}
//=================================================================================================

