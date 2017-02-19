/**
 * A neuron
 * @author Jason Bury
 */
#ifndef NEURON_HPP
#define NEURON_HPP
typedef unsigned short neuronSize_t;
class Neuron{
	public:

		/** The dimension of the input*/
		neuronSize_t N; //the final declaration is available in c++11

		/**
		 * @brief Generate an output
		 * @param input The input
		 * @return The value of the activation function
		 */
		virtual double compute(double input[]) =0;

		/**
		 * @brief Perform the backpropagation
		 * @param errorContribution The error contribution of the activation function of this neuron.
		 *  Let f its activation function, let Q the quadratic error, errorContribution = the partial derivate dQ/df
		 * @param lastInput The last input
		 * @param step The gradient step but in negative (-eta).
		 * @param thisContributions The table to store contribution of this neuron to activation functions of all neurons in previous layer.
		 *  Let f the activation function of this neuron.
		 *  Lef g[i] the activation function of the neuron i, a neuron in the previous layer.
		 *  Let Q the quadratic error, thisContribution[i] = the partial derivate dQ/df * df/dg[i]
		 */
		virtual void backpropagation(double errorContribution, double lastInput[], double step, double thisContributions[]) =0;
		
		#ifdef NEUROPRINT
		/**
		 * @brief Print current parameters
		 */
		virtual void neuroprint() =0;
		#endif
};
#endif
