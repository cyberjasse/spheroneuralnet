/**
 * Do not move the sphero but get the streaming frame. Usefull to test the bluetooth performance
 */
#include "sphero/bluetooth/bluez_adaptor.h"
#include "sphero/Sphero.hpp"
#include "sphero/packets/async/StreamObserver.hpp"
#include <cstdint>
#include <iostream>

class PrintTime : public StreamObserver{
private :
	Sphero *sph;
	int count;
public :
	PrintTime(Sphero *s){
		sph=s;
		count = 0;
	}
	virtual void notify(struct StreamFrame *frame){
		std::cout << frame->chrono<<" "<<frame->yaw<<" "<<frame->x<<" "<<frame->y<<" "<<frame->speedx<<" "<<frame->speedy<<" "<<frame->ax<<" "<<frame->ay << std::endl;
		count++;
		if(count >= 500){
			sph->disconnect();
		}
	}
};

std::string DEFAULT_ADDRESS = "68:86:E7:00:58:83";
using namespace std;
int main(int argc, char* argv[]){
	
	//load the adress
	string address;
	if(argc<2){
		address = DEFAULT_ADDRESS;
	}
	else{
		address = argv[1];
	}
	
	//create the Sphero object
	Sphero *sph = new Sphero(address.c_str(), new bluez_adaptor());
	//connection
	if(sph->connect()){
		cerr << " >connect() == true< " << endl;
	}
	else{
		cerr << " >connection failed< " << endl;
		delete sph;
		return 1;
	}
	
	//start stream
	std::cout << "time yaw x y speedx speedy accelx accely\n";
	PrintTime *observer = new PrintTime(sph);
	sph->addStreamObserver(observer);
	sph->startStream( (uint8_t)(200) );
}
