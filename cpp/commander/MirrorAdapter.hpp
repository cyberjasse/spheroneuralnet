#ifndef MIRRORADAPTER_HPP
#define MIRRORADAPTER_HPP
#include "SimpleAdapter.hpp"

/**
 * Apply SimpleAdapter then mirror with the x axis if the targety is negative.
 */
class MirrorAdapter : public SimpleAdapter{
private :
	bool mirroring;

public :
	//override
	virtual TransformedFrame normalizeFrame(struct StreamFrame currentstate, struct StreamFrame targetstate);

	virtual int normalizeHead(int16_t head);
	
	virtual int16_t denormalizeHead(int head);
};
#endif
