/**
 * A script to test the machine learning on a sphero input-output file
 */
#include "commander/LearningCommander.hpp"
#include "commander/SimpleAdapter.hpp"
#include "commander/PointlistTarget.hpp"
#include <iostream>
#include <stdlib.h>
#include "sphero/bluetooth/bluez_adaptor.h"
#include "sphero/Sphero.hpp"

int main(int argc, char* argv[]){
	//test if parameters are here
	if(argc<2){
		std::cerr << "Enter 1 or several filenames\n";
		return 1;
	}
	//Get parameters
	std::vector<std::string> filenames;
	for(int i=1 ; i<argc ; i++){
		filenames.push_back( argv[i]);
	}
	Sphero *sph;
	const std::string DEFAULT_ADDRESS = "68:86:E7:00:58:83";
	sph = new Sphero(DEFAULT_ADDRESS.c_str(), new bluez_adaptor());
	
	//Create a LearningCOmmander
	SimpleAdapter *adapter = new SimpleAdapter();
	LearningCommander *commander = new LearningCommander(sph, adapter);
	//start training
	commander->learnFromFiles(filenames, 0,1000000);
	/*if target file, drive
	if(argc>3){
		std::cerr << "training finished. Driving...\n";
		std::string targetname = argv[3];
		PointlistTarget *target = new PointlistTarget(targetname);
		commander->setTarget(target);
		if(sph->connect()){
			std::cerr << " >connect() == true< " << std::endl;
		}
		else{
			std::cerr << " >connection failed< " << std::endl;
			delete sph;
			return 1;
		}
		const short frequency  = 5;
		sph->addStreamObserver(commander);
		sph->startStream(frequency);
	}*/
}
