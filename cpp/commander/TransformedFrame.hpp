#ifndef TRANSFORMEDFRAME_HPP
#define TRANSFORMEDFRAME_HPP
/**
 * A data frame that summarize the current state and the state to reach.
 * Thus the current position is (0,0).
 * And the current orientation is 0.
 */
struct TransformedFrame{
	/** The x composant of the current speed.*/
	int currentSpeedx;
	/** The y composant of the current speed.*/
	int currentSpeedy;
	/** The time between this frame and the previous. In microsecond.*/
	unsigned long time;
	/** The x coordinate of the position to reach.*/
	int targetx;
	/** The y coordinate of the position to reach.*/
	int targety;
	/** The x composant of the targeted speed.*/
	int targetSpeedx;
	/** The y composant of the targeted speed.*/
	int targetSpeedy;
	/** The x composant of the current acceleration. In 4mG*/
	int currentAccelx;
	/** The y composant of the current acceleration. In 4mG*/
	int currentAccely;
};
#endif
