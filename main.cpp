//#include "xBeeRelay.h"
#include "../xBee/Xbee.h"
#include "Network/XbeePacketNetwork.h"
#include "Network/LinuxSerialDataLink.h"
#include <stdio.h>
#include "../SystemServices/Logger.h"
#include "../utility/SysClock.h"
#include "../Messages/VesselStateMsg.h"
#include "../Messages/DataRequestMsg.h"
#include "../Messages/WindDataMsg.h"
#include "../Messages/CompassDataMsg.h"
#include "../Messages/GPSDataMsg.h"
#include "../Messages/ActuatorPositionMsg.h"
#include "../Messages/WaypointDataMsg.h"
#include "../Messages/CourseDataMsg.h"
#include "udpclient.h"


unsigned long lastMessage = 0;

void printMessage(Message* msgPtr, MessageDeserialiser& deserialiser)
{
	lastMessage = SysClock::unixTime();

	switch(msgPtr->messageType())
	{
		case MessageType::DataRequest:
		{
			DataRequestMsg msg(deserialiser);

			if(msg.isValid())
			{
				Logger::info("Message Type %s - Source: %d Destination: %d", msgToString(MessageType::DataRequest).c_str(), msg.sourceID(), msg.destinationID());
			}
		}
		break;
		case MessageType::WindData:
		{
			WindDataMsg msg(deserialiser);

			if(msg.isValid())
			{
				Logger::info("Message: %s - Wind Dir: %f Wind Temp: %f Wind Speed: %f", msgToString(MessageType::WindData).c_str(), msg.windDirection(), msg.windTemp(), msg.windSpeed());
			}
		}
		break;
		case MessageType::CompassData:
		{
			CompassDataMsg msg(deserialiser);

			if(msg.isValid())
			{
				Logger::info("Message:  %s - Heading: %d Pitch: %d Roll: %d", msgToString(MessageType::CompassData).c_str(), msg.heading(), msg.pitch(), msg.roll());
			}
		}
		break;
		case MessageType::GPSData:
		{
			GPSDataMsg msg(deserialiser);

			if(msg.isValid())
			{
				Logger::info("Message: %s - Unix Time: %d Lat: %.7lf Lon: %.7lf", msgToString(MessageType::GPSData).c_str(), msg.unixTime(), msg.latitude(), msg.longitude());
			}
		}
		break;
		case MessageType::ActuatorPosition:
		{
			ActuatorPositionMsg msg(deserialiser);

			if(msg.isValid())
			{
				Logger::info("Message: %s - Rudder: %d Sail: %d", msgToString(MessageType::ActuatorPosition).c_str(), msg.rudderPosition(), msg.sailPosition());
			}
		}
		break;
		case MessageType::VesselState:
		{
			VesselStateMsg msg(deserialiser);

			if(msg.isValid())
			{
				unsigned int unixTime = (unsigned)(int)msg.unixTime();
				Logger::info("Message: %s - Unix Time: %u Lat: %.7f Long: %.7f Compass: %d Speed: %f Wind Dir: %f Wind Speed: %f", msgToString(MessageType::VesselState).c_str(), unixTime, msg.latitude(), msg.longitude(), msg.compassHeading(), msg.speed(), msg.windDir(), msg.windSpeed());
				udpwrite("heading=%d speed=%f lat=%.7f lon=%.7f", msg.compassHeading(), msg.speed(), msg.latitude(), msg.longitude());
			}
			else
			{
				Logger::info("Not valid");
			}
		}
		break;
		case MessageType::WaypointData:
		{
			WaypointDataMsg msg(deserialiser);

			if(msg.isValid())
			{
				Logger::info("Message: %s - ID: %d Lat: %.7lf Lon: .7lf", msgToString(MessageType::WaypointData).c_str(), msg.nextId(), msg.nextLatitude(), msg.nextLongitude());
				udpwrite("wpnum=%d ", msg.nextId());
			}
		}
		break;
		case MessageType::CourseData:
		{
			CourseDataMsg msg(deserialiser);

			if(msg.isValid())
			{
				Logger::info("Message: %s - True Wind Dir: %f Distance To WP: %f Course To WP: %f", msgToString(MessageType::CourseData).c_str(), msg.trueWindDir(), msg.distanceToWP(), msg.courseToWP());
				udpwrite("twd=%f dtw=%f ctw=%f", msg.trueWindDir(), msg.distanceToWP(), msg.courseToWP());
			}
		}
		break;
		default:
			Logger::info("Packet: %s", msgToString(msgPtr->messageType()).c_str());
			return;
	}
}

void incoming(uint8_t* data, uint8_t size)
{
	MessageDeserialiser deserialiser(data, size);

	Message msg(deserialiser);

	if(msg.isValid())
	{
		deserialiser.resetInternalPtr();
		printMessage(&msg, deserialiser);
	}
}

int main() {
	const int OFFLINE_TIME = 5;
	bool offline = true;
	//xBeeRelay relay;
	std::string data = "";

	LinuxSerialDataLink serialDataLink("/dev/ttyUSB0", XBEE_BAUD_RATE);
	serialDataLink.initialise(XBEE_PACKET_SIZE);

	XbeePacketNetwork packetNetwork(serialDataLink, true);
	packetNetwork.setIncomingCallback(incoming);

	lastMessage = SysClock::unixTime();

	while (true) { 

		packetNetwork.processRadioMessages();

		if(SysClock::unixTime() - lastMessage > OFFLINE_TIME)
		{
			if(not offline)
			{
				offline = true;
				udpwrite("offline=1");
			}

		}
		else if(offline)
		{
			udpwrite("offline=0");
			offline = false;
		}
	}

}
