#ifndef VIRTUALSPHERO_HPP
#define VIRTUALSPHERO_HPP
#include "../sphero/Streamer.hpp"
#include "DataAdapter.hpp"

/**
 * A virtual Sphero
 */
class VirtualSphero : public Streamer{
private :
	/** the x of the speed vector */
	double speedx=0.0;
	/** the y of the speed vector */
	double speedy=0.0;
	/** The distance of the speed vector */
	double currentSpeed=0.0;
	/** The acceleration */
	double acceleration=0.0;
	/** the x of the position */
	double x=0.0;
	/** the y of the position */
	double y=0.0;
	/** the orientation in degrees. Between 0 and 359*/
	double orientation=0.0;
	/** the streaming period */
	double streamingT=0.0;
	/** the number of frame to notify in total */
	int N;
	
	/** The subscribed stream observer */
	StreamObserver *observer;

public :
	
	/**
	 * @param Nframes The number of frames to receive
	 */
	VirtualSphero(int Nframes);
	
	void notifyStream();
	// override
	virtual void addStreamObserver(StreamObserver *so);
	virtual void startStream(uint16_t freq);
	virtual void roll(uint8_t speed, uint16_t heading, uint8_t state = 1);
};
#endif
