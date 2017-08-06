#include "AheadCommander.hpp"

AheadCommander::AheadCommander(Sphero *sph, std::string filename){
	file.open(filename);
	sphero = sph;
	file << "time x y speedx speedy yaw\n";
}

void AheadCommander::notify(struct StreamFrame *frame){
	file << frame->chrono <<" "<< frame->x <<" "<< frame->y <<" "<<frame->speedx <<" "<<frame->speedy<<" "<<frame->yaw << std::endl;
	sphero->roll( (uint8_t)(50) , (int16_t)(90) );
}
