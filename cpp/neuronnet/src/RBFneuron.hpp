/**
 * A RBF hidden neuron
 * @author Jason Bury
 */
#ifndef RBFNEURON_HPP
#define RBFNEURON_HPP
class RBFneuron : public Neuron{
	private:
		double prototype[];
		double variance[];
		/** Size of the prototype*/
		final neuronSize_t size;

	public:
		/**
		 * @param prototype The prototype is the means of the gaussian
		 * @param variance
		 */
		RBFneuron(double prototype[], double variance[]);
}
#endif
