#ifndef POINTLISTTARGET_HPP
#define POINTLISTTARGET_HPP
#include "Target.hpp"
#include <vector>
#include <iostream>
#include <cstdint>

/**
 * Provide target path by a list of point writed in a file
 */
class PointlistTarget : public Target{
private :
	std::vector<StreamFrame> list;
	/** is the path finished */
	bool end;
	
	/**
	 * Compute distance between 2 points.
	 * @return the square of the euclidian distance
	 */
	int distance(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
	
public :
	PointlistTarget(std::string filename);
	
	virtual StreamFrame getTarget(struct StreamFrame *frame);
	
	virtual int remaining();
};

#endif
