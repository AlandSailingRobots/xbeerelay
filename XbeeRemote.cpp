/****************************************************************************************
 *
 * File:
 * 		XbeeRemote.cpp
 *
 * Purpose:
 *		
 *
 * Developer Notes:
 *
 ***************************************************************************************/


#include "XbeeRemote.h"

#ifdef linux
#include "Network/LinuxSerialDataLink.h"
#elif _WIN32
#include "Network/WindowsSerialDataLink.h"
#endif

/***************************************************************************************/
XbeeRemote::XbeeRemote()
	:m_DataLink(NULL), m_Network(NULL), m_Connected(false)
{

}

/***************************************************************************************/
XbeeRemote::~XbeeRemote()
{

}

/***************************************************************************************/
bool XbeeRemote::initialise()
{
#ifdef linux

	m_DataLink = new LinuxSerialDataLink("/dev/ttyUSB0", XBEE_BAUD_RATE);

#elif _WIN32

#endif

	if(m_DataLink->initialise(XBEE_PACKET_SIZE))
	{
		m_Network = new XbeePacketNetwork(*m_DataLink, true);
		m_Network->setIncomingCallback(incomingData);
		return true;
	}
	else
	{
		delete m_DataLink;
		m_DataLink = NULL;
		return false;
	}

}

/***************************************************************************************/
void XbeeRemote::run()
{
	if(m_Network != NULL)
	{
		while(true)
		{
			m_Network->processRadioMessages();
		}
	}
}

/***************************************************************************************/
 void XbeeRemote::incomingData(uint8_t* data, uint8_t size)
{

}
