#include "StreamDisplayer.hpp"
#include <iostream>
void StreamDisplayer::notify(struct StreamFrame *frame){
	std::cout <<"yaw:"<<frame->yaw
	          <<" x:"<<frame->x
	          <<" y:"<<frame->y
	          <<" speedx:"<<frame->speedx
	          <<" speedy:"<<frame->speedy
	          <<" chrono:"<<frame->chrono
	          << std::endl;
	//delete(frame);
}

StreamDisplayer::StreamDisplayer(){}
