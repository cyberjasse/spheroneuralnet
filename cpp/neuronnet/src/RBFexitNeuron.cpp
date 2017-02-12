#include "RBFexitNeuron.hpp"
#include "Neuron.hpp"
#include <cstdlib>
#include <time.h>
#include <iostream>

/** last Weighted sum*/
double lastWeightedSum;
/** sum of last input elements*/
double lastSum;

RBFexitNeuron::RBFexitNeuron(double weights[], neuronSize_t length){
	ws = weights;
	N = length;
}

RBFexitNeuron::RBFexitNeuron(neuronSize_t length){
	ws = new double[length];
	srand(time(NULL));
	for(neuronSize_t i=0 ; i<length ; i++){
		ws[i] = (rand()/(double)RAND_MAX)*2.0 -1.0;
	}
	N = length;
}


double RBFexitNeuron::compute(double input[]){
	lastSum = 1.0;// 1 instead of 0 because we add a bias
	lastWeightedSum = 1.0;//idem
	for(neuronSize_t j=0 ; j<N ; j++){
		lastSum += input[j];
		lastWeightedSum += input[j]*ws[j];
	}
	double result = lastWeightedSum/lastSum;
	std::cout << "Exit neuron   -> RESULT=" << result << std::endl;
	return result;
}

void RBFexitNeuron::backpropagation(double errorContribution, double lastInput[], double step, double thisContributions[]){
	double R2 = (lastSum*lastSum);
	for(neuronSize_t r=0 ; r<N ; r++){
		//compute the error contribution of this neuron to the activation fonction of the jth neuron of the previous layer
		thisContributions[r] = errorContribution*(ws[r]*lastSum - lastWeightedSum)/R2;
		std::cout << "Exit neuron   <- thisContributions=" << thisContributions[r];
		//modify weights
		double modification = step * errorContribution * lastInput[r]/lastSum;
		std::cout << " MODIFICATION=" << modification << std::endl;
		ws[r] += modification;
	}
	return;
}
