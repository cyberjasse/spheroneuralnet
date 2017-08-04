#include "commander/DataAdapter.hpp"
#include "commander/TransformedFrame.hpp"
#include "sphero/packets/async/StreamFrame.hpp"
#include "commander/LearningCommander.hpp"
#include <iostream>
#include <fstream>
#include "commander/SimpleAdapter.hpp"

int main(int argc, char *argv[]){
	DataAdapter *adapter = new SimpleAdapter;

	//test if parameters are here
	if(argc<3){
		std::cerr << "Enter 2 filename as parameter\n";
		return 1;
	}
	//Get parameters
	std::string inputfilename = argv[1];
	std::string outputfilename = argv[2];
	ifstream input;
	input.open(inputfilename);
	ofstream output;
	output.open(outputfilename);
	// read the first line containing column names
	const int linesize = 10;
	std::string garb;
	for(int i=0 ; i<linesize ; i++){
		input >> garb;
	}
	// read line by line
	std::vector<struct InputOutput> l = std::vector<struct InputOutput>();
	const unsigned long TIMEMIN = 0;
	const unsigned long TIMEMAX = 1000000;
	int speed;
	int head;
	bool still = true;
	while(still){
		struct StreamFrame frame;
		if(input >> frame.yaw){
			input >> frame.x >> frame.y >> frame.speedx >> frame.speedy >> frame.ax >> frame.ay >> frame.chrono;
			input >> speed >> head;
			if(frame.chrono>=TIMEMIN && frame.chrono<=TIMEMAX){//filter
				struct InputOutput io = {frame,(uint8_t)(speed),(int16_t)(head)};
				l.push_back(io);
			}
		}
		else{
			break;
		}
	}
	input.close();
	output << "@relation streaming\n";
	output << "@attribute currentSpeedx numeric\n";
	output << "@attribute currentSpeedy numeric\n";
	output << "@attribute targetx numeric\n";
	output << "@attribute targety numeric\n";
	output << "@attribute accelx numeric\n";
	output << "@attribute accely numeric\n";
	output << "@attribute speed numeric\n";
	output << "@attribute head numeric\n";
	output << "@data\n";
	for(int i=0 ; i<l.size()-1 ; i++){
		struct TransformedFrame tframe = adapter->normalizeFrame( l[i].frame, l[i+1].frame);
		speed = adapter->normalizeSpeed(l[i].speedCommand);
		head = adapter->normalizeHead( l[i].headCommand);
		// WRITE HERE
		output << tframe.currentSpeedx <<","<< tframe.currentSpeedy <<","<< tframe.targetx <<","<< tframe.targety <<","<< tframe.currentAccelx <<","<< tframe.currentAccely<<","
		<< speed <<","<< head << std::endl;
	}
	output.close();
}
