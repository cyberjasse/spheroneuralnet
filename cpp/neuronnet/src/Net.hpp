/**
 * A feedforward artificial neural network
 * @author Jason Bury
 */

#ifndef NET_HPP
#define NET_HPP
#include "Layer.hpp"
class Net{
	protected:
		/** The last layer should be the exit layer*/
		Layer** ls;
		/** The last output generated*/
		double* lastOutput;
		/** The number of layers*/
		unsigned int nLayers;

	public:
		/**
		 * @param layers Layer in this feedforward neural network, in order. The exit layer have to be the last layer. Don't include an entry layer
		 * @param size The number of layers
		 */
		Net(Layer **layers, unsigned int size);

		/**
		 * @brief Generate a solution
		 * @param input The input
		 * @param output The table where it will be stored the output
		 */
		virtual void compute(double input[], double output[]);

		/**
		 * @brief Perform the backpropagation
		 * @param expected The output expected from the last solution
		 * @param thisContributions The error contributions of the first layer
		 *  Let Q the quadratic error, Let x[i] the ith element of the input.
		 *  thisContributions[i] = the parital derivate dQ/dx[i]
		 * @return The quadratic error Q
		 */
		virtual double backpropagation(double expected[], double thisContributions[]);
		
		#ifdef NEUROPRINT
		/**
		 * @brief print current neuron parameters
		 */
		virtual void neuroprint();
		#endif
};
#endif//NET_HPP
