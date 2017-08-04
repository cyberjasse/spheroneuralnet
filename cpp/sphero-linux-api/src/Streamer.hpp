#ifndef STREAMER_HPP
#define STREAMER_HPP
#include "packets/async/StreamFrame.hpp"
#include "packets/async/StreamObserver.hpp"

/**
 * The interface of a device that can stream data. Like the sphero or a virtual sphero
 */
class Streamer{
public:
	/** Allow a stream observer to subsribe and then receive data frame from streaming */
	virtual void addStreamObserver(StreamObserver *so)=0;
	/** Send command */
	virtual void roll(uint8_t speed, uint16_t heading, uint8_t state = 1)=0;

	virtual void startStream(uint16_t freq)=0;
};
#endif
