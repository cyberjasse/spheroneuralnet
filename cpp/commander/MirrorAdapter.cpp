#include "MirrorAdapter.hpp"

TransformedFrame MirrorAdapter::normalizeFrame(struct StreamFrame currentstate, struct StreamFrame targetstate){
	struct TransformedFrame tf = SimpleAdapter::normalizeFrame(currentstate, targetstate);
	if(tf.targety < 0){
		mirroring = true;
		tf.targety = -tf.targety;
		tf.currentSpeedy = -tf.currentSpeedy;
		tf.targetSpeedy = -tf.targetSpeedy;
		tf.currentAccely = -tf.currentAccely;
	}
	else{
		mirroring = false;
	}
	return tf;
}

int MirrorAdapter::normalizeHead(int16_t head){
	int nhead = SimpleAdapter::normalizeHead(head);
	if(mirroring){
		nhead = DataAdapter::getAngleBetween(-nhead, -179, 180);
	}
	return nhead;
}

int16_t MirrorAdapter::denormalizeHead(int head){
	int nhead = SimpleAdapter::denormalizeHead(head);
	if(mirroring){
		nhead = DataAdapter::getAngleBetween(-nhead, 0, 359);
	}
	return nhead;
}
