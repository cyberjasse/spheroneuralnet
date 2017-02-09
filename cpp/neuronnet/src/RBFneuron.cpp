#include "RBFneuron.hpp"
#include <cmath>
#include <iostream>

double lastOutput;

RBFneuron::RBFneuron(double prototype[], double sd[], neuronSize_t length){
	mu = prototype;
	sigma = sd;
	N = length;
}

double RBFneuron::compute(double input[]){
	double sum = 0;
	for(neuronSize_t k=0 ; k<N ; k++){
		double frac = input[k]-mu[k];
		frac = frac*frac;
		frac = frac/ sigma[k]*sigma[k];
		sum += frac;
	}
	lastOutput = exp(-sum/2.0);
	return lastOutput;
}

void RBFneuron::backpropagation(double errorContribution, double lastInput[], double step, double thisContributions[]){
	for(neuronSize_t k=0 ; k<N ; k++){
		double dif = lastInput[k]-mu[k];
		double cor = dif/ (sigma[k]*sigma[k]);
		cor *= lastOutput;
		cor *= step;
		cor *= errorContribution;
		mu[k] += cor;
		cor *= dif;
		cor /= sigma[k];
		sigma[k] += cor;
		return; //TODO this contribution
	}
}
