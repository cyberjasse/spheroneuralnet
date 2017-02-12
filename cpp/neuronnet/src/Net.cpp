#include "Net.hpp"
#include <iostream>
Net::Net(Layer **layers, unsigned int size){
	ls = layers;
	nLayers = size;
}

void Net::compute(double input[], double output[]){
	double* layerIn = input;
	double* layerOut;
	int l;
	for(l=0 ; l<nLayers ; l++){
		layerOut = new double[ls[l]->size];
		ls[l]->compute(layerIn, layerOut);
		layerIn=layerOut;
	}
	lastOutput = layerOut;
	for(l=0 ; l<ls[nLayers-1]->size ; l++){
		output[l] = lastOutput[l];
	}
}

double Net::backpropagation(double expected[], double thisContributions[]){
	//compute error
	layerSize_t errorSize = ls[nLayers-1]->size;
	double* layerIn = new double[errorSize];
	double error = 0.0;
	for(int i=0 ; i<errorSize ; i++){
		double delta = lastOutput[i]-expected[i];
		layerIn[i] = delta;//for the dQ/df[i] where i is an exit neuron
		error += delta*delta;//fot Q
	}
	//backpropagation
	double* layerOut;
	for(int l=nLayers-1 ; l>=0 ; l--){
		layerOut = new double[ls[l]->ns[0]->N];
		ls[l]->backpropagation(layerIn, layerOut);
		layerIn=layerOut;
	}
	thisContributions = layerIn;
	return error/2;
}
