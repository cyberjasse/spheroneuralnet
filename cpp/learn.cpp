/**
 * A script to test the machine learning on a sphero input-output file
 */
#include "commander/LearningCommander.hpp"
#include "commander/SimpleAdapter.hpp"
#include "neuronnet/src/RBFnet.hpp"
#include <iostream>
#include <stdlib.h>

int main(int argc, char* argv[]){
	//test if parameters are here
	if(argc<3){
		std::cerr << "Enter a filename and a number of iteration as parameter\n";
		return 1;
	}
	//Get parameters
	std::string filename = argv[1];
	int niteration = atoi( argv[2]);
	//Create a LearningCOmmander
	SimpleAdapter *adapter = new SimpleAdapter();
	RBFnet *net = new RBFnet(6,2, 30, 250, -0.4);
	LearningCommander commander = LearningCommander(NULL, adapter, net);
	//start training
	commander.learnFromFile(filename, niteration);
}
