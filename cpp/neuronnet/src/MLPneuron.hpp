#ifndef MLPNEURON_HPP
#define MLPNEURON_HPP
#include "Neuron.hpp"
#include "DerivableFunction.hpp"

/**
 * A Neuron for a multi-layer perceptron.
 * You can provide a derivable function
 * Dont forget to seed via srand()
 * Does not support bias for the moment
 */
class MLPneuron : public Neuron{
private:
	/** weights*/
	double* ws;
	/** The derivable function */
	DerivableFunction *function;
	
	double lastWeightedSum;

public :
	
	/**
	 * Initialize weights randomly
	 * @param length The number of input
	 */
	MLPneuron(DerivableFunction *f, neuronSize_t length);
	
	//Override
	virtual double compute(double input[]);
	virtual void backpropagation(double errorContribution, double lastInput[], double step, double thisContributions[]);
	#ifdef NEUROPRINT
	virtual void neuroprint();
	#endif
};
#endif
