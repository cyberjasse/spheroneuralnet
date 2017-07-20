#include "RandomCommander.hpp"
#include "DataAdapter.hpp" //For its static functions
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
	return command;
}

RandomCommander::RandomCommander(){
	srand(4000);
	cyaw = 0;
	cv = 0;
}
