#define CPU_ONLY
#include <caffe/caffe.hpp>
#include <memory>
#include "caffe/layers/memory_data_layer.hpp"
#include "LearningCommander.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

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
			double speed, head;
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

void LearningCommander::learnFromList(std::vector<InputOutput> l, bool normalize){
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
	
	// normalize data ------------
	
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
	
	// Convert data to MemoryDataLayer
	
	// memory allocation
	const int BATCHSIZE = 100;
	const int N = tflistSize/BATCHSIZE;
	double *data = new double[BATCHSIZE*(INPUTSIZE+OSIZE)*N];//FIXME really *(inputsize+outputsize) ?
	double *label = new double[BATCHSIZE*OSIZE*N];
	// fill
	int layersize = N*BATCHSIZE;
	int index = -1;
	for(int i=0; i<layersize ; i++){
		index++ ; data[index] = (double)(tflist[i].currentSpeedx);
		index++ ; data[index] = (double)(tflist[i].currentSpeedy);
		index++ ; data[index] = (double)(tflist[i].targetx);
		index++ ; data[index] = (double)(tflist[i].targety);
		index++ ; data[index] = (double)(tflist[i].currentAccelx);
		index++ ; data[index] = (double)(tflist[i].currentAccely);
	}
	index = -1;
	for(int i=0; i<layersize ; i++){
		index++ ; data[index] = (double)(headlist[i]);
	}
	//perform normalization
	if(normalize){
		//normalize inputs
		index = -1;
		for(int i=0 ; i<layersize ; i++){
			for(int j=0 ; i<INPUTSIZE ; i++){
				index ++;
				data[index] = (data[index]-inputMeans[j]) / inputSd[j];
			}
		}
		//normalize labels (outputs)
		index = -1;
		for(int i=0 ; i<layersize ; i++){
			for(int j=0 ; i<OSIZE ; i++){
				index ++;
				label[index] = (label[index]-outputMeans[j]) / outputSd[j];
			}
		}
	}
	
	// perform training.  //inspired by https://medium.com/@shiyan/caffe-c-helloworld-example-with-memorydata-input-20c692a82a22
	
	// load solver parameters . solver = neural network. parameters = epochs, batch size, learning rate... It also load the net architecture.
	caffe::SolverParameter solver_param;
    caffe::ReadSolverParamsFromTextFileOrDie("./solver.prototxt", &solver_param);
    // create the solver with loaded parameters
    boost::shared_ptr<caffe::Solver<double> > solver(caffe::SolverRegistry<double>::CreateSolver(solver_param));
    caffe::MemoryDataLayer<double> *dataLayer_trainnet = (caffe::MemoryDataLayer<double> *) (solver->net()->layer_by_name("inputdata").get());
    	caffe::MemoryDataLayer<double> *dataLayer_testnet_ = (caffe::MemoryDataLayer<double> *) (solver->test_nets()[0]->layer_by_name("test_inputdata").get());
	// provide data and label
	dataLayer_testnet_->Reset(data, label, layersize);
	dataLayer_trainnet->Reset(data, label, layersize);
	// train the solver
	solver->Solve();
	
	// test
	
	std::shared_ptr<caffe::Net<double> > testnet;
	testnet.reset(new caffe::Net<double>("./model.prototxt", caffe::TEST));
	testnet->ShareTrainedLayersWith(solver->net().get());
	caffe::MemoryDataLayer<double> *dataLayer_testnet = (caffe::MemoryDataLayer<double> *) (testnet->layer_by_name("test_inputdata").get());
	dataLayer_testnet_->Reset(data, label, layersize);
	testnet->Forward();
    boost::shared_ptr<caffe::Blob<double> > output_layer = testnet->blob_by_name("output");
    const double* begin = output_layer->cpu_data();
    const double* end = begin + layersize;
    std::vector<float> result(begin, end);
    index = -1;
    for(int i=0 ; i<result.size() ; i++){
    	index++;
    	std::cout << " Got " << result[i] << "   expected " << label[index] << std::endl;
    }
    free(data);
    free(label);
}

void LearningCommander::learnFromFile(std::string filename, int timemin, int timemax){
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
	learnFromList(l);
}
