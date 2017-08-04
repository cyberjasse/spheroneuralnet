#include "Sigmoid.hpp"
#include <math.h>
#include <iostream>
#include <stdlib.h>

Sigmoid::Sigmoid(double lambda){
	l = lambda;
}

double Sigmoid::f(double x){
	double ex = exp(-l*x);
	if(isinf(ex)){
		#ifdef SDEBUG
		std::cerr << "[Sigmoid.cpp] exp(-"<<l<<"*"<<x<<") is inf\n";
		#endif
		l /= 2.0;
		return f(x);
	}
	else if(ex < 0.0000001){
		#ifdef SDEBUG
		std::cerr << "[Sigmoid.cpp] exp(-"<<l<<"*"<<x<<") is perhaps too small\n";
		#endif
		l /= 2.0;
		return f(x);
	}
	else if(isnan(ex)){
		std::cerr << "[Sigmoid.cpp] exp(-"<<l<<"*"<<x<<") is nan\n";
		exit (EXIT_FAILURE);
	}
	return 1/(1+ex);
}

double Sigmoid::df(double x){
	double ex = exp(-l*x);
	double den = 1+ex;
	den = den*den;
	return l*ex/den;
}
