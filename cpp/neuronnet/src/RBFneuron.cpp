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
	double sum = 0.0;
	for(neuronSize_t k=0 ; k<N ; k++){
		std::cout << "Hidden neuron -> input=" << input[k] << ", mu=" << mu[k] << ", sigma=" << sigma[k];
		double frac = input[k]-mu[k];
		frac = frac*frac;
		frac = frac/ (sigma[k]*sigma[k]);//variance plutot
		sum += frac;
	}
	lastOutput = exp(-sum/2.0);
	std::cout << ", RESULT=" << lastOutput << std::endl;
	return lastOutput;
}

void RBFneuron::backpropagation(double errorContribution, double lastInput[], double step, double thisContributions[]){
	for(neuronSize_t k=0 ; k<N ; k++){
		double dif = lastInput[k]-mu[k];
		double cor = dif/ (sigma[k]*sigma[k]);
		cor *= lastOutput;
		cor *= step;
		cor *= errorContribution;
		std::cout << "Hidden neuron <- error=" << errorContribution << " MODIFICATION mu=" << cor;
		mu[k] += cor;
		cor *= dif;
		cor /= sigma[k];
		std::cout << " MODIFICATION sigma=" << cor << std::endl;
		sigma[k] += cor;
		return; //TODO this contribution
	}
}
