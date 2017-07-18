/** */
#include "chrono.hpp"
#include <sys/time.h>
#include <iostream>

chrono::chrono(){
	start();
}

void chrono::start(){
	gettimeofday(&lasttime, NULL);
}

unsigned long chrono::minus(struct timeval t1, struct timeval t2){
	unsigned long us = (t1.tv_sec - t2.tv_sec) * 1000000;
	us += (t1.tv_usec - t2.tv_usec);
	return us;
}

unsigned long chrono::top(){
	struct timeval t2;
	gettimeofday(&t2, NULL);
	unsigned long us = minus(t2 , lasttime);
	lasttime = t2;
	return us;
}
	
