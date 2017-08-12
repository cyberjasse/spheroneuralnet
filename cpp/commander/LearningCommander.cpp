#define CPU_ONLY
#include <caffe/caffe.hpp>
#include <memory>
#include "caffe/layers/memory_data_layer.hpp"
#include "LearningCommander.hpp"
#include <iostream>
#include <fstream>
#include <math.h>
#include <unistd.h>

LearningCommander::LearningCommander(Sphero *sph, DataAdapter *dadapter){
	sphero = sph;
	adapter = dadapter;
	target = NULL;
}

void LearningCommander::setTarget(Target *tar){
	target = tar;
}

void LearningCommander::notify(struct StreamFrame *frame){
	/*if(target != NULL){
		if(target->remaining() > 0){
			// prepare input
			struct StreamFrame targetstate = target->getTarget(frame);
			struct TransformedFrame transformed = adapter->normalizeFrame(*frame, targetstate);
			float speed, head;
			compute(transformed, &speed, &head, true, false);
			uint8_t ns = adapter->denormalizeSpeed(round(speed));
			int16_t nh = adapter->denormalizeHead( round(head ));
			sphero->roll(ns, nh);
		}
		else{
			sphero->disconnect();
		}
	}*/
}

double LearningCommander::normalize(double value, double min, double max){
	if(max==min)
		max = min+1;
	return (value - min) / (max - min);
}

double LearningCommander::denormalize(double value, double min, double max){
	if(max==min)
		max = min+1;
	return (value * (max - min) + min);
}

void LearningCommander::learnFromList(std::vector<InputOutput> l, bool tonormalize){
	const int ratioTest = 3; //take 1 sample on 3 for the test
	float errormean; //Mean of quadratic errors
	float testmean; //Mean of test errors
	int listsize = l.size();
	float speed, head;
	float expected[OUTPUTSIZE];
	// fill list of transformed frame and fill list of outputs
	int tflistSize = listsize-1;
	struct TransformedFrame tflist[tflistSize];
	float speedlist[tflistSize];
	float headlist[tflistSize];
	for(int i=0 ; i<tflistSize ; i++){
		tflist[i] = adapter->normalizeFrame( l[i].frame, l[i+1].frame);
		speedlist[i] = adapter->normalizeSpeed(l[i].speedCommand);
		headlist[i] = adapter->normalizeHead( l[i].headCommand);
	}
	
	// find minimums and maximums to normalize data ------------
	
	if(tonormalize){
		//initial values
		inputMins[0] = (float)(tflist[0].currentSpeedx);
		inputMins[1] = (float)(tflist[0].currentSpeedy);
		inputMins[2] = (float)(tflist[0].targetx);
		inputMins[3] = (float)(tflist[0].targety);
		inputMins[4] = (float)(tflist[0].currentAccelx);
		inputMins[5] = (float)(tflist[0].currentAccely);
		for(int i=0; i<INPUTSIZE; i++)
			inputMaxs[i] = inputMins[i];
		outputMins[0] = (float)(speedlist[0]);
		outputMins[1] = (float)(headlist[0]);
		for(int i=0; i<OUTPUTSIZE; i++)
			outputMaxs[i] = outputMins[i];
		//search mins and maxs
		for(int i=0; i<tflistSize; i++){
			//input min
			inputMins[0] = min((float)(tflist[i].currentSpeedx) , inputMins[0]);
			inputMins[1] = min((float)(tflist[i].currentSpeedy) , inputMins[1]);
			inputMins[2] = min((float)(tflist[i].targetx) , inputMins[2]);
			inputMins[3] = min((float)(tflist[i].targety) , inputMins[3]);
			inputMins[4] = min((float)(tflist[i].currentAccelx) , inputMins[4]);
			inputMins[5] = min((float)(tflist[i].currentAccely) , inputMins[5]);
			//output min
			outputMins[0] = min((float)(speedlist[i]) , outputMins[0]);
			outputMins[1] = min((float)(headlist[i]), outputMins[1]);
			//input max
			inputMaxs[0] = max((float)(tflist[i].currentSpeedx) , inputMaxs[0]);
			inputMaxs[1] = max((float)(tflist[i].currentSpeedy) , inputMaxs[1]);
			inputMaxs[2] = max((float)(tflist[i].targetx) , inputMaxs[2]);
			inputMaxs[3] = max((float)(tflist[i].targety) , inputMaxs[3]);
			inputMaxs[4] = max((float)(tflist[i].currentAccelx) , inputMaxs[4]);
			inputMaxs[5] = max((float)(tflist[i].currentAccely) , inputMaxs[5]);
			//output max
			outputMaxs[0] = max((float)(speedlist[i]) , outputMaxs[0]);
			outputMaxs[1] = max((float)(headlist[i]), outputMaxs[1]);
		}
	}
	
	// Convert data to MemoryDataLayer
	
	// memory allocation
	const int BATCHSIZE = 100;
	const int N = tflistSize/BATCHSIZE;
	float *data = new float[BATCHSIZE*(INPUTSIZE)*N];//FIXME really *(inputsize+outputsize) ?
	float *label = new float[BATCHSIZE*OSIZE*N];
	// fill
	int layersize = N*BATCHSIZE;
	int index = -1;
	for(int i=0; i<layersize ; i++){
		index++ ; data[index] = (float)(tflist[i].currentSpeedx);
		index++ ; data[index] = (float)(tflist[i].currentSpeedy);
		index++ ; data[index] = (float)(tflist[i].targetx);
		index++ ; data[index] = (float)(tflist[i].targety);
		index++ ; data[index] = (float)(tflist[i].currentAccelx);
		index++ ; data[index] = (float)(tflist[i].currentAccely);
	}
	for(int i=0; i<layersize ; i++){
		label[i] = (float)(headlist[i]);
	}
	//perform normalization
	if(tonormalize){
		//normalize inputs
		index = -1;
		for(int i=0 ; i<layersize ; i++){
			for(int j=0 ; j<INPUTSIZE ; j++){
				index ++;
				data[index] = normalize(data[index], inputMins[j], inputMaxs[j]);
			}
		}
		//normalize labels (outputs)
		for(int i=0 ; i<layersize ; i++){
			label[i] = normalize(label[i], outputMins[1], outputMaxs[1]);
		}
	}
	sleep(1);
	
	// perform training.  //inspired by https://medium.com/@shiyan/caffe-c-helloworld-example-with-memorydata-input-20c692a82a22

    caffe::SolverParameter solver_param;
    caffe::ReadSolverParamsFromTextFileOrDie("./solver.prototxt", &solver_param);

    boost::shared_ptr<caffe::Solver<float> > solver(caffe::SolverRegistry<float>::CreateSolver(solver_param));
    caffe::MemoryDataLayer<float> *dataLayer_trainnet = (caffe::MemoryDataLayer<float> *) (solver->net()->layer_by_name("inputdata").get());
    caffe::MemoryDataLayer<float> *dataLayer_testnet_ = (caffe::MemoryDataLayer<float> *) (solver->test_nets()[0]->layer_by_name("test_inputdata").get());

    dataLayer_testnet_->Reset(data, label, layersize);

    dataLayer_trainnet->Reset(data, label, layersize);

    solver->Solve();

    boost::shared_ptr<caffe::Net<float> > testnet;

    testnet.reset(new caffe::Net<float>("./model.prototxt", caffe::TEST));
    //testnet->CopyTrainedLayersFrom("XOR_iter_5000000.caffemodel");

    testnet->ShareTrainedLayersWith(solver->net().get());

    caffe::MemoryDataLayer<float> *dataLayer_testnet = (caffe::MemoryDataLayer<float> *) (testnet->layer_by_name("test_inputdata").get());

    dataLayer_testnet->Reset(data, label, layersize);

    testnet->Forward();

    boost::shared_ptr<caffe::Blob<float> > output_layer = testnet->blob_by_name("output");

    const float* begin = output_layer->cpu_data();
    const float* end = begin + layersize;
    
    std::vector<float> result(begin, end);

	index = -1;
    for(int i = 0; i< result.size(); ++i){
    	index++;
    	std::cout << " Input=";
    	for(int j=index*INPUTSIZE ; j<index*INPUTSIZE+INPUTSIZE ; j++)
    		std::cout <<" "<<data[j];
    	std::cout << "  Got "<<result[i]<<"   expected "<<label[index]<<std::endl;
    }
}

void LearningCommander::learnFromFiles(std::vector<std::string> filenames, int timemin, int timemax){
	std::vector<struct InputOutput> l = std::vector<struct InputOutput>();
	std::ifstream file;
	const int linesize = 10;
	std::string garb;
	for(std::string filename : filenames){
		file.open(filename);
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
	}
	learnFromList(l, true);
}
