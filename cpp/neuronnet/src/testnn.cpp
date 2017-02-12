#include "RBFnet.hpp"
#include "Net.hpp"
#include <cstdlib>
#include <time.h>
#include <iostream>

int main(int argc, char* argv[]){
	double **prototypes = new double*[1];
	prototypes[0] = new double[1];
	double **weights = new double*[1];
	weights[0] = new double[1];
	prototypes[0][0] = 0.0;
	weights[0][0] = 0.5;
	RBFnet net = RBFnet(1, 1, 2, 5, -5.0,  prototypes, weights);
	int i;
	double input[1];
	double output[1];
	double backpropa[1];
	srand(time(NULL));//init seed
	for(i=0 ; i<1 ; i++){
		input[0] = 3;//(double)(rand() % 1000) / 1000.0;
		net.compute(input, output);
		std::cout << "input "<< input[0]<< ", Got "<< output[0] <<std::endl;
		net.backpropagation( input, backpropa);
	}
}
/*
With 1 input, 1 hidden neuron, 1 exit neuron, sd=5, mu=0, weight=0.5
If input= 3 and expected output=3
hidden neuron output=0.8352702114
exit neuron output (with bias=1) =0.77243944
Modification weight = -eta* -1.0138098
Contribution exit neuron = 0.33067365
mu modification = -eta* 0.03314422?
sigma modification = -eta* 0.0198868

If input=10, hidden neuron output=0.1353352832, exit neuron output (with bias=1) =0.94039854
If input=1, hidden neuron output=0.9801986733, exit neuron output (with bias=1) =
If input=0.5, hidden neuron output=0.99501247792, exit neuron output (with bias=1) =
If input=0, hidden neuron output=1
If input=15, hidden neuron output=0.0111089
*/
