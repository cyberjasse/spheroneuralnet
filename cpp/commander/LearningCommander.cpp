#include "LearningCommander.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

LearningCommander::LearningCommander(Sphero *sph, DataAdapter *dadapter, Net *lm){
	sphero = sph;
	adapter = dadapter;
	learningmachine = lm;
	target = NULL;
}

void LearningCommander::setTarget(Target *tar){
	target = tar;
}

void LearningCommander::compute(struct TransformedFrame transformed, double *speed, double *head, bool print ){
	double input[INPUTSIZE] = { (double)(transformed.currentSpeedx),
						(double)(transformed.currentSpeedy),
						(double)(transformed.targetx),
						(double)(transformed.targety),
						(double)(transformed.currentAccelx),
						(double)(transformed.currentAccely)};/*,
						(double)(transformed.targetSpeedx),
						(double)(transformed.targetSpeedy)};*/
	
	double output[OUTPUTSIZE];
	learningmachine->compute(input, output);
	*speed = output[0];
	//*head = output[1];
	if(print){
		std::cout << "[input] ";
		for(int i=0 ; i<INPUTSIZE ; i++)
			std::cout << input[i] <<" ";
		std::cout << "[output] ";
		for(int i=0 ; i<OUTPUTSIZE ; i++)
			std::cout << output[i] <<" ";
	}
}

void LearningCommander::notify(struct StreamFrame *frame){
	if(target != NULL){
		if(target->remaining() > 0){
			// prepare input
			struct StreamFrame targetstate = target->getTarget(frame);
			struct TransformedFrame transformed = adapter->normalizeFrame(*frame, targetstate);
			double speed, head;
			compute(transformed, &speed, &head);
			uint8_t ns = adapter->denormalizeSpeed(round(speed));
			int16_t nh = adapter->denormalizeHead( round(head ));
			sphero->roll(ns, nh);
		}
		else{
			sphero->disconnect();
		}
	}
}

void LearningCommander::learnFromList(std::vector<InputOutput> l, int niteration){
	const int ratioTest = 3; //take 1 sample on 3 for the test
	double errormean; //Mean of quadratic errors
	double testmean; //Mean of test errors
	int listsize = l.size();
	double speed, head;
	double expected[OUTPUTSIZE];
	double contribution[INPUTSIZE];
	for(int iteration=0 ; iteration<niteration ; iteration++){
		bool last = iteration==niteration-1;
		errormean = 0;
		testmean = 0;
		for(int i=0 ; i<listsize-1 ; i++){
			int ip1 = i+1;
			struct TransformedFrame tframe = adapter->normalizeFrame( l[i].frame, l[ip1].frame);
			compute(tframe, &speed, &head,last);
			expected[0] = adapter->normalizeSpeed(l[i].speedCommand);
			//expected[1] = adapter->normalizeHead( l[i].headCommand);
			if( false){//(ip1)%3 == 0){
				//take the sample for the test
				int deltas = round(speed)-expected[0];
				int deltah = 0;//round(head) -expected[1];
				testmean += (deltas*deltas + deltah*deltah)/ (2 * (listsize/3));
			}
			else{
				//take the sample for the training
				if(last)std::cout <<"[expected] "<<expected[0] << std::endl;
				errormean += learningmachine->backpropagation(expected, contribution) / listsize;
			}
		}
		//std::cout << errormean << " " << testmean << std::endl;
	}
}

void LearningCommander::learnFromFile(std::string filename, int niteration, int timemin, int timemax){
	std::vector<struct InputOutput> l = std::vector<struct InputOutput>();
	std::ifstream file;
	file.open(filename);
	const int linesize = 10;
	std::string garb;
	// read the first line containing column names
	for(int i=0 ; i<linesize ; i++){
		file >> garb;
	}
	// read line by line
	int speed;
	int head;
	bool still = true;
	while(still){
		struct StreamFrame frame;
		if(file >> frame.yaw){
			file >> frame.x >> frame.y >> frame.speedx >> frame.speedy >> frame.ax >>frame.ay >> frame.chrono;
			file >> speed >> head;
			//std::cout<<"[DEBUG LearningCommander.cpp]"<<frame.x<<" "<<frame.y<<" "<<frame.speedx<<" "<<frame.speedy<<" "<<frame.chrono<<" "<<speed<<" "<<head<< std::endl;
			if(frame.chrono>=timemin && frame.chrono<=timemax){
				struct InputOutput io = {frame,(uint8_t)(speed),(int16_t)(head)};
				l.push_back(io);
			}
		}
		else{
			break;
		}
	}
	learnFromList(l, niteration);
}
