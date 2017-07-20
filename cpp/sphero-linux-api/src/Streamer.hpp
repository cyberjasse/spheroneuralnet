#ifndef STREAMER_HPP
#define STREAMER_HPP
#include "packets/async/StreamFrame.hpp"
#include "packets/async/StreamObserver.hpp"
#include <vector>
/**
 * A device that can stream data. Contains functions to subscribe a StreamObserver then start the stream
 */
class Streamer{
	private :
		std::vector<StreamObserver*> streamobservers = std::vector<StreamObserver*>();;
	
	public :
		/**
		 * Notify to all subscribed StreamObserver the new received streaming frame.
		 */
		virtual void notifyStream(struct StreamFrame *frame);
		
		/**
		 * All a StreamObserver to be subscribed
		 */
		virtual void addStreamObserver(StreamObserver *so);
		
		/**
		 * Start the stream
		 */
		virtual void startStream()=0;
};
#endif
