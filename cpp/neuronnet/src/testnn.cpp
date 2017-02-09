#include "RBFnet.hpp"
#include "Net.hpp"
#include <cstdlib>
#include <time.h>
#include <iostream>

int main(int argc, char* argv[]){
	RBFnet net = RBFnet(1, 1, 1);
	int i;
	double input[1];
	double output[1];
	double backpropa[1];
	srand(time(NULL));//init seed
	for(i=0 ; i<100 ; i++){
		input[0] = (double)(rand() % 50);
		std::cout << "input= "<< input[0] <<std::endl;
		net.compute(input, output);
		std::cout << "input "<< input[0]<< ". Got "<< output[0] <<std::endl;
		net.backpropagation( input, backpropa);
	}
}
