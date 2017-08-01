#ifndef AHEADCOMMANDER_HPP
#define AHEADCOMMANDER_HPP
#include "../sphero/Sphero.hpp"
#include "../sphero/packets/async/StreamObserver.hpp"
#include <iostream>
#include <fstream>

class AheadCommander : public StreamObserver{
	private :
		Sphero * sphero;
		ofstream file;
	public :
		AheadCommander(Sphero *sph, std::string filename);
		
		virtual void notify(struct StreamFrame *frame);
};
#endif
