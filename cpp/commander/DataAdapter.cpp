#include "DataAdapter.hpp"

void DataAdapter::rotation(int16_t *x, int16_t *y, int16_t angle){
	double c = cos(angle);
	double s = sin(angle);
	double xprime = (*x) * c + (*y) * c;
	double yprime = (*y) * c - (*x) * s;
	*x = (int)(round(xprime));
	*y = (int)(round(yprime));
}

double DataAdapter::degreeToRadian(int16_t angle){
	return (angle * M_PI) / 180.0;
}

int16_t DataAdapter::correctAngle(int16_t angle){
	if(angle < -179)
		return angle+360;
	else if(angle > 180)
		return angle-360;
}
