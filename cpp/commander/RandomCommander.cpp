#include "RandomCommander.hpp"
#include <unistd.h>
#include <stdlib.h>
void RandomCommander::notify(struct TransformedFrame *frame){
	//Random command
	const uint8_t maxspeed = 50;
	const int16_t maxhead = 359;
	uint8_t nv = (uint8_t)(rand()%5);
	if(maxspeed-nv >= cv)
		cv += nv;
	else
		cv = maxspeed;
	int16_t ny = (int16_t)(rand()%20);
	if(maxhead-ny >= cyaw)
		cyaw += ny;if
	else
		cyaw = maxhead;
	if(cyaw < 0)
		cyaw = 0;
	s->roll(cv, cyaw);
	//Saving in file
	if(file != NULL){
		file << frame->currentSpeedx <<" "<< frame->currentSpeedy <<" "
		     << frame->targetx <<" "<< frame->targety <<" "
		     << frame->targetSpeedx <<" "<< frame->targetSpeedy<<" "
		     << cv <<" "<< cyaw << std::endl;
	}
}
RandomCommander::RandomCommander(Sphero *sphero, std::string filename=NULL){
	srand(4000);
	cyaw = 0;
	cv = 0;
	s = sphero;
	if(filename != NULL){
		file.open(filename);
		file << "#yaw is the orientation of the Sphero.\n"
		     << "#time is the duration between this data frame and the previous. In micro seconds.\n"
		     << "#orderedSpeed is the speed ordered by the commander after receiving this frame.\n"
		     << "#orderedHead is the orientation ordered by the commander after receiving this frame. between 0 and 359.\n"
		     << "yaw x y speedx speedy time orderedSpeed orderedHead\n";
	}
	else{
		file = NULL;
	}
}

RandomCommander::~RandomCommander(){
	if(file != NULL){
		file.close();
	}
}
