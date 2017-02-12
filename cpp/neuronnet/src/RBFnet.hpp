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
		RBFnet(unsigned int inputSize, unsigned int outputSize, unsigned int hiddenSize, double sd=5.0, double step=0.5);

		/**
		 * Create a RBF net providing prototypes and weights
		 * @param inputSize The length of the input vector
		 * @param outputSize The length of the output vector
		 * @param hiddenSize The number of hidden neurons
		 * @param step The negative of the gradient step (-eta) for all neurons
		 * @param sd The initial standard deviation for all hidden neurons
		 * @param prototypes Initial prototypes
		 * @param weights Initial weights
		 */
		 RBFnet(unsigned int inputSize, unsigned int outputSize, unsigned int hiddenSize, double sd, double step, double **prototypes, double **weights);

	private:
		/**
		 * Common code shared by constructor
		 */
		void init(unsigned int inputSize, unsigned int outputSize, unsigned int hiddenSize, double sd, double step, double **prototypes, double **weights);
};
#endif
