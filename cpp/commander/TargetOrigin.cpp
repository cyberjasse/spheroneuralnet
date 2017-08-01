#include "TargetOrigin.hpp"

TargetOrigin::TargetOrigin(int limit){
	l = limit;
}

StreamFrame TargetOrigin::getTarget(struct StreamFrame *frame){
	l--;
	struct StreamFrame t = {0,0,0,0,0,1,0,0};
	return t;
}

int TargetOrigin::remaining(){
	return l;
}
