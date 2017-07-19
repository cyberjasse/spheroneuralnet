#include "DataAdapter.hpp"

DataAdapter::DataAdapter(Sphero *sphero, Commander *commander, Target *target){
	sph = sphero;
	cmdr = commander;
	tar = target;
}
