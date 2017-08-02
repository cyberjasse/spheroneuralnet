#ifndef RANDOMCOMMANDER_HPP
#define RANDOMCOMMANDER_HPP
#include "../sphero/Sphero.hpp"
#include "../sphero/packets/async/StreamObserver.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
/**
 * The bounds of the map. 1 unit is 1 centimeter
 */
struct Bounds{
	int left;
	int right;
	int top;
	int bottom;
};

/**
 * The position of the sphero. Is it in ou out of the map ?
 */
enum Status{
	IN, // in the map, 
	LEFT, // out of the map, on the left
	RIGHT, // out of the map, on the right
	TOP, // out of the map, on the top
	BOTTOM, // out of the map, on the bottom
};

/**
 * A stream observer that send random command to the sphero. Not following the received frame.
 */
class RandomCommander : public StreamObserver{
	private :
		/** The current speed ordered */
		int cv;
		/** The current head ordered */
		int chead;
		/** The targeted yaw */
		int targetYaw;
		/** The number of steps to reach targeted yaw */
		int yawNsteps;
		/** A step to reach the targeted yaw */
		int yawstep;
		/** The targeted speed */
		int targetSpeed;
		/** The number of steps to reach targeted speed */
		int speedNsteps;
		/** A step to reach the targeted speed */
		int speedstep;
		/** The bounds of the map */
		struct Bounds *bds;
		
		/** The file to write the input-output */
		ofstream file;
		/** A pointer to the sphero to command */
		Sphero *sph;
		
		/** The status of the sphero */
		char status;
		
		/**
		 * get a value R such as |x-R| < epsilon
		 */
		int getnear(int x, int epsilon);
		
		/**
		 * Set an angle target
		 * @param target The target angle. Between 0 and 359 degrees
		 * @param currentYaw The current yaw.
		 * @param minNsteps The minimal number of steps to achieve the goal
		 * @param maxNsteps The maximal number of steps to achieve the goal
		 */
		void setYawTarget(int target, int currentYaw, int minNsteps, int maxNsteps);
		
		/**
		 * Set a speed target
		 * @param target The speed targeted. As an unsigned 8-bits
		 * @param currentSpeed The current speed
		 * @param minNsteps The minimal number of steps to reach the targeted speed
		 * @param maxNsteps The maximal number of steps to reach the targeted speed
		 */
		void setSpeedTarget(int target, int currentSpeed, int minNsteps, int maxNsteps);
		
		/**
		 * With all those private arguments, generate command and send it
		 */
		void roll();
		
	public :
		/**
		 * @param bounds The bounds of the map. Set it to NULL for an infinite map.
		 */
		RandomCommander(Sphero *sphero, std::string filename, struct Bounds *bounds = NULL);
		
		virtual void notify(struct StreamFrame *frame);
};
#endif
