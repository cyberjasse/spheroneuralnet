#include "Net.hpp"
#include <iostream>
#include <stdlib.h>
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
if(layerIn[i] < -100000 || layerIn[i] > 100000){
	std::cerr << "[Net] layerIn[i] = "<<layerIn[i]<<std::endl;
	exit (EXIT_FAILURE);
}
		error += delta*delta;//for Q
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

neuronSize_t Net::getInputSize(){
	return ls[0]->ns[0]->N;
}

layerSize_t Net::getOutputSize(){
	return ls[nLayers-1]->size;
}

#ifdef NEUROPRINT
#include <iostream>
void Net::neuroprint(){
	for(int i=0 ; i<nLayers ; i++){
		if(i == 0){
			std::cout << "ENTRY LAYER  (LAYER 0):\n";
		}
		else{
			std::cout << "HIDDEN LAYER (LAYER " << i << ");\n";
		}
		ls[i]->neuroprint();
	}
}
#endif
