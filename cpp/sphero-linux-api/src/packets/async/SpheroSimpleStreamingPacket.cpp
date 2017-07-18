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
	uint8_t rawdata[PACKET_SIZE+1];
	uint8_t checksum;
	int16_t x,y,speedX,speedY,yaw;
	const int PS = 23;
	const uint8_t z = 0;
	for(int i=0 ; i<PS ; i++) rawdata[i]=z;
	int size = recv(fd, &rawdata, sizeof(uint8_t)*PACKET_SIZE+10 , 0);
	/*Should return FFh FEH  DLENexpanded id(8bits) data CHK(8bits)
	  DLEN = size of Data + size of CHK
	  CHK  = checksum. equal to the sum of the underlined bytes modulo 256 and then bit inverted
	         underlined bytes is DLEN + Data
	  So should return FFH FEH DLENexpanded 3 yaw(16bits) X(16bits) Y(16bits) vX(16bits) vY(16bits)
	  
	if(size < 0)
	{
		cout<<"ha " << "retour=" << size <<endl;
		return false;	
	}
	cout << "size=" << size << " [";
	for(int i=0 ; i<size ; i++) cout << (int)rawdata[i] << ", ";
	cout << "]" << endl;
cout<<"tic-";
	ChecksumComputer cc;
#ifdef MAP
	std::cerr << "Creation d'un simplestreapacket" << std::endl;
#endif

	if(recv(fd, &buf, sizeof(buf), 0) != sizeof(buf))
	{
		return false;	
	}
cout<<"tac-";
	cc.addField(buf);

	if(recv(fd, rawdata, sizeof(rawdata), 0) != sizeof(rawdata))
	{
		return false;	
	}
cout<<"toc-";
	cc.addField(rawdata, PACKET_SIZE - 1);

	if(cc() !=  rawdata[12]) //FIXME
	{
#ifdef MAP
		std::cerr << "Erreur checksum" << std::endl;
#endif
		return false;
	}
cout<<"tuc" << endl;*/
	yaw 	= get2bytesfromTable(1, rawdata);
	x   	= get2bytesfromTable(3, rawdata);
	y 		= get2bytesfromTable(5, rawdata);
	speedX 	= get2bytesfromTable(7, rawdata);
	speedY 	= get2bytesfromTable(9, rawdata);
	checksum= rawdata[11];
	static chrono c = chrono();
	StreamFrame frame = {yaw, x, y, speedX, speedY, c.top()};
	sphero->notifyStream(&frame);
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

