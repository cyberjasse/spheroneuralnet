/**
 * Neurons send their output to all neurons in the next layer
 * @author Jason Bury
 */
#ifndef LAYER_HPP
#define LAYER_HPP
#include "Neuron.hpp"

class Layer{
	protected:
		/** The last input vector*/
		double lastInput[];
		
	public:
		/** Neurons in this layer*/
		final Neuron neurons[];
		/** The number of neurons*/
		final uint16_t size;

		/**
		 * @param neurons all neurons in this layer
		 */
		Layer(Neuron neurons[]);

		/**
		 * Compute the activation function off all neurons
		 * @param input The input for all neurons
		 * @return The output of all neurons
		 */
		double[] compute(double input[]);

		/**
		 * @brief Perform the backpropagation throught this layer.
		 * @param nextLayerContributions The error contributions from the next layer
		 * @return The error contributions of this layer
		 */
		double[] backpropagation(double nextLayerContributions[]);

		/**
		 * @brief Get the last input vector given in compute()
		 */
		double[] getLastInput();
}
#endif
