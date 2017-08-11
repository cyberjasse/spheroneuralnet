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
	if(argc<3){
		std::cerr << "Enter a filename and a number of iteration as parameter\n";
		return 1;
	}
	//Get parameters
	std::string filename = argv[1];
	int niteration = atoi( argv[2]);
	Sphero *sph;
	if(argc>3){
		const std::string DEFAULT_ADDRESS = "68:86:E7:00:58:83";
		sph = new Sphero(DEFAULT_ADDRESS.c_str(), new bluez_adaptor());
	}
	else{
		sph = NULL;
	}
	
	//Create a LearningCOmmander
	SimpleAdapter *adapter = new SimpleAdapter();
	LearningCommander *commander = new LearningCommander(sph, adapter);
	//start training
	commander->learnFromFile(filename, 0,1000000);
	//if target file, drive
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
	}
}
