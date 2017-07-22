#include "RandomCommander.hpp"
#include <unistd.h>
#include <stdlib.h>

void RandomCommander::notify(struct StreamFrame *frame){

	// CONSTANTS HERE
	const int maxspeed = 70;
	const int maxspeedIfOut = 50;//maximum speed if out of the map
	const int minspeed = 15;
	const int maxdeltaspeed = 15;
	const int maxdeltahead = 25;
	
	// Test if we exiting the map. If yes, about-turn
	int maxs = maxspeedIfOut; //redefinition of max speed to decrease it if we exit the map
	int nyaw; // the new yaw
	if(frame->x < bds->left){
		std::cerr << "on left border\n";
		nyaw = 90;
	}
	else if(frame->x > bds->right){
		std::cerr << "on right border\n";
		nyaw = 270;
	}
	else if(frame->y < bds->bottom){
		std::cerr << "on bottom border\n";
		nyaw = 0;
	}
	else if(frame->y > bds->top){
		std::cerr << "on top border\n";
		nyaw = 180;
	}
	else{
	// if no, choose another one
		maxs = maxspeed;
		int deltayaw = rand()%(2*maxdeltahead) - maxdeltahead;
		nyaw = frame->yaw + 180 + deltayaw;
		if(nyaw < 0) nyaw += 360;
		else if(nyaw > 360) nyaw -= 360;
	}
	
	// Choose a new speed
	int dspeed = rand()%(2*maxdeltaspeed) - maxdeltaspeed;
	cv += dspeed;
	if(cv > maxs){
		cv = maxs;
	}
	else if(cv < minspeed){
		cv = minspeed;
	}
	
	// Send command
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
	file<< "#yaw is the current direction of the sphero, in degree. between -179 and 180\n"
		<< "#x,y is the current position, in cm\n"
		<< "#currentSpeedx,currentSpeedy is the current speed vector of the sphero.\n"
		<< "#time is the duration between this data frame and the previous. In micro seconds.\n"
		<< "#orderedSpeed is the speed ordered by the commander after receiving this frame.\n"
		<< "#orderedHead is the orientation ordered by the commander after receiving this frame. between -179 and 180\n"
		<< "yaw x y currentSpeedx currentSpeedy time orderedSpeed orderedHead\n";
	if(bounds == NULL){
		struct Bounds b = {-6000,6000,6000,-6000};
		bds = &b;
	}
	else{
		bds = bounds;
	}
}

