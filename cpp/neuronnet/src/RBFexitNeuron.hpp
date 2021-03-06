/**
 * A neuron for the exit layer of a RBF
 * @author Jason Bury
 */
#ifndef RBFEXITNEURON_HPP
#define RBFEXITNEURON_HPP
#include "Neuron.hpp"
class RBFexitNeuron : public Neuron{
	private:
		/** weights*/
		double* ws;

	public:
		/**
		 * @param weights Initial weights
		 * @param length The number of weights
		 */
		RBFexitNeuron(double weights[], neuronSize_t length);

		/**
		 * Create with random initial weights between -1 and 1
		 * @param weights The number of weights
		 */
		RBFexitNeuron(neuronSize_t length);

		//Override
		virtual double compute(double input[]);
		virtual void backpropagation(double errorContribution, double lastInput[], double step, double thisContributions[]);
		#ifdef NEUROPRINT
		virtual void neuroprint();
		#endif
};
#endif
