/****************************************************************************************
 *
 * File:
 * 		XbeeRemote.h
 *
 * Purpose:
 *		
 *
 * Developer Notes:
 *
 ***************************************************************************************/


#pragma once


#include "Network/XbeePacketNetwork.h"
#include "Network/DataLink.h"


class XbeeRemote {
public:
	XbeeRemote();
	~XbeeRemote();

	///----------------------------------------------------------------------------------
	/// Initialises the Xbee remote data link and network
	///----------------------------------------------------------------------------------
	bool initialise();

	///----------------------------------------------------------------------------------
	/// Runs the Xbee packet network, this function only returns when something has gone
	/// wrong.
	///----------------------------------------------------------------------------------
	void run();

	///----------------------------------------------------------------------------------
	/// Called when a message is received over the Xbee network
	///----------------------------------------------------------------------------------
	static void incomingData(uint8_t* data, uint8_t size)

private:
	DataLink*			m_DataLink;
	XbeePacketNetwork* 	m_Network;
	bool 				m_Connected;
};
