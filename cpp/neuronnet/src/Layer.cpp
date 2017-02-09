#include "Layer.hpp"
#include <iostream>

Layer::Layer(Neuron** neurons, layerSize_t length, double step){
	ns = neurons;
	size = length;
	meta = step;
	lastInput = new double[length];
}

void Layer::compute(double input[], double output[]){
	for(layerSize_t i=0 ; i<size ; i++){
		output[i] = ns[i]->compute(input);//compute
		lastInput[i] = input[i];//copy the input
	}
}

void Layer::backpropagation(double nextLayerContributions[], double thisContributions[]){
		//start with all thisContributions to 0
		for(neuronSize_t j=0 ; j<ns[0]->N ; j++){
			thisContributions[j] = 0.0;
		}
		//
		double contribs[ns[0]->N];
		for(layerSize_t i=0 ; i<size ; i++){
			ns[i]->backpropagation( nextLayerContributions[i], lastInput, meta, contribs);
			for(neuronSize_t j=0 ; j<ns[0]->N ; j++){
				thisContributions[j] += contribs[j];
			}
		}
}
