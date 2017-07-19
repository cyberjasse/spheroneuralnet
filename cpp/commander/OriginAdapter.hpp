#ifndef ORIGINADAPTER_HPP
#define  ORIGINADAPTER_HPP
#include "DataAdapter.hpp"

/**
 * This adapter give the target position relative to the current position and rotate all to align the sphero direction to the x axis.
 */
class OriginAdapter : public DataAdapter{
	public :
		virtual void notify(struct StreamFrame *frame);
};
#endif
