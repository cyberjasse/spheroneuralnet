#include "Identity.hpp"
#include <iostream>
#include <stdlib.h>
double Identity::f(double x){
if(x<-100000||x>100000){
	std::cerr << "[Identity] x= " << x <<std::endl;
	exit (EXIT_FAILURE);
}
	return x;
}

double Identity::df(double x){
	return 1.0;
}
