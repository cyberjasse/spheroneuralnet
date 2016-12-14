/**
 * A neuron
 * @author Jason Bury
 */
#ifndef NEURON_HPP
#define NEURON_HPP
typedef unsigned short neuronSize_t;
class Neuron{
	public:
		/**
		 * @brief Generate an output
		 * @param input The input
		 * @return The value of the activation function
		 */
		virtual double compute(double input[]) =0;

		/**
		 * @brief Perform the backpropagation
		 * @param errorContributions The error contributions of the next layer
		 * @param lastInput The last input
		 * @param step The gradient step but in negative (-eta).
		 * @param thisContributions The table to store contribution of this neuron to activation functions of all neurons in previous layer.
		 */
		virtual void backpropagation(double errorContributions[], double lastInput[], double step, double thisContributions[]) =0;
};
#endif
