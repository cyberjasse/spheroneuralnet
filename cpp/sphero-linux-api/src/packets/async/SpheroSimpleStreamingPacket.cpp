/*************************************************************************
   SpheroSimpleStreamingPacket  - Represents an asynchronous collision packet
							 -------------------
	début                : jeu. 07 mai 2015
*************************************************************************/

//-------------------------------------------------------- System includes
//#ifdef MAP
	#include <iostream>
//#endif

#include <vector>
#include <sys/socket.h>
#include <endian.h>
#include "chrono.hpp"

using namespace std;

//--------------------------------------------------------- Local includes
#include "SpheroSimpleStreamingPacket.hpp"
#include "../../Sphero.hpp"
#include "../Toolbox.hpp"
#include "../Constants.hpp"
#include "DataBuffer.h"

#include "../ChecksumComputer.hpp"
#include "StreamFrame.hpp"

//------------------------------------------------ Constants
static size_t const PACKET_SIZE = 13;

//------------------------------------------------ Constructors/Destructor

/**
 * @brief SpheroSimpleStreamingPacket : Constructor
 * @param sphero : The Sphero instance that sends the collision packet
 */
SpheroSimpleStreamingPacket::SpheroSimpleStreamingPacket(
		Sphero* sphero,
		int16_t x,
		int16_t y,
		int16_t speedX,
		int16_t speedY
		):	SpheroAsyncPacket(sphero), _x(x), _y(y), _speedX(speedX), _speedY(speedY){
}


SpheroSimpleStreamingPacket::~SpheroSimpleStreamingPacket ( )
{
#ifdef MAP
	std::cout << "<SpheroSimpleStreamingPacket> destructor called" << std::endl;
#endif
}


//--------------------------------------------------------- Public methods

/**
 * @brief extractPacket : extracts the packet from a socket
 * @param fd : The socket file descriptor
 * @param sphero : The Sphero sending the packet
 * @param packet_ptr : A pointer to a SpheroPacket pointer
 * @return true if the packet was successfully extracted from the socket, false otherwise
 *
 * Contract: the socket has to be in blocking read
 */
bool SpheroSimpleStreamingPacket::extractPacket(int fd,  Sphero* sphero, SpheroPacket**)
{
	const int PS = 23;
	uint8_t rawdata[PS];
	uint8_t checksum;
	int16_t x,y,speedX,speedY,yaw;
	int size = recv(fd, &rawdata, sizeof(uint8_t)*PS , 0);
	/*Should return FFh FEH  DLENexpanded id(8bits) data CHK(8bits)
	  DLEN = size of Data + size of CHK
	  CHK  = checksum. equal to the sum of the underlined bytes modulo 256 and then bit inverted
	         underlined bytes is DLEN + Data
	  So should return FFH FEH DLENexpanded 3 yaw(16bits) X(16bits) Y(16bits) vX(16bits) vY(16bits)*/
	if(rawdata[0] != 3){
		cerr << "[SpheroSimpleStreamingPacket] rawdata[0]!=3 : not an async packet " << endl;
	}
	else{
		uint16_t dlen = get2bytesfromTable(1, rawdata);
		if(dlen != 11){
			cerr << "[SpheroSimpleStreamingPacket] dlen!=11 : not enough data received " << endl;
		}
		else if(dlen != size-3){
			cerr << "[SpheroSimpleStreamingPacket] dlen!=size-3 : incorrect packet size " << endl;
		}
		// check checksum
		else{
			yaw 	= get2bytesfromTable(3, rawdata);
			x   	= get2bytesfromTable(5, rawdata);
			y 		= get2bytesfromTable(7, rawdata);
			speedX 	= get2bytesfromTable(9, rawdata);
			speedY 	= get2bytesfromTable(11, rawdata);
			static chrono c = chrono();
			StreamFrame frame = {yaw, x, y, speedX, speedY, c.top()};
			sphero->notifyStream(&frame);
		}
	}
	return false;
}

int16_t SpheroSimpleStreamingPacket::get2bytesfromTable(int index, uint8_t* table){
	return (table[index]<<8) + table [index+1];
}


/**
 * @brief packetAction : Performs the action associated to the packet
 *			on the Sphero instance
 */
void SpheroSimpleStreamingPacket::packetAction()
{
	std::cout << "SSSpacketAction\n";
	_sphero->setX(_x);
	_sphero->setY(_y);
	_sphero->setSpeedX(_speedX);
	_sphero->setSpeedY(_speedY);	
}

