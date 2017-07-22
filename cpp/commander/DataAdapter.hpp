#ifndef DATAADAPTER_HPP
#define DATAADAPTER_HPP
#include "../sphero/packets/async/StreamFrame.hpp"
#include "TransformedFrame.hpp"
/**
 * An interface class for adapter that can transform a StreamFrame combined with a target frame to get a frame to use in a commander.
 * The frame it returns is a TransformedFrame
 */
class DataAdapter{
	public :
	
		/**
		 * Adapt the input StreamFrame and target frame and summarize it with a TransformedFrame
		 */
		virtual TransformedFrame normalizeFrame(struct StreamFrame currentstate, struct StreamFrame targetstate)=0;
		
		/**
		 * Adapt a speed value.
		 * @param speed Not a physical speed but a motor speed, between 0 and 255.
		 * Like a speed we can command to the sphero.
		 */
		virtual int normalizeSpeed(uint8_t speed)=0;
		
		/**
		 * Adapt a head value.
		 * @param head The direction of the sphero in degrees between 0 and 359.
		 * 0 degree = ahead
		 * Like a direction we can command to the sphero.
		 */
		virtual int normalizeHead(int16_t head)=0;
		
		/**
		 * Apply the inverse transformation of the speed from a commander to give a value to send to the sphero.
		 */
		virtual uint8_t denormalizeSpeed(int speed)=0;
		
		/**
		 * Apply the inverse transformation of the head from a commander to give a value to send to the sphero
		 */
		virtual int16_t denormalizeHead(int head)=0;
		
		/**
		 * Aply a rotation to a point around (0,0)
		 * @param x The x coordinate of the point
		 * @param y The y coordinate of the point
		 * @param angle The rotation angle. in radian
		 */
		static void rotation(int16_t *x, int16_t *y, double angle);

		/**
		 * Convert an angle in degree to radian
		 */
		static double degreeToRadian(int16_t angle);

		/**
 		* If the angle is <-179 or >180, it returns an angle between -179 and 180.
 		*/
		static int16_t correctAngle(int16_t angle);
};
#endif
