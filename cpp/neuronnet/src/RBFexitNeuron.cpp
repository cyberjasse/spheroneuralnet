#include "RBFexitNeuron.hpp"
#include "Neuron.hpp"

/** last Weighted sum*/
double lastWeightedSum;
/** sum of last input elements*/
double lastSum;

RBFexitNeuron::RBFexitNeuron(double weights[], neuronSize_t length){
	ws = weights;
	N = length;
}

double RBFexitNeuron::compute(double input[]){
	double lastSum = 0.0;
	double lastWeightedSum = 0.0;
	for(neuronSize_t j=0 ; j<N ; j++){
		lastSum += input[j];
		lastWeightedSum += input[j]*ws[j];
	}
	return lastWeightedSum/lastSum;
}

void RBFexitNeuron::backpropagation(double errorContribution, double lastInput[], double step, double thisContributions[]){
	double R2 = (lastSum*lastSum);
	for(neuronSize_t r=0 ; r<N ; r++){
		//modify weights
		ws[r] += step * errorContribution * lastInput[r]/lastSum;
		//compute the error contribution of this neuron to the activation fonction of the jth neuron of the previous layer
		thisContributions[r] = (ws[r]*lastSum - lastWeightedSum)/R2;
	}
	return;
}