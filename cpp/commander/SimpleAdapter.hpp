#ifndef SIMPLEADAPTER_HPP
#define SIMPLEADAPTER_HPP
#include "DataAdapter.hpp"

/**
 * Transform all point to have positions relative to the current position.
 * Then apply rotation to have the sphero yaw aligned with the x axis.
 */
class SimpleAdapter : public DataAdapter{
	private :
		/** Last rotation angle performed */
		int lastangle;
	
	public :
	
		virtual TransformedFrame normalizeFrame(struct StreamFrame currentstate, struct StreamFrame targetstate);
		
		virtual int normalizeSpeed(uint8_t speed);
		
		virtual int normalizeHead(int16_t head);
		
		virtual uint8_t denormalizeSpeed(int speed);
		
		virtual int16_t denormalizeHead(int head);
};
#endif
