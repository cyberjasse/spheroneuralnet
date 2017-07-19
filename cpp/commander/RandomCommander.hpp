#ifndef RANDOMCOMMANDER_HPP
#define RANDOMCOMMANDER_HPP
#include "../sphero/Sphero.hpp"
#include "Commander.hpp"
#include "DataAdapter.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
/**
 * A stream observer that send random command to the sphero. Not following the received frame
 */
class RandomCommander : public Commander{
	private :
		int16_t cyaw;
		uint8_t cv;
		Sphero *s;
		ofstream file:
	public :
		/**
		 * @param sphero A pointer to the sphero object.
		 * @param filename The name of the file to write data collected and command. Set to NULL for no recording.
		 */
		RandomCommander(Sphero *sphero, std::string filename);
		virtual ~RandomCommander();
		virtual void notify(struct TransformedFrame *frame);
};
#endif
