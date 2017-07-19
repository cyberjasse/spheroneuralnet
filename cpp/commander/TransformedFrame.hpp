#ifndef TRANSFORMEDFRAME_HPP
#define TRANSFORMEDFRAME_HPP
/**
 * A data frame that summarize the current state and the state to reach.
 * Thus the current position is (0,0).
 * The current orientation is the x axis.
 */
struct TransformedFrame{
	/** The x composant of the current speed.*/
	int currentSpeedx;
	/** The y composant of the current speed.*/
	int currentSpeedy;
	/** The x coordinate of the position to reach.*/
	int targetx;
	/** The y coordinate of the position to reach.*/
	int targety;
	/** The x composant of the targeted speed.*/
	int targetSpeedx;
	/** The y composant of the targeted speed.*/
	int targetSpeedy;
};
#endif
