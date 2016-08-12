//#include "xBeeRelay.h"

#include <stdio.h>
#include "../SystemServices/Logger.h"


#include "XbeeRemote.h"




int main() {
	std::string portName = "";
	#ifdef _WIN32
		portName = "COM3";
	#elif __linux__
		portName = "/dev/ttyUSB0";
	#endif


	XbeeRemote xbeeRemote(portName);

	if(not xbeeRemote.initialise())
	{
		Logger::error("Shutting down, failed to initialise");
		return 1;
	}


	while(true)
	{
		xbeeRemote.run();
	}
}
