#include "RBFnet.hpp"
#include "MLPnet.hpp"
#include "Net.hpp"
#include <cstdlib>
#include <time.h>
#include <iostream>

/**
 * Create a nrow*ncol matrix
 */
void matrix(int nrow, int ncol, double value, double **&matrix){
	matrix = new double *[nrow];
	for(int i=0 ; i<nrow ; i++){
		matrix[i] = new double[ncol];
		for(int j=0 ; j<ncol ; j++){
			matrix[i][j] = value;
		}
	}
}

/**
 * Generate a random number of type double and in [min,max[
 * From http://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c
 */
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main(int argc, char* argv[]){
	srand(time(NULL));//init seed
	/*double **prototypes;
	matrix(2,1, 0.0, prototypes);
	std::cout << "prot:" << prototypes[0][0] << " , " << prototypes[1][0] << std::endl;
	double **weights;
	matrix(1,2, 0.5, weights);
	std::cout << "ws:" << weights[0][0] << " , " << weights[0][1] << std::endl;*/
	Net *net = new MLPnet(2, 1, 50, -0.2);
	int i;
	double input[2];
	double output[1];
	double backpropa[1];
	double min = -100.0;
	double max = 100.0;
	//Practice time
	for(i=0 ; i<100000 ; i++){
		input[0] = fRand(min,max);
		input[1] = fRand(min,max);
		net->compute(input, output);
		double *expected = new double[1];
		expected[0] = input[0] + input[1];
		net->backpropagation(expected , backpropa);
	}
	//Test time
	for(i=0 ; i<20 ; i++){
		input[0] = fRand(min,max);
		input[1] = fRand(min,max);
		net->compute(input, output);
		double *expected = new double[1];
		expected[0] = input[0] + input[1];
		std::cout << "Got "<< output[0] << "  expected "<< expected[0] << std::endl;
	}
	#ifdef NEUROPRINT
	net.neuroprint();
	#endif
}
/*
for the RBF

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
