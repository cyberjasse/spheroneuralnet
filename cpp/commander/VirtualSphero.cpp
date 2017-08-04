#include "VirtualSphero.hpp"
#include <math.h>

#include <iostream>

void VirtualSphero::roll(uint8_t speed, uint16_t heading, uint8_t state){
	// CONSTANTS HERE
	const double maxSpeed = 147.0; // maximum speed
	const double m = 3.2; // target speed will be m*speed
	const double a = 1.5; // acceleration will be a*(target speed - current speed)
	const double angularSpeed = 450.0; // angular speed
	const double minSpeed = 1.5; // if current speed is < minSpeed and acceleration<0 then speed become 0
	// Compute the new state about speed
	double targetSpeed = m*speed;
	acceleration = a*(targetSpeed - currentSpeed);
	double newSpeed;
	if(currentSpeed<minSpeed && acceleration<0.0)
		newSpeed = 0.0;
	else
		newSpeed = currentSpeed + acceleration*streamingT;
	// Compute the new state about angle
	int deltaAngle = DataAdapter::getAngleDiff(orientation,heading);
	double maxDeltaAngular = angularSpeed*streamingT;
	double newOrientation;
	if(deltaAngle > 0){
		if(deltaAngle < maxDeltaAngular)
			newOrientation = orientation+deltaAngle;
		else
			newOrientation = orientation+maxDeltaAngular;
	}
	else{
		if(deltaAngle > -maxDeltaAngular)
			newOrientation = orientation+deltaAngle;
		else
			newOrientation = orientation-maxDeltaAngular;
	}
	// correct angle
	if(newOrientation > 359)
		newOrientation -= 360;
	else if(newOrientation < 0)
		newOrientation += 360;
	// Compute the new state about position
	double radangle = DataAdapter::degreeToRadian(newOrientation);
	speedx = newSpeed*cos(radangle);
	speedy = newSpeed*sin(radangle);
	x += speedx*streamingT;
	y += speedy*streamingT;
	// send the new state
	orientation = newOrientation;
	currentSpeed = newSpeed;
}

void VirtualSphero::notifyStream(){
	struct StreamFrame sf;
	int yaw = round(orientation);
	sf.yaw = DataAdapter::correctAngle(yaw);
	sf.x = round(x);
	sf.y = round(y);
	sf.speedx = round(speedx);
	sf.speedy = round(speedy);
	sf.chrono = 200000;
	double radangle = DataAdapter::degreeToRadian(orientation);
	sf.ax = acceleration*cos(radangle);
	sf.ay = acceleration*sin(radangle);
	observer->notify(&sf);
}

void VirtualSphero::startStream(uint16_t freq){
	streamingT = 1.0/freq;
	for(int i=0 ; i<N ; i++){
		notifyStream();
	}
}

void VirtualSphero::addStreamObserver(StreamObserver *so){
	observer = so;
}

VirtualSphero::VirtualSphero(int Nframes){
	N = Nframes;
}
