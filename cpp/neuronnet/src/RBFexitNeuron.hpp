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
		/** Number of weights*/
		neuronSize_t N;
		/** last Weighted sum*/
		double lastWeightedSum;
		/** sum of last input elements*/
		double lastSum;

	public:
		/**
		 * @param weights Initial weights
		 * @param size The length of weights
		 */
		RBFexitNeuron(double weights[], neuronSize_t lenght);

		virtual double compute(double input[]);
		virtual void backpropagation(double errorContributions[], double lastInput[], double step, double thisContributions[]);
};
#endif
