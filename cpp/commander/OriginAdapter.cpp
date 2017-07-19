#include "OriginAdapter.hpp"
#include <cstdint>

void OriginAdapter::notify(struct StreamFrame *frame){
	// The target to the origin
	int16_t targetx = tar->x - frame->x;
	int16_t targety = tar->y - frame->y;
	int16_t targetspeedx = tar->targetSpeedx - frame->speedx;
	int16_t targetspeedy = tar->targetSpeedy - frame->speedy;
	int16_t currentspeedx = frame->speedX;
	int16_t currentspeedy = frame->speedY;
	// rotation
	double angle = DataAdapter.degreeToRadian( -frame->yaw );
	DataAdapter.rotation(&targetx, &targety, angle);
	DataAdapter.rotation(&targetspeedx, &targetspeedy, angle);
	DataAdapter.rotation(&currentspeedx, &currentspeedy, angle);
	// send the transformed data
	struct TransformedFrame tf = {currentspeedx, currentspeedy, targetx, targety, targetspeedx, targetspeedy};
	struct Command *command cmdr->getCommand(tf);
	// transform the command
	int16_t yaw = DataAdapter.correctAngle( frame->yaw + command->yaw );
	yaw += 180; //To have between 0 and 359
	sph->roll(command->speed , command->yaw);
	delete(command);
}


