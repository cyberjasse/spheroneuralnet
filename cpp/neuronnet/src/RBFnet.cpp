#include "RBFnet.hpp"
#include "RBFneuron.hpp"
#include "RBFexitNeuron.hpp"
#include "Layer.hpp"
#include <cstdlib>
#include <time.h>

RBFnet::RBFnet(unsigned int inputSize, unsigned int outputSize, unsigned int hiddenSize, double step, double sd)
    : Net(NULL, 2)
{
    ls = new Layer *[2];
    //create hidden neurons
    int i;
    Neuron** hiddens = new Neuron *[hiddenSize];
    srand(time(NULL));//init seed
    for (i = 0 ; i < hiddenSize ; i++)
    {
        //create prototype and standard deviations
        int j;
        double* prototype = new double[inputSize];
        double* sds = new double[inputSize];
        for (j = 0 ; j < inputSize ; j++)
        {
            prototype[i] = rand() / (double)RAND_MAX;
            sds[i] = sd;
        }
        hiddens[i] = new RBFneuron(prototype, sds, inputSize);
    }
    ls[0] = new Layer(hiddens, hiddenSize, step);
    //create exit neurons
    Neuron** exits = new Neuron *[outputSize];
    for (i = 0 ; i < outputSize ; i++)
    {
        exits[i] = new RBFexitNeuron(hiddenSize);
    }
    ls[1] = new Layer(exits, outputSize, step);
}
