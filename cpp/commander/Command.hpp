#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <cstdint>
/**
 * A structure that represents a command
 */
struct Command{
	/** The speed of the motor. From 0 to 255.*/
	uint8_t speed;
	/** The orientation head. From -179 to 180.*/
	int16_t yaw;
};
#endif
