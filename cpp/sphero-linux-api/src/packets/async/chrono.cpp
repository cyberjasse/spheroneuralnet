/** */
#include "chrono.hpp"
#include <sys/time.h>
#include <iostream>
#define GETPERIOD

chrono::chrono(){
	start();
}

void chrono::start(){
	clock_gettime(CLOCK_MONOTONIC, &lasttime);
}

unsigned long chrono::minus(struct timespec t1, struct timespec t2){
	// (Ak+b) - (Ck+d) = Ak+b-Ck-d = Ak-Ck+b-d = k*(A-C)+b-d
	unsigned long us = (t1.tv_sec - t2.tv_sec) * 1000000;
	// b-d can be <0 . And b ,d are unsigned. So dont do us += b-d;
	us += t1.tv_nsec/1000;
	us -= t2.tv_nsec/1000;
	return us;
}

unsigned long chrono::top(){
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
#ifdef GETTIME
	return ts.tv_sec*1000000 + ts.tv_nsec/1000;
#else
	unsigned long diff = minus(ts, lasttime);
	lasttime = ts;
	return diff;
#endif
}
	
