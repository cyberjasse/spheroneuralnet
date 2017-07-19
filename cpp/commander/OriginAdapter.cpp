#include "OriginAdapter.hpp"
#include "Command.hpp"
#include <cstdint>

OriginAdapter::OriginAdapter(Sphero *sphero, Commander *commander, Target *target){
	sph = sphero;
	cmdr = commander;
	tar = target;
}

void OriginAdapter::notify(struct StreamFrame *frame){
	if(frame == NULL){
		sph->disconnect();
		return;
	}
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
	struct TransformedFrame tf = {currentspeedx, currentspeedy, targetx, targety, targetspeedx, targetspeedy};
	struct Command command = cmdr->getCommand(&tf);
	// transform the command
	int16_t yaw = DataAdapter::correctAngle( frame->yaw + command.yaw );
	if(yaw < 0){
		yaw += 360; // To have yaw between 0 and 359.
	}
	sph->roll(command.speed , yaw);
}
