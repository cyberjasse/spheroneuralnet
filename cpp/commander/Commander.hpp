#ifndef COMMANDER_HPP
#define COMMANDER_HPP
#include "TransformedFrame.hpp"
#include "Command.hpp"

/**
 * The interface for module to take decision about the command to send.
 */
class Commander{
	/** To be notified about a new stream frame (transformed). Then take decision. Then send it to the data adapter.*/
	public :
		virtual Command getCommand(struct TransformedFrame *frame)=0;
};
#endif
