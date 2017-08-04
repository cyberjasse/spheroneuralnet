#ifndef SIGMOID_HPP
#define SIGMOID_HPP
#include "DerivableFunction.hpp"
/**
 * The sigmoid differentiable function
 */
class Sigmoid : public DerivableFunction{
private :
	/** The lambda of the sigmoid function */
	double l;

public :
	Sigmoid(double lambda=1.0);
	
	/**
	 * Apply the function to x
	 */
	virtual double f(double x);
	
	/**
	 * Apply the derivate of the function to x
	 */
	virtual double df(double x);
};
#endif
