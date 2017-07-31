#include "RandomCommander.hpp"
#include "DataAdapter.hpp"
#include <unistd.h>
#include <stdlib.h>

int RandomCommander::getnear(int x, int epsilon){
	int R = ( rand()%(2*epsilon) ) - epsilon;
	return x+R;
}

void RandomCommander::notify(struct StreamFrame *frame){

	// CONSTANTS HERE
	const int maxspeed = 70;
	const int maxspeedIfOut = 50;//maximum speed if out of the map
	const int minspeed = 20;
	const int maxdeltaspeed = 5;
	const int maxdeltahead = 5;
	const int maxdeltaAboutTurn = 70;
	
	// Test if we exiting the map. If yes, about-turn
	int maxs = maxspeedIfOut; //redefinition of max speed to decrease it if we exit the map
	int nyaw; // the new yaw
	if(frame->x < bds->left){
		if(status != LEFT){
			std::cerr << "on left border\n";
			nyaw = getnear(90, maxdeltaAboutTurn);
			status = LEFT;
		}
		else nyaw = chead;
	}
	else if(frame->x > bds->right){
		if(status != RIGHT){
			std::cerr << "on right border\n";
			nyaw = getnear(270, maxdeltaAboutTurn);
			status = RIGHT;
		}
		else nyaw = chead;
	}
	else if(frame->y < bds->bottom){
		if(status != BOTTOM){
			std::cerr << "on bottom border\n";
			nyaw = getnear(0, maxdeltaAboutTurn);
			status = BOTTOM;
		}
		else nyaw = chead;
	}
	else if(frame->y > bds->top){
		if(status != TOP){
			std::cerr << "on top border\n";
			nyaw = getnear(180, maxdeltaAboutTurn);
			status = TOP;
		}
		else nyaw = chead;
	}
	else{
	// if no, choose another one
		status = IN;
		maxs = maxspeed;
		nyaw = getnear(frame->yaw, maxdeltahead);
	}
	// correct the neaw head
	if(nyaw < 0)
		nyaw += 360;
	else if(nyaw > 259)
		nyaw -= 360;
	// Choose a new speed
	cv = getnear(cv, maxdeltaspeed);
	if(cv > maxs){
		cv = maxs;
	}
	else if(cv < minspeed){
		cv = minspeed;
	}
	
	// Send command
	chead = nyaw;
	sph->roll( (uint8_t)(cv) , (int16_t)(nyaw) );
	// Save data in file
	file<< frame->yaw <<" "
		<< frame->x <<" "<< frame->y <<" "
		<< frame->speedx <<" "<< frame->speedy <<" "
		<< frame->chrono <<" "
		<< cv <<" "<< nyaw << std::endl;
}

RandomCommander::RandomCommander(Sphero *sphero, std::string filename, struct Bounds *bounds){
	srand(4000);
	cv = 0;
	sph = sphero;
	file.open(filename);
	file << "yaw x y currentSpeedx currentSpeedy time orderedSpeed orderedHead\n";
	if(bounds == NULL){
		struct Bounds b = {-6000,6000,6000,-6000};
		bds = &b;
	}
	else{
		bds = bounds;
	}
	status = IN;
}

