#ifndef __XBEERELAY__
#define __XBEERELAY__

#include "xBee/xBee.h"
#include <curl/curl.h>

class xBeeRelay {

	public:
		xBeeRelay();

		~xBeeRelay();

		size_t static write_to_string(void *ptr, size_t size, size_t count, void *stream);

		void relayData(std::string data);

		std::string receiveData();

		std::string extractMessage(std::string data);

		void pushLogsToServer(std::string data); 

	private:
		xBee m_xBee;
		int m_fd;
		CURL* m_curl; 
	};

#endif