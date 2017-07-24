#include "LearningCommander.hpp"
#include <iostream>
#include <fstream>

LearningCommander::LearningCommander(Sphero *sph, DataAdapter *dadapter, Net *lm){
	sphero = sph;
	adapter = dadapter;
	learningmachine = lm;
	target = NULL;
}

void LearningCommander::setTarget(Target *tar){
	target = tar;
}

void LearningCommander::compute(struct TransformedFrame transformed, double *speed, double *head ){
	double input[6] = { (double)(transformed.currentSpeedx),
						(double)(transformed.currentSpeedy),
						(double)(transformed.targetx),
						(double)(transformed.targety),
						(double)(transformed.targetSpeedx),
						(double)(transformed.targetSpeedy)};
	double output[2];
	learningmachine->compute(input, output);
	*speed = output[0];
	*head = output[1];
}

void LearningCommander::notify(struct StreamFrame *frame){
	/*if(target != NULL){
		// prepare input
		struct StreamFrame targetstate = target->getTarget(frame);
		struct TransformedFrame transformed = adapter->adaptFrame(frame, &targetstate);
		
	 TODO	
	}*/
}

void LearningCommander::learnFromList(std::vector<InputOutput> l, int niteration){
	const int ratioTest = 3; //take 1 sample on 3 for the test
	double errormean; //Mean of quadratic errors
	double testmean; //Mean of test errors
	int listsize = l.size();
	double speed, head;
	double expected[2];
	double contribution[6];
	for(int iteration=0 ; iteration<niteration ; iteration++){
		errormean = 0;
		testmean = 0;
		for(int i=0 ; i<listsize-1 ; i++){
			int ip1 = i+1;
			struct TransformedFrame tframe = adapter->normalizeFrame( l[i].frame, l[ip1].frame);
			compute(tframe, &speed, &head);
			expected[0] = adapter->normalizeSpeed(l[i].speedCommand);
			expected[1] = adapter->normalizeHead( l[i].headCommand);
			if( (ip1)%3 == 0){
				//take the sample for the test
				int deltas = speed-expected[0];
				int deltah = head -expected[1];
				testmean += (deltas*deltas + deltah*deltah)/ (2 * (listsize/3));
			}
			else{
				//take the sample for the training
				//std::cout <<"[DEBUG LearningCommander.cpp]"<< "expected "<<expected[0]<<" "<<expected[1] << ". got "<<speed<<" "<<head << std::endl;
				errormean += learningmachine->backpropagation(expected, contribution) / listsize;
			}
		}
		std::cout << errormean << " " << testmean << std::endl;
	}
}

void LearningCommander::learnFromFile(std::string filename, int niteration){
	std::vector<struct InputOutput> l = std::vector<struct InputOutput>();
	ifstream file;
	file.open(filename);
	const int linesize = 8;
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
			file >> frame.x >> frame.y >> frame.speedx >> frame.speedy >> frame.chrono;
			file >> speed >> head;
			//std::cout<<"[DEBUG LearningCommander.cpp]"<<frame.x<<" "<<frame.y<<" "<<frame.speedx<<" "<<frame.speedy<<" "<<frame.chrono<<" "<<speed<<" "<<head<< std::endl;
			struct InputOutput io = {frame,(uint8_t)(speed),(int16_t)(head)};
			l.push_back(io);
		}
		else{
			break;
		}
	}
	learnFromList(l, niteration);
}
