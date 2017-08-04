#include "MLPnet.hpp"
#include "MLPlayer.hpp"
#include "DerivableFunction.hpp"
#include "Sigmoid.hpp"
#include "Identity.hpp"
#include <time.h>

MLPnet::MLPnet(unsigned int inputSize, unsigned int outputSize, unsigned int hiddenSize, double step)
	: Net(NULL, 2)
{
	ls = new Layer *[2];
	DerivableFunction *sf = new Sigmoid(0.01);
	DerivableFunction *idf = new Identity;
	//create the hidden layer
	ls[0] = new MLPlayer(sf, hiddenSize, step, inputSize);
	//create the output layer
	ls[1] = new MLPlayer(idf, outputSize, step, hiddenSize);
}
