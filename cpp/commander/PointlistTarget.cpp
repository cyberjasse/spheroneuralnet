#include "PointlistTarget.hpp"
#include <fstream>
#include <iostream>

PointlistTarget::PointlistTarget(std::string filename){
	list = std::vector<StreamFrame>();
	std::ifstream file;
	file.open(filename);
	int16_t x;
	int16_t y;
	while(file >> x){
		file >> y;
		struct StreamFrame sf = {0,x,y,0,0,0};
		list.push_back(sf);
	}
	file.close();
	if(list.size() < 1){
		end = true;
	}
	else{
		end = false;
	}
}

int PointlistTarget::distance(int16_t x1, int16_t y1, int16_t x2, int16_t y2){
	int dx = x1-x2;
	int dy = y1-y2;
	return dx*dx + dy*dy;
}
StreamFrame PointlistTarget::getTarget(struct StreamFrame *frame){
	//Find the nearest point from the sphero
	int argmin = 0;
	int min = distance(frame->x, frame->y, list[0].x, list[0].y);
	for(int i=1 ; i<list.size() ; i++){
		int d = distance(frame->x, frame->y, list[i].x, list[i].y);
		if(d<min){
			min = d;
			argmin = i;
		}
	}
	//return the next stream frame
	int lastindex = list.size()-1;
	if(argmin == lastindex){
		end = true;
		return list[lastindex];
	}
	else{
		return list[argmin+1];
	}
}

int PointlistTarget::remaining(){
	if(end) return 0;
	else return 1;
}
