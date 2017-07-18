#ifndef STREAMDISPLAYER_HPP
#define STREAMDISPLAYER_HPP
#include "StreamObserver.hpp"
#include "StreamFrame.hpp"
/**
 * Observe received data and display it to the main output
 */
class StreamDisplayer : public StreamObserver{
	/**
	 * Display data received
	 */
	public : virtual void notify(struct StreamFrame *frame);
	public : StreamDisplayer();
};
#endif
