#ifndef ORIGINADAPTER_HPP
#define  ORIGINADAPTER_HPP
#include "DataAdapter.hpp"
#include <fstream>
/**
 * This adapter give the target position relative to the current position and rotate all to align the sphero direction to the x axis.
 * Also, save the received data frame before transformation and save the commands before the transformation of commands.
 */
class OriginAdapter : public DataAdapter{
	private :
		Commander *cmdr;
		Sphero *sph;
		Target *tar;
		bool isFileOpened;
		ofstream file;
	public :
		OriginAdapter(Sphero *sphero, Commander *commander, Target *target, std::string filename="");
		virtual void notify(struct StreamFrame *frame);
};
#endif
