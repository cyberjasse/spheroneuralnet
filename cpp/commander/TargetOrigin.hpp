#ifndef TARGETORIGIN_HPP
#define TARGETORIGIN_HPP
#include "Target.hpp"

/**
 * A target that everytime provide the origin as target
 */
class TargetOrigin : public Target{
	private :
		int l;
	
	public :
		virtual StreamFrame getTarget(struct StreamFrame *frame);
		
		/**
		 * @param limit The number of command to send. Set to 0 if no limit.
		 */
		TargetOrigin(int limit=0);
};
#endif
