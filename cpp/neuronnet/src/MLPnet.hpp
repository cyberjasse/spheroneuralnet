#ifndef MLPNET_HPP
#define MLPNET_HPP
#include "Net.hpp"

/**
 * Multi-layer perceptron feed forward
 */
class MLPnet : public Net{
public :
	/**
	 * Create a RBF net with random initial prototypes
	 * @param inputSize The length of the input vector
	 * @param outputSize The length of the output vector
	 * @param hiddenSize The number of hidden neurons
	 * @param step The negative of the gradient step (-eta) for all neurons
	 */
	MLPnet(unsigned int inputSize, unsigned int outputSize, unsigned int hiddenSize, double step);
};
#endif
