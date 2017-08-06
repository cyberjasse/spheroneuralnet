#include "NormalizeNet.hpp"

NormalizeNet::NormalizeNet(Layer **layers, unsigned int size, double *inputMeans, double *inputStandardDeviation, double *outputMeans, double *outputStandardDeviation)
: Net(layers, size){
	double *inputMu = inputMeans;
	double *inputSd = inputStandardDeviation;
	double *outputMu = outputMeans;
	double *outputSd = outputStandardDeviation;
}

void NormalizeNet::compute(double input[], double output[]){
	//normalize inputs
	for(int i=0 ; i<this->getInputSize() ; i++){
		input[i] = (input[i]-inputMu[i])/inputSd[i];
	}
	Net::compute(input, output);
	//denormalize outputs
	for(int i=0 ; i<this->getOutputSize() ; i++){
		output[i] = (output[i]*outputSd[i]) + outputMu[i];
	}
}

double NormalizeNet::backpropagation(double expected[], double thisContributions[]){
	//normalize expected
	for(int i=0 ; i<this->getInputSize() ; i++){
		expected[i] = (expected[i]-outputMu[i])/outputSd[i];
	}
	Net::backpropagation(expected, thisContributions);
}
