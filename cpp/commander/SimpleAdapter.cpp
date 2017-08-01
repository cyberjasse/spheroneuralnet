#include "SimpleAdapter.hpp"

TransformedFrame SimpleAdapter::normalizeFrame(struct StreamFrame currentstate, struct StreamFrame targetstate){
	// The target to the origin
	int16_t targetx = targetstate.x - currentstate.x;
	int16_t targety = targetstate.y - currentstate.y;
	int16_t targetspeedx = targetstate.speedx;
	int16_t targetspeedy = targetstate.speedy;
	int16_t currentspeedx = currentstate.speedx;
	int16_t currentspeedy = currentstate.speedy;
	int16_t currentaccelx = currentstate.ax;
	int16_t currentaccely = currentstate.ay;
	// rotation
	lastangle = -currentstate.yaw; //rotation to apply
	double radangle = DataAdapter::degreeToRadian( lastangle );
	DataAdapter::rotation(&targetx, &targety, radangle);
	DataAdapter::rotation(&targetspeedx, &targetspeedy, radangle);
	DataAdapter::rotation(&currentspeedx, &currentspeedy, radangle);
	DataAdapter::rotation(&currentaccelx, &currentaccely, radangle);
	struct TransformedFrame tf = {currentspeedx,currentspeedy, currentstate.chrono, targetx,targety, targetspeedx,targetspeedy, currentaccelx,currentaccely};
	return tf;
}
		
int SimpleAdapter::normalizeSpeed(uint8_t speed){
	return (int)(speed);
}
		
int SimpleAdapter::normalizeHead(int16_t head){
	// return an angle between -180 and 179 degree then apply last rotation
	// from an angle between 0 and 359
	return getAngleBetween(head + lastangle, -195,194);
}
		
uint8_t SimpleAdapter::denormalizeSpeed(int speed){
	return (uint8_t)(speed);
}
		
int16_t SimpleAdapter::denormalizeHead(int head){
	// return an angle between 0 and 359 degree then apply inverse last rotation
	// from an angle between -180 and 179
	return DataAdapter::getAngleBetween(head - lastangle, 0,359);
}
