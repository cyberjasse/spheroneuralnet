#ifndef NORMALIZENET_HPP
#define NORMALIZENET_HPP
#include "Net.hpp"

/**
 * a Net that normalize data before computing and then denormalize
 */
class NormalizeNet : public Net{
private :
	/** value to center the inputs */
	double *inputMu;
	/** value to reduct the inputs */
	double *inputSd;
	/** value to center the outputs */
	double *outputMu;
	/** value to reduct the outputs */
	double *outputSd;

public :
	NormalizeNet(Layer **layers, unsigned int size, double *inputMeans, double *inputStandarDeviation, double *outputMeans, double *outputStandardDeviation);
	
	//override
	virtual void compute(double input[], double output[]);

	virtual double backpropagation(double expected[], double thisContributions[]);
};
#endif
