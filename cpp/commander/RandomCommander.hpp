#ifndef RANDOMCOMMANDER_HPP
#define RANDOMCOMMANDER_HPP
#include "../sphero/Sphero.hpp"
#include "TransformedFrame.hpp"
#include "Commander.hpp"
#include <fstream>
#include <iostream>
#include <cstdint>
/**
 * A stream observer that send random command to the sphero. Not following the received frame
 */
class RandomCommander : public Commander{
	private :
		int16_t cyaw;
		int cv;
	public :
		RandomCommander();
		virtual Command getCommand(struct TransformedFrame *frame);
};
#endif
