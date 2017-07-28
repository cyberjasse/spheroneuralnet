#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "commander/DataAdapter.hpp"
#include <math.h>

bool de(double d1, double d2){
	double comp = d2-d1;
	if(comp <0) comp = -comp;
	return comp < 0.00001;
}

TEST_CASE("Testing rotation"){
	int16_t x,y;
	const int m=1000;
	x=2*m;
	y=0*m;
	DataAdapter::rotation(&x,&y, M_PI/2.);
	CHECK(x==0*m);
	CHECK(y==2*m);
	DataAdapter::rotation(&x,&y, -M_PI/4.);
	CHECK(x==round(sqrt(2.)*m));
	CHECK(y==round(sqrt(2.)*m));
	DataAdapter::rotation(&x,&y, M_PI/4. + M_PI/3.);
	CHECK(x == -round(sqrt(3)*m));
	CHECK(y == 1*m);
}
