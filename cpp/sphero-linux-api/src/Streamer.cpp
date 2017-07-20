#include "Streamer.hpp"

void Streamer::notifyStream(struct StreamFrame *frame){
	for(StreamObserver *so : streamobservers){
		so->notify(frame);
	}
}

void Streamer::addStreamObserver(StreamObserver *so){
	streamobservers.push_back(so);
}
