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

void LearningCommander::compute(struct TransformedFrame transformed, double *speed, double *head, bool normalize, bool print ){
	double input[INPUTSIZE] = { (double)(transformed.currentSpeedx),
						(double)(transformed.currentSpeedy),
						(double)(transformed.targetx),
						(double)(transformed.targety),
						(double)(transformed.currentAccelx),
						(double)(transformed.currentAccely)};/*,
						(double)(transformed.targetSpeedx),
						(double)(transformed.targetSpeedy)};*/
	//apply normalization
	if(normalize){
		for(int i=0 ; i<INPUTSIZE ; i++){
			input[i] = (input[i]-inputMeans[i]) / inputSd[i];
		}
	}
	const int osize = 1;
	double output[osize];
	learningmachine->compute(input, output);
	//*speed = output[0];
	*head = output[0];
	if(print){
		std::cout << "[input] ";
		for(int i=0 ; i<INPUTSIZE ; i++)
			std::cout << input[i] <<" ";
		std::cout << "[output] ";
		for(int i=0 ; i<osize ; i++)
			std::cout << output[i] <<" ";
	}
}

double LearningCommander::backpropagation(double *expected, bool normalize, bool print){
	double contribution[INPUTSIZE];
	if(normalize){
		expected[0] = (expected[0]-outputMeans[1]) / outputSd[1];
	}
	learningmachine->backpropagation(expected, contribution);
	if(print){
		std::cout <<"[expected] "<<expected[0]<< std::endl;
	}
}

void LearningCommander::notify(struct StreamFrame *frame){
	if(target != NULL){
		if(target->remaining() > 0){
			// prepare input
			struct StreamFrame targetstate = target->getTarget(frame);
			struct TransformedFrame transformed = adapter->normalizeFrame(*frame, targetstate);
			double speed, head;
			compute(transformed, &speed, &head, true, false);
			uint8_t ns = adapter->denormalizeSpeed(round(speed));
			int16_t nh = adapter->denormalizeHead( round(head ));
			sphero->roll(ns, nh);
		}
		else{
			sphero->disconnect();
		}
	}
}

void LearningCommander::learnFromList(std::vector<InputOutput> l, int niteration, bool normalize){
	const int ratioTest = 3; //take 1 sample on 3 for the test
	double errormean; //Mean of quadratic errors
	double testmean; //Mean of test errors
	int listsize = l.size();
	double speed, head;
	double expected[OUTPUTSIZE];
	// fill list of transformed frame and fill list of outputs
	int tflistSize = listsize-1;
	struct TransformedFrame tflist[tflistSize];
	double speedlist[tflistSize];
	double headlist[tflistSize];
	for(int i=0 ; i<tflistSize ; i++){
		tflist[i] = adapter->normalizeFrame( l[i].frame, l[i+1].frame);
		speedlist[i] = adapter->normalizeSpeed(l[i].speedCommand);
		headlist[i] = adapter->normalizeHead( l[i].headCommand);
	}
	// normalize data
	if(normalize){
		for(int i=0 ; i<INPUTSIZE ; i++){
			inputMeans[i] = 0.0;
			inputSd[i] = 0.0;
		}
		for(int i=0 ; i<OUTPUTSIZE ; i++){
			outputMeans[i] = 0.0;
			outputSd[i] = 0.0;
		}
		//compute input means
		for(int i=0 ; i<tflistSize ; i++){
			inputMeans[0] += (double)(tflist[i].currentSpeedx);
			inputMeans[1] += (double)(tflist[i].currentSpeedy);
			inputMeans[2] += (double)(tflist[i].targetx);
			inputMeans[3] += (double)(tflist[i].targety);
			inputMeans[4] += (double)(tflist[i].currentAccelx);
			inputMeans[5] += (double)(tflist[i].currentAccely);
		}
		for(int i=0 ; i<INPUTSIZE ; i++){
			inputMeans[i] /= tflistSize;
		}
		//compute output means
		for(int i=0 ; i<tflistSize ; i++){
			outputMeans[0] += (double)speedlist[i];
			outputMeans[1] += (double)headlist[i];
		}
		for(int i=0 ; i<OUTPUTSIZE ; i++){
			outputMeans[i] /= tflistSize;
		}
		//compute input sd
		for(int i=0 ; i<tflistSize ; i++){
			inputSd[0] += pow( (double)(tflist[i].currentSpeedx)-inputMeans[0] , 2);
			inputSd[1] += pow( (double)(tflist[i].currentSpeedy)-inputMeans[1] , 2);
			inputSd[2] += pow( (double)(tflist[i].targetx)-inputMeans[2] , 2);
			inputSd[3] += pow( (double)(tflist[i].targety)-inputMeans[3] , 2);
			inputSd[4] += pow( (double)(tflist[i].currentAccelx)-inputMeans[4] , 2);
			inputSd[5] += pow( (double)(tflist[i].currentAccely)-inputMeans[5] , 2);
		}
		for(int i=0 ; i<INPUTSIZE ; i++){
			inputSd[i] /= tflistSize;
			inputSd[i] = sqrt(inputSd[i]);
		}
		//compute output sd
		for(int i=0 ; i<tflistSize ; i++){
			outputSd[0] += pow( (double)speedlist[i]-outputMeans[0] , 2);
			outputSd[1] += pow( (double)headlist[i]-outputMeans[1] , 2);
		}
		for(int i=0 ; i<OUTPUTSIZE ; i++){
			outputSd[i] /= tflistSize;
			outputSd[i] = sqrt(outputSd[i]);
		}
	}
	//perform learning		
	for(int iteration=0 ; iteration<niteration ; iteration++){
		bool last = iteration==niteration-1;
		errormean = 0.0;
		for(int i=0 ; i<tflistSize ; i++){
			compute(tflist[i], &speed, &head, normalize, last);
			//take the sample for the training
			expected[0] = headlist[i];
			errormean += backpropagation(expected, normalize, last);
		}
		errormean /= tflistSize;
		if(iteration%10==0){
			std::cout << " errormean = " << errormean << std::endl;
		}
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
