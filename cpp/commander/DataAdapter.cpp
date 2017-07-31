#include "DataAdapter.hpp"
#include <math.h>

void DataAdapter::rotation(int16_t *x, int16_t *y, double angle){
	double c = cos(angle);
	double s = sin(angle);
	double xprime = (*x) * c - (*y) * s;
	double yprime = (*y) * c + (*x) * s;
	*x = (int)(round(xprime));
	*y = (int)(round(yprime));
}

double DataAdapter::degreeToRadian(int16_t angle){
	return (angle * M_PI) / 180.0;
}

int16_t DataAdapter::correctAngle(int16_t angle){
	return getAngleBetween(angle, -179,180);
}

int DataAdapter::getAngleBetween(int angle, int min, int max){
	if(angle < min)
		return getAngleBetween(angle+360, min, max);
	if(angle > max)
		return getAngleBetween(angle-360, min, max);
	return angle;
}
