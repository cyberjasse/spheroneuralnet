#ifndef MLPLAYER_HPP
#define MLPLAYER_HPP
#include "Layer.hpp"
#include "DerivableFunction.hpp"

/**
 * A layer with constructor to fill the layer by MLPneuron just providing size and derivable function
 */
class MLPlayer : public Layer{
public :
	/**
	 * @param f The derivable function
	 * @param length The size of the layer
	 * @param step The negative of the gradient step (-eta)
	 * @param Ninput The number of inputs of each neuron
	 */
	MLPlayer(DerivableFunction *f, layerSize_t length, double step, layerSize_t Ninput);
};
#endif
