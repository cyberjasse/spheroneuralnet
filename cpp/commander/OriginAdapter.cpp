#include "OriginAdapter.hpp"
#include "Command.hpp"
#include <cstdint>

OriginAdapter::OriginAdapter(Sphero *sphero, Commander *commander, Target *target, std::string filename){
	sph = sphero;
	cmdr = commander;
	tar = target;
	if(filename != ""){
		file.open(filename);
		file<< "#yaw is the current direction of the sphero.\n"
			<< "x,y is the current position.\n"
			<< "#currentSpeedx,currentSpeedy is the current speed vector of the sphero.\n"
			<< "#time is the duration between this data frame and the previous. In micro seconds.\n"
			<< "#orderedSpeed is the speed ordered by the commander after receiving this frame.\n"
			<< "#orderedHead is the orientation ordered by the commander after receiving this frame. between -179 and 180.\n"
			<< "yaw x y currentSpeedx currentSpeedy time orderedSpeed orderedHead\n";
		isFileOpened = true;
	}
	else{
		isFileOpened = false;
	}
}

void OriginAdapter::notify(struct StreamFrame *frame){
	if(tar->remaining() > 0){
		// The target to the origin
		struct StreamFrame targetFrame = tar->getTarget(frame);
		int16_t targetx = targetFrame.x - frame->x;
		int16_t targety = targetFrame.y - frame->y;
		int16_t targetspeedx = targetFrame.speedx - frame->speedx;
		int16_t targetspeedy = targetFrame.speedy - frame->speedy;
		int16_t currentspeedx = frame->speedx;
		int16_t currentspeedy = frame->speedy;
		// rotation
		double angle = DataAdapter::degreeToRadian( -frame->yaw );
		DataAdapter::rotation(&targetx, &targety, angle);
		DataAdapter::rotation(&targetspeedx, &targetspeedy, angle);
		DataAdapter::rotation(&currentspeedx, &currentspeedy, angle);
		// send the transformed data
		struct TransformedFrame tf = {currentspeedx, currentspeedy, frame->chrono, targetx, targety, targetspeedx, targetspeedy};
		struct Command command = cmdr->getCommand(&tf);
		// transform the command
		int16_t yaw = DataAdapter::correctAngle( frame->yaw + command.yaw );
		if(yaw < 0){
			yaw += 360; // To have yaw between 0 and 359. See the doc: received yaw is >-179& <180 while yaw to send is >0 & <359.
		}
		sph->roll(command.speed , yaw); // send the command to the sphero
		// write
		if(isFileOpened){
			file<< frame->yaw <<" "
				<< frame->x <<" "<< frame->y <<" "
				<< frame->speedx <<" "<< frame->speedy <<" "
				<< frame->chrono <<" "
				<< command.speed <<" "<< command.yaw << std::endl;
		}
	}
	else{
		sph->disconnect();
	}
}
