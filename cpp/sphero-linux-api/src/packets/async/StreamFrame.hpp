#ifndef STREAMFRAME_HPP
#define STREAMFRAME_HPP
#include <cstdint>
/**
 * A frame of data that can be received by streaming
 */
struct StreamFrame{
	/** The yaw of the sphero */
	int16_t yaw;

	/** The x coordinate following sphero's odometer */
	int16_t x;

	/** The y coordinate following sphero's odometer */
	int16_t y;

	/** The speed projected to the x axis*/
	int16_t speedx;

	/** The speed projected to the y axis*/
	int16_t speedy;

	/** The time in micro second between this frame and the previous frame */
	unsigned long chrono;	
};
#endif
