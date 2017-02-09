/**
 * A radial basis function neural network
 * @author Jason Bury
 */
#ifndef RBFNET_HPP
#define RBFNET_HPP
#include "Net.hpp"
class RBFnet : public Net{
	public:
		/**
		 * Create a RBF net with random initial prototypes
		 * @param inputSize The length of the input vector
		 * @param outputSize The length of the output vector
		 * @param hiddenSize The number of hidden neurons
		 * @param step The negative of the gradient step (-eta) for all neurons
		 * @param sd The initial standard deviation for all hidden neurons
		 */
		RBFnet(unsigned int inputSize, unsigned int outputSize, unsigned int hiddenSize, double step=0.5, double sd=0.5);
};
#endif
