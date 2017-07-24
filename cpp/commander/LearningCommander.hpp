#ifndef LEARNINGCOMMANDER_HPP
#define LEARNINGCOMMANDER_HPP
#include "../sphero/packets/async/StreamObserver.hpp"
#include "DataAdapter.hpp"
#include "Target.hpp"
#include "../sphero/Sphero.hpp"
#include "../neuronnet/src/Net.hpp"
#include <vector>

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
		
		/**
		 * Call to learningmachine.compute() but providing a TransformedFrame as input
		 * @param speed The speed returned by the learning machine output
		 * @param head The head returned by the learning machine output
		 */
		void compute(struct TransformedFrame tframe, double *speed, double *head );
	
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
		 */
		void learnFromList(std::vector<InputOutput> l, int niteration);
		
		/**
		 * Same that learnFromList but providing a filename
		 */
		void learnFromFile(std::string filename, int niteration);
		
		virtual void notify(struct StreamFrame *frame);
};

#endif
