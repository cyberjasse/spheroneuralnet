/**
 * Neurons send their output to all neurons in the next layer
 * @author Jason Bury
 */
#ifndef LAYER_HPP
#define LAYER_HPP
#include "Neuron.hpp"
typedef  unsigned int layerSize_t;
class Layer{
	protected:
		/** The last input vector*/
		double* lastInput;
		/** The negative of the gradient step*/
		double meta;
		
	public:
		/** Neurons in this layer*/
		Neuron** ns;
		/** The number of neurons*/
		layerSize_t size;

		/**
		 * @param neurons all neurons in this layer
		 * @param length The number of neurons
		 * @param step The negative of the gradient step (-eta)
		 */
		Layer(Neuron** neurons, layerSize_t length, double step);

		/**
		 * Compute the activation function off all neurons
		 * @param input The input for all neurons
		 * @param output The table where the output for all neurons will be stored
		 */
		void compute(double input[], double output[]);

		/**
		 * @brief Perform the backpropagation throught this layer.
		 * @param nextLayerContributions The error contributions from the next layer
		 *  Let Q the quadratic error, let f[i] the activation function of the ith neurons of this layer,
		 *  nextLayerContributions = the partial derivate dQ/df[i]
		 * @return The error contributions of this layer.
		 *  Let Q the quadratic error, let f[j] the activation function of the jth neurons of the previous layer,
		 *  the return = the partial derivate dQ/df[j]
		 */
		void backpropagation(double nextLayerContributions[], double thisContributions[]);

		/**
		 * @brief Get the last input vector given in compute()
		 */
		double* getLastInput();
};
#endif
