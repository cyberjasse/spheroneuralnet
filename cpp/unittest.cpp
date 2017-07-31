#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "commander/DataAdapter.hpp"
#include "commander/SimpleAdapter.hpp"
#include "sphero/packets/async/StreamFrame.hpp"
#include "commander/TransformedFrame.hpp"
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

TEST_CASE("Testing convert degree -> radian"){
	CHECK(de(DataAdapter::degreeToRadian(1) , 0.0174532925));
	CHECK(de(DataAdapter::degreeToRadian(60) , M_PI/3));
}

TEST_CASE("Testing correct angle"){
	CHECK(DataAdapter::correctAngle(20) == 20);
	CHECK(DataAdapter::correctAngle(-5) == -5);
	CHECK(DataAdapter::correctAngle(-184) == 180-4);
	CHECK(DataAdapter::correctAngle(190) == -180+10);
}

TEST_CASE("Testing DataAdapter normalize frame"){
	struct StreamFrame current;
	struct StreamFrame target;
	current.yaw = -45;
	current.x = 12;
	current.y = 12;
	current.speedx = -50;
	current.speedy = 50;
	current.chrono = 100;
	target.yaw = 0;
	target.x = 20;
	target.y = 20;
	target.speedx = 30;
	target.speedy = 30;
	SimpleAdapter adapter;
	struct TransformedFrame tf = adapter.normalizeFrame(current, target);
	CHECK(tf.currentSpeedx == round(-100./sqrt(2.)));
	CHECK(tf.currentSpeedy == 0);
	CHECK(tf.time == 100);
	CHECK(tf.targetx == 0);
	CHECK(tf.targety == round(16./sqrt(2.)));
	CHECK(tf.targetSpeedx == 0);
	CHECK(tf.targetSpeedy == round(60. / sqrt(2.)));
}
