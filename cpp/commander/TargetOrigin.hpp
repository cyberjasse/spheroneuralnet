#ifndef TARGETORIGIN_HPP
#define TARGETORIGIN_HPP
#include "Target.hpp"

/**
 * A target that everytime provide the origin as target
 */
class TargetOrigin : public Target{
	public : virtual StreamFrame getTarget(struct StreamFrame *frame);
};
#endif
