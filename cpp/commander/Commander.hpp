#ifndef COMMANDER_HPP
#define COMMANDER_HPP
#include "DataAdapter.hpp"
#include <cstdint>
/**
 * A structure that represents a command
 */
struct Command{
	/** The speed of the motor. From 0 to 255.*/
	uint8_t speed;
	/** The orientation head. From -179 to 180.*/
	int16_t yaw;
}

/**
 * The abstract class for module to take decision about the command to send.
 */
class Commander{
	/** To be notified about a new stream frame (transformed). Then take decision. Then send it to the data adapter.*/
	public :
		virtual Command* getCommand(struct TransformedFrame *frame)=0;
	
		Commander(DataAdapter *dataadapter);
};
