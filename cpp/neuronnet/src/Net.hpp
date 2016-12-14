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
		Layer layers[];
		/** The last output generated*/
		double lastOutput[];

	public:
		/**
		 * @param layers Layer in this feedforward neural network, in order. The exit layer have to be the last layer. Don't include an entry layer
		 */
		Net(Layer layers[]);

		/**
		 * @brief Generate a solution
		 * @param input The input
		 * @return The output
		 */
		double[] compute(double input[]);

		/**
		 * @brief Perform the backpropagation
		 * @param errorContributions The error contribution of the last output generated
		 * @return The error contribution of the first layer
		 */
		double[] backpropagation(double errorContributions[]);
}
#endif//NET_HPP
