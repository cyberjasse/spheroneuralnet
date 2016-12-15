/**
 * A RBF hidden neuron
 * @author Jason Bury
 */
#ifndef RBFNEURON_HPP
#define RBFNEURON_HPP
#include "Neuron.hpp"
class RBFneuron : public Neuron{
	private:
		/** The prototype*/
		double* mu;
		/** The standard deviation*/
		double* sigma;
		/** Size of the prototype*/
		neuronSize_t N;

	public:
		/**
		 * @param prototype An initial prototype. It is the mean of the gaussian
		 * @param sd An initial standard deviation
		 * @param length The dimension of prototype and sd
		 */
		RBFneuron(double prototype[], double sd[], neuronSize_t length);

		//Override
		virtual double compute(double input[]);
		virtual void backpropagation(double errorContributions[], double lastInput[], double step, double thisContributions[]);
};
#endif
