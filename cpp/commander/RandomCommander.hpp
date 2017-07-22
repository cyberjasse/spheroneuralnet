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
 * A stream observer that send random command to the sphero. Not following the received frame.
 */
class RandomCommander : public StreamObserver{
	private :
		/** The current speed ordered */
		int cv;
		/** The bounds of the map */
		struct Bounds *bds;
		
		/** The file to write the input-output */
		ofstream file;
		/** A pointer to the sphero to command */
		Sphero *sph;
		
	public :
		/**
		 * @param bounds The bounds of the map. Set it to NULL for an infinite map.
		 */
		RandomCommander(Sphero *sphero, std::string filename, struct Bounds *bounds = NULL);
		
		virtual void notify(struct StreamFrame *frame);
};
#endif
