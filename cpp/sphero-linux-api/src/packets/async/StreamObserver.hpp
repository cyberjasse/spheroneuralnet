#ifndef STREAMOBSERVER_HPP
#define STREAMOBSERVER_HPP
#include "StreamFrame.hpp"
/**
 * The interface for a class that want to be notified about received stream data
 */
class StreamObserver{
	/**
	 * Notify that a frame of sampled data is received
	 * @param sp The data received
	 */
	public : virtual void notify(struct StreamFrame *frame);
	
	public : StreamObserver();
};
#endif
