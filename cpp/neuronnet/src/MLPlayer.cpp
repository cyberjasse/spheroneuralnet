#include "MLPlayer.hpp"
#include "MLPneuron.hpp"
#include <time.h>

MLPlayer::MLPlayer(DerivableFunction *f, layerSize_t length, double step, layerSize_t Ninput)
	: Layer(NULL, length, step)
{
	ns = new Neuron*[length];
	for(int i=0 ; i<length ; i++){
		ns[i] = new MLPneuron(f, Ninput);
	}
}
