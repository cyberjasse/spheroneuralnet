#ifndef IDENTITY_HPP
#define IDENTITY_HPP
#include "DerivableFunction.hpp"

/**
 * The identity function
 */
class Identity : public DerivableFunction{
public :
	//override
	virtual double f(double x);
	virtual double df(double x);
};
#endif
