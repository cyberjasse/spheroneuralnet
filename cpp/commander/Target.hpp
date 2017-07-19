#ifndef TARGET_HPP
#define TARGET_HPP
#include "../sphero/packets/async/StreamFrame.hpp"
/**
 * The interface for a class that provide target trajectory
 */
class Target{

	public :
		/**
		 * Get a frame that represent the state to reach
		 * @param frame The current state. Or NULL if its finished.
		 */
		virtual StreamFrame getTarget(struct StreamFrame *frame)=0;
	
		/**
		 * The number of remaining states to reach.
		 */
		virtual int remaining()=0;
};
#endif
