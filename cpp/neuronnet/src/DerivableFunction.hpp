#ifndef DERIVABLEFUNCTION
#define DERIVABLEFUNCTION
/**
 * A differentiable function
 */

class DerivableFunction{
public :
	/**
	 * Apply the function to x
	 */
	virtual double f(double x)=0;
	
	/**
	 * Apply the derivate of the function to x
	 */
	virtual double df(double x)=0;
};
#endif
