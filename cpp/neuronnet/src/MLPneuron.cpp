#include "MLPneuron.hpp"
#include <cstdlib>
#include <time.h>

MLPneuron::MLPneuron(DerivableFunction *f, neuronSize_t length){
	function = f;
	ws = new double[length];
	for(neuronSize_t i=0 ; i<length ; i++){
		ws[i] = (rand()/(double)RAND_MAX)*2.0 -1.0;
	}
	N = length;
}

double MLPneuron::compute(double input[]){
	double lastWeightedSum = 0.0;
	for(int i=0 ; i<N ; i++){
		lastWeightedSum += input[i]*ws[i];
	}
	return function->f(lastWeightedSum);
}

void MLPneuron::backpropagation(double errorContribution, double lastInput[], double step, double thisContributions[]){
	for(int i=0 ; i<N ; i++){
		// compute its error contribution
		thisContributions[i] = function->df(lastWeightedSum)*errorContribution;
		//modify weights
		ws[i] += step*thisContributions[i]*lastInput[i];
	}
}
