#include "LearningCommander.hpp"
#include <iostream>

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
	double errorsum; //Sum of quadratic errors
	double testsum; //Sum of test errors
	int listsize = l.size();
	double speed, head;
	double expected[2];
	double contribution[6];
	for(int iteration=0 ; iteration<niteration ; iteration++){
		errorsum = 0;
		testsum = 0;
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
				testsum += deltas*deltas + deltah*deltah;
			}
			else{
				//take the sample for the training
				errorsum += learningmachine->backpropagation(expected, contribution);
			}
		}
		std::cout << errorsum << " " << testsum;
	}
}
				
