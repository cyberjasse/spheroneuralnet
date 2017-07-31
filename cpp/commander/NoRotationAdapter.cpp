#include "NoRotationAdapter.hpp"

TransformedFrame NoRotationAdapter::normalizeFrame(struct StreamFrame currentstate, struct StreamFrame targetstate){
	struct TransformedFrame tf;
	tf.targetx = targetstate.x - currentstate.x;
	tf.targety = targetstate.y - currentstate.y;
	tf.currentSpeedx = currentstate.speedx;
	tf.currentSpeedy = currentstate.speedy;
	tf.time = targetstate.chrono;
	tf.targetSpeedx = targetstate.speedx;
	tf.targetSpeedy = targetstate.speedy;
	return tf;
}

int NoRotationAdapter::normalizeSpeed(uint8_t speed){
	return speed;
}

int NoRotationAdapter::normalizeHead(int16_t head){
	return head;
}

uint8_t NoRotationAdapter::denormalizeSpeed(int speed){
	return (uint8_t)(speed);
}

int16_t NoRotationAdapter::denormalizeHead(int head){
	return (int16_t)(head);
}
