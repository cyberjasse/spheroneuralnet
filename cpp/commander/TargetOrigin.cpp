#include "TargetOrigin.hpp"

TargetOrigin::TargetOrigin(int limit){
	l = limit;
}

StreamFrame TargetOrigin::getTarget(struct StreamFrame *frame){
	struct StreamFrame t = {0,0,0,0,0,1};
	return t;
}
