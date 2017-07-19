#ifndef DATAADAPTER_HPP
#define DATAADAPTER_HPP
#include "../sphero/Sphero.hpp"
#include "../sphero/packets/async/StreamObserver.hpp"
#include "TransformedFrame.hpp"
#include "Commander.hpp"
#include "Target.hpp"
#include <cstdint>
#include <math.h>

/**
 * An interface class for StreamObserver that will transform received data to send to an commander.
 * It also transform commands from the commander following the transformation.
 */
class DataAdapter : public StreamObserver{
	public :
		DataAdapter();
		DataAdapter(Sphero *sphero, Commander *commander, Target *target);
		
		/**
		 * Aply a rotation to a point around (0,0)
		 * @param x The x coordinate of the point
		 * @param y The y coordinate of the point
		 * @param angle The rotation angle. in degree
		 */
		static void rotation(int16_t *x, int16_t *y, int16_t angle);

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
