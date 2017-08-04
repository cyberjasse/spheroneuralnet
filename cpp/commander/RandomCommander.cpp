#include "RandomCommander.hpp"
#include "DataAdapter.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int RandomCommander::getnear(int x, int epsilon){
	int R = ( rand()%(2*epsilon) ) - epsilon;
	return x+R;
}

void RandomCommander::setYawTarget(int target, int currentYaw, int minNsteps, int maxNsteps){
	targetYaw = target;
	yawNsteps = rand()%(maxNsteps-minNsteps) + minNsteps;
	yawstep = DataAdapter::getAngleDiff(currentYaw,targetYaw)/yawNsteps;
}

void RandomCommander::setSpeedTarget(int target, int currentSpeed, int minNsteps, int maxNsteps){
	targetSpeed = target;
	speedNsteps = rand()%(maxNsteps-minNsteps) + minNsteps;
	speedstep = (target-currentSpeed)/speedNsteps;
}

void RandomCommander::notify(struct StreamFrame *frame){

	// CONSTANTS HERE
	const int maxspeed = 170;
	const int minspeed = 20;
	const int maxdeltaAboutTurn = 60;
	const int minNstepsSpeed = 3;
	const int maxNstepsSpeed = 8;
	const int minNstepsYaw = 4;
	const int maxNstepsYaw = 10;
	const int minNstepsAboutTurn = 3; //minimum number of steps to do a about-turn
	const int maxNstepsAboutTurn = 6; //maximum number of steps to do a about-turn
	const int chanceSetNewTarget = 3;
	
	// Test if we exiting the map. If yes, about-turn
	int nyaw;
	if(frame->x < bds->left && status != LEFT){ //test if we are out of the map, on the left. Then, test if we didn't already set a yaw target to do about-turn
		std::cerr << "on left border\n";
		nyaw = getnear(90, maxdeltaAboutTurn);
		status = LEFT;
		setYawTarget(nyaw, frame->yaw, minNstepsAboutTurn, maxNstepsAboutTurn);
	}
	else if(frame->x > bds->right && status != RIGHT){
		std::cerr << "on right border\n";
		nyaw = getnear(270, maxdeltaAboutTurn);
		status = RIGHT;
		setYawTarget(nyaw, frame->yaw, minNstepsAboutTurn, maxNstepsAboutTurn);
	}
	else if(frame->y < bds->bottom && status != BOTTOM){
		std::cerr << "on bottom border\n";
		nyaw = getnear(0, maxdeltaAboutTurn);
		status = BOTTOM;
		setYawTarget(nyaw, frame->yaw, minNstepsAboutTurn, maxNstepsAboutTurn);
	}
	else if(frame->y > bds->top && status != TOP){
		std::cerr << "on top border\n";
		nyaw = getnear(180, maxdeltaAboutTurn);
		status = TOP;
		setYawTarget(nyaw, frame->yaw, minNstepsAboutTurn, maxNstepsAboutTurn);
	}
	else{
	// if no, perhaps choose a new yaw if the targeted yaw is reached.
		if(yawNsteps == 0){
			// probability to set a new target
			if(rand()%chanceSetNewTarget==0){//then set a new target yaw
				nyaw = rand()%360;
				setYawTarget(nyaw, frame->yaw, minNstepsYaw, maxNstepsYaw);
			}
		}
	}
	// perhaps choose a new speed if the targeted speed is reached.
	if(rand()%chanceSetNewTarget==0){//then set a new target speed
		int nspeed = rand()%(maxspeed-minspeed) + minspeed;
		setSpeedTarget(nspeed, cv, minNstepsSpeed, maxNstepsSpeed);
	}
	// Send command
	roll();
	// Save data in file
	file<< frame->yaw <<" "
		<< frame->x <<" "<< frame->y <<" "
		<< frame->speedx <<" "<< frame->speedy <<" "
		<< frame->ax <<" "<< frame->ay <<" "
		<< frame->chrono <<" "
		<< cv <<" "<< chead << std::endl;
}

void RandomCommander::roll(){
	// compute new head
	if(yawNsteps <= 1){
		chead = targetYaw;
		yawNsteps = 0;
	}
	else{
		chead += yawstep;
		yawNsteps--;
	}
	//compute new speed
	if(speedNsteps <= 1){
		cv = targetSpeed;
		speedNsteps = 0;
	}
	else{
		cv += speedstep;
		speedNsteps--;
	}
	// correct the neaw head
	if(chead < 0)
		chead += 360;
	else if(chead > 359)
		chead -= 360;
	// send command
	sph->roll( (uint8_t)(cv) , (int16_t)(chead) );
}

RandomCommander::RandomCommander(Streamer *sphero, std::string filename, struct Bounds *bounds){
	srand (time(NULL));
	cv = 0;
	sph = sphero;
	file.open(filename);
	file << "yaw x y speedx speedy accelx accely time orderedSpeed orderedHead\n";
	if(bounds == NULL){
		struct Bounds b = {-6000,6000,6000,-6000};
		bds = &b;
	}
	else{
		bds = bounds;
	}
	status = IN;
	yawNsteps = 0;
	speedNsteps = 0;
	chead = 0;
	cv = 0;
	targetYaw = chead;
	targetSpeed = cv;
}

