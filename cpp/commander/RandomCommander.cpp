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

int RandomCommander::aboutTurn(int angle, int currentYaw){

	const int maxdeltaAboutTurn = 70; //less than 90
	const int minNstepsAboutTurn = 2; //minimum number of steps to do a about-turn
	const int maxNstepsAboutTurn = 5; //maximum number of steps to do a about-turn
	
	int nyaw = getnear(angle, maxdeltaAboutTurn);
	setYawTarget(nyaw, currentYaw, minNstepsAboutTurn, maxNstepsAboutTurn);
	return nyaw;
}

void RandomCommander::notify(struct StreamFrame *frame){

	// CONSTANTS HERE
	const int maxspeed = 70; //less than 255
	const int minspeed = 20; //less than 255
	const int minNstepsSpeed = 3;
	const int maxNstepsSpeed = 8;
	const int minNstepsYaw = 3;
	const int maxNstepsYaw = 10;
	const int chanceSetNewTarget = 3;
	
	// Test if we exiting the map. If yes, about-turn
	int nyaw;
	// convert frame->yaw
	frame->yaw = -frame->yaw; //because when we drive to 90, yaw become -90
	frame->yaw = DataAdapter::getAngleBetween(frame->yaw, 0, 359);
	if(frame->x < bds->left){//test if we are out of the map, on the left
		if(frame->y < bds->bottom){
			if(status != LEFTBOTTOM){
				std::cerr << "on left-bottom corner\n";
				status = LEFTBOTTOM;
				nyaw = aboutTurn(45, frame->yaw);
			}
		}
		else if(frame->y > bds->top){
			if(status != LEFTTOP){
				std::cerr << "on left-top corner\n";
				status = LEFTTOP;
				nyaw = aboutTurn(135, frame->yaw);
			}
		}
		else if(status != LEFT){//test if we didn't already set a yaw target to do about-turn 
			std::cerr << "on left border\n";
			nyaw = aboutTurn(90, frame->yaw);
			status = LEFT;
		}
	}
	if(frame->x > bds->right){
		if(frame->y < bds->bottom){
			if(status != RIGHTBOTTOM){
				std::cerr << "on right-bottom corner\n";
				status = RIGHTBOTTOM;
				nyaw = aboutTurn(315, frame->yaw);
			}
		}
		else if(frame->y > bds->top){
			if(status != RIGHTTOP){
				std::cerr << "on right-top corner\n";
				status = RIGHTTOP;
				nyaw = aboutTurn(225, frame->yaw);
			}
		}
		else if(status != RIGHT){//test if we didn't already set a yaw target to do about-turn 
			std::cerr << "on right border\n";
			nyaw = aboutTurn(270, frame->yaw);
			status = RIGHT;
		}
	}
	else if(frame->y < bds->bottom){
		if(status != BOTTOM){
			std::cerr << "on bottom border\n";
			status = BOTTOM;
			nyaw = aboutTurn(0, frame->yaw);
		}
	}
	else if(frame->y > bds->top){
		if(status != TOP){
			std::cerr << "on top border\n";
			status = TOP;
			nyaw = aboutTurn(180, frame->yaw);
		}
	}
	else{
	// if no, perhaps choose a new yaw if the targeted yaw is reached.
		if(status != IN){
			status = IN;
			std::cerr << "IN the map\n";
		}
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
	std::cout << "x="<<frame->x<<" y="<<frame->y<<" yaw="<<frame->yaw<<" chead="<<chead<<" target="<<targetYaw<<std::endl;
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

