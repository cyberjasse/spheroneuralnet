#ifndef NOROTATIONADAPTER_HPP
#define NOROTATIONADAPTER_HPP
#include "DataAdapter.hpp"

/**
 * Just transform the target position relative to the current.
 * This class is usefull only for data investigation
 */
class NoRotationAdapter : public DataAdapter{
public :
	virtual TransformedFrame normalizeFrame(struct StreamFrame currentstate, struct StreamFrame targetstate);

	virtual int normalizeSpeed(uint8_t speed);

	virtual int normalizeHead(int16_t head);

	virtual uint8_t denormalizeSpeed(int speed);

	virtual int16_t denormalizeHead(int head);
};

#endif
