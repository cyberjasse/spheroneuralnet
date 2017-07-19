#include "RandomCommander.hpp"
#include "TransformedFrame.hpp"
#include "DataAdapter.hpp"
#include <unistd.h>
#include <stdlib.h>
Command RandomCommander::getCommand(struct TransformedFrame *frame){
	const int maxspeed = 50;
	const int maxdeltahead = 20;
	int nv = rand()%5;
	this->cv += nv;
	if(cv > maxspeed){
		cv = maxspeed;}
	else if(cv < 0){
		cv = 0;}
	int16_t ny = (int16_t)(rand()%(2*maxdeltahead)) - maxdeltahead;
	cyaw = DataAdapter::correctAngle(cyaw+ny);
	struct Command command = { (uint8_t)(cv) , cyaw };
	//Saving in file
	file << frame->currentSpeedx <<" "<< frame->currentSpeedy <<" "
	     << frame->targetx <<" "<< frame->targety <<" "
	     << frame->targetSpeedx <<" "<< frame->targetSpeedy<<" "
	     << cv <<" "<< cyaw << std::endl;
	return command;
}

RandomCommander::RandomCommander(std::string filename){
	srand(4000);
	cyaw = 0;
	cv = 0;
	file.open(filename);
	file << "#yaw is the orientation of the Sphero.\n"
	     << "#time is the duration between this data frame and the previous. In micro seconds.\n"
	     << "#orderedSpeed is the speed ordered by the commander after receiving this frame.\n"
	     << "#orderedHead is the orientation ordered by the commander after receiving this frame. between 0 and 359.\n"
	     << "yaw x y speedx speedy time orderedSpeed orderedHead\n";
}

RandomCommander::~RandomCommander(){
	file.close();
}
