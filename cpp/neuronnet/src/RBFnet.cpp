#include "RBFnet.hpp"
#include "RBFneuron.hpp"
#include "RBFexitNeuron.hpp"
#include "Layer.hpp"
#include <cstdlib>
#include <time.h>

void RBFnet::init(unsigned int inputSize, unsigned int outputSize, unsigned int hiddenSize, double sd, double step, double **prototypes, double **weights){
	ls = new Layer *[2];
	//create hidden neurons
    int i;
    Neuron** hiddens = new Neuron *[hiddenSize];
    for (i = 0 ; i < hiddenSize ; i++)
    {
        //create standard deviations
        int j;
        double* sds = new double[inputSize];
        for (j=0 ; j<inputSize ; j++)
        {
            sds[j] = sd;
        }
        hiddens[i] = new RBFneuron(prototypes[i], sds, inputSize);
    }
    ls[0] = new Layer(hiddens, hiddenSize, step);
    //create exit neurons
    Neuron** exits = new Neuron *[outputSize];
    for (i=0 ; i<outputSize ; i++)
    {
        exits[i] = new RBFexitNeuron(weights[i], hiddenSize);
    }
    ls[1] = new Layer(exits, outputSize, step);
}

RBFnet::RBFnet(unsigned int inputSize, unsigned int outputSize, unsigned int hiddenSize, double sd, double step, double **prototypes, double **weights)
	: Net(NULL, 2)
{
	init(inputSize, outputSize, hiddenSize, sd, step, prototypes, weights);
}

RBFnet::RBFnet(unsigned int inputSize, unsigned int outputSize, unsigned int hiddenSize, double sd, double step)
    : Net(NULL, 2)
{
	int i, j;
	srand(time(NULL));//init seed
	//create prototype
	double** prototypes = new double*[hiddenSize];
	for(i=0 ; i<hiddenSize ; i++){
		prototypes[i] = new double[inputSize];
		for(j=0 ; j<inputSize ; j++){
			prototypes[i][j] = rand() / (double)RAND_MAX;
		}
	}
	//create weights
	double** weights = new double*[outputSize];
	for(i=0 ; i<outputSize ; i++){
		weights[i] = new double[hiddenSize];
		for(j=0 ; j<hiddenSize ; j++){
			weights[i][j] = (rand() / (double)RAND_MAX)*2.0 -1.0;
		}
	}
	init(inputSize, outputSize, hiddenSize, sd, step, prototypes, weights);
}
