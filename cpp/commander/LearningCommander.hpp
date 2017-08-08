#ifndef LEARNINGCOMMANDER_HPP
#define LEARNINGCOMMANDER_HPP
#include "../sphero/packets/async/StreamObserver.hpp"
#include "DataAdapter.hpp"
#include "Target.hpp"
#include "../sphero/Sphero.hpp"
#include "../neuronnet/src/Net.hpp"
#include <vector>

#define INPUTSIZE 6
#define OUTPUTSIZE 2

/**
 * A struct containing a StreamFrame and commands
 */
struct InputOutput{
	struct StreamFrame frame;
	uint8_t speedCommand;
	int16_t headCommand;
};

/**
 * A Commander that use a learning machine
 */
class LearningCommander : public StreamObserver{
	private :
		Net *learningmachine;
		DataAdapter *adapter;
		Sphero *sphero;
		Target *target;
		
		/** Mean of inputs */
		double inputMeans[INPUTSIZE];
		/** standard deviation of inputs */
		double inputSd[INPUTSIZE];
		/** Mean of outputs */
		double outputMeans[OUTPUTSIZE];
		/** standard deviation of outputs */
		double outputSd[OUTPUTSIZE];
		
		/**
		 * Call to learningmachine.compute() but providing a TransformedFrame as input and apply normalization before
		 * @param speed The speed returned by the learning machine output
		 * @param head The head returned by the learning machine output
		 */
		void compute(struct TransformedFrame tframe, double *speed, double *head, bool normalize, bool print);
		
		/**
		 * Call to learningmachone.backpropagation() but apply normalization before
		 */
		double backpropagation(double *expected, bool normalize, bool print);
	
	public :
		LearningCommander(Sphero *sph, DataAdapter *dadapter, Net *lm);
		
		/**
		 * Set a Target object.
		 * The commander will send command to do all states given by the target (is possible)
		 * If the target finish to send target states, the commander will reject its target
		 */
		virtual void setTarget(Target *tar);
		
		/**
		 * Perform a learning providing a list of pairs input-output.
		 * The input is a StreamFrame that the sphero can give and the ouput is the command sended after received the streamframe
		 * @param niteration The number of times the commander will perform a learning from the list
		 * @param normalize Set to true to normalize data
		 */
		void learnFromList(std::vector<InputOutput> l, int niteration, bool normalize=true);
		
		/**
		 * Same that learnFromList but providing a filename.
		 * With parameters to filter frames by the chrono value.
		 * @param timemin The minimum time between a frame and the previous to keep a frame
		 * @param timemax The maximum time between a frame and the next to keep a frame
		 */
		void learnFromFile(std::string filename, int niteration,  int timemin=0, int timemax=2000000);
		
		virtual void notify(struct StreamFrame *frame);
};

#endif
