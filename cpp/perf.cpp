/** This script for measuring bluetooth performance and latency to get information from sensors*/
#include <iostream>
#include <csignal>
#include <string>
#include <vector>
#include <sys/time.h>
#include <unistd.h>
#include "sphero/bluetooth/bluez_adaptor.h"
#include "sphero/Sphero.hpp"
#include "sphero/packets/Constants.hpp"
#include "sphero/packets/async/SpheroStreamingPacket.hpp"

string DEFAULT_ADDRESS = "68:86:E7:00:58:83";
//in a global variable to use it in the signal handler:
Sphero* sph = NULL;

void sigHandler(int num){
	cerr << "Handle interrupt "<<num<<"\n";
	if(sph != NULL){
		cerr << "delete sph"<<endl;
		delete sph;
	}
	exit(num);
}

void top(struct timeval &before, struct timeval &after){
	cout << "top " << after.tv_sec-before.tv_sec << " " << (after.tv_usec-before.tv_usec)<< endl;
}

using namespace std;
/** Enter the device adress in parameter (format : "XX:XX:XX:XX:XX:XX") */
int main(int argc, char* argv[]){
	//load the adress
	string address;
	if(argc<2){
		cerr << "Default address is "<<DEFAULT_ADDRESS<<endl;
		address = DEFAULT_ADDRESS;
	}
	else{
		vector<string> args = vector<string>(&argv[0],&argv[0+argc]);
		address = args[1];
	}
	//create the Sphero object
	sph = new Sphero(address.c_str(), new bluez_adaptor());
	signal(SIGINT, sigHandler);
	//connection
	sph->onConnect([](){
		cerr << " >Connection established< " << endl;
	});
	if(sph->connect()){
		cerr << " >connect() == true< " << endl;
	}
	else{
		cerr << " >connection failed< " << endl;
		delete sph;
		return 1;
	}
	/* send a streaming request
	uint32_t mask1 = 0;
	uint32_t mask2 = 0;
	mask2 += mask2::ODOMETER_X;
	mask2 += mask2::ODOMETER_Y;
	MyStreamingPacket* msp = new MyStreamingPacket();
	sph->setDataStreaming(3,1,mask1,30,mask2);*/
	// let the process
	uint16_t var;
	struct timeval t1;
	struct timeval t2;
	gettimeofday(&t1, NULL);
	int packetCount=0;
	int x = sph->getX();
	int y = sph->getY();
	int speedx = sph->getSpeedX();
	int speedy = sph->getSpeedY();
	bool changed = false;
	while(packetCount<10000){
		//usleep(1000);//maximum sampling frequency is 400Hz
		changed = false;
		if(sph->getX() != x){
			x = sph->getX();
			changed = true;
		}
		if(sph->getY() != y){
			y = sph->getY();
			changed = true;
		}
		if(sph->getSpeedX() != speedx){
			speedx = sph->getSpeedX();
			changed = true;
		}
		if(sph->getSpeedY() != speedy){
			speedy = sph->getSpeedY();
			changed = true;
		}
		if(changed){
			int modulo = packetCount%3;
			if(modulo == 0)
				sph->setColor(255,0,0, false);
			else if(modulo == 1)
				sph->setColor(0,255,0,false);
			else
				sph->setColor(0,0,255,false);
			gettimeofday(&t2, NULL);
			top(t1,t2);
			t1=t2;
			cerr<<".";//cerr <<sph->getX()<<" "<<sph->getY()<< " "<<sph->getSpeedX()<<" "<<sph->getSpeedY()<< endl;
			packetCount++;
		}
	}
	cerr<<"\n";
	//terminate
	delete sph;//calling destructor
	return 0;
}
