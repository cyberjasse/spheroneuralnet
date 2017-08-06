#include "MLPneuron.hpp"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <stdlib.h>

MLPneuron::MLPneuron(DerivableFunction *f, neuronSize_t length){
	function = f;
	ws = new double[length];
	for(neuronSize_t i=0 ; i<length ; i++){
		ws[i] = (rand()/(double)RAND_MAX)*2.0 -1.0;
if(ws[i] < -1.0 || ws[i] > 1.0){
	std::cerr << "[MLPneuron] ws init to "<< ws[i] << std::endl;
	exit (EXIT_FAILURE);
}
	}
	N = length;
}

double MLPneuron::compute(double input[]){
	double lastWeightedSum = 0.0;
	for(int i=0 ; i<N ; i++){
if(input[i] > 101 || input[i] < -101){
	std::cerr << "[MLPneuron] input=" << input[i] << std::endl;
	exit (EXIT_FAILURE);
}
if(ws[i] > 100000 || ws[i] < -100000){
	std::cerr << "[MLPneuron] ws=" << ws[i] << std::endl;
	exit (EXIT_FAILURE);
}
		lastWeightedSum += input[i]*ws[i];
	}
	double toreturn = function->f(lastWeightedSum);
if(toreturn > 100000 || toreturn < -100000){
	std::cerr << "[MLPneuron] toreturn compute=" << toreturn << std::endl;
	exit (EXIT_FAILURE);
}
	return toreturn;
}

void MLPneuron::backpropagation(double errorContribution, double lastInput[], double step, double thisContributions[]){
	for(int i=0 ; i<N ; i++){
		// compute its error contribution
		thisContributions[i] = function->df(lastWeightedSum)*errorContribution;
if(errorContribution > 100000 || errorContribution < -100000){
	std::cerr << "[MLPneuron] errorContribution = " << errorContribution << std::endl;
	exit (EXIT_FAILURE);
}
		//modify weights
		double modification = step*thisContributions[i]*lastInput[i];
if(lastInput[i] > 100000 || lastInput[i] < -100000){
	std::cerr << "[MLPneuron] lastInput[i] = " << lastInput[i] << std::endl;
	exit (EXIT_FAILURE);
}
if(modification > 1000000 || modification < -1000000){
	std::cerr << "[MLPneuron] modification of " << modification << std::endl;
	exit (EXIT_FAILURE);
}
		ws[i] += modification;
	}
}
