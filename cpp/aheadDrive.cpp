#include <iostream>
#include <string>
#include <vector>
#include "sphero/bluetooth/bluez_adaptor.h"
#include "sphero/Sphero.hpp"
#include "commander/AheadCommander.hpp"

string DEFAULT_ADDRESS = "68:86:E7:00:58:83";
using namespace std;
int main(int argc, char* argv[]){
	//Check if there is a filename
	string filename;
	if(argc<2){
		cerr << "Please enter a file name as parameter" <<endl;
		return 1;
	}
	else{
		filename = argv[1];
	}
	
	//load the adress
	string address;
	if(argc<3){
		cerr << "Default address is "<<DEFAULT_ADDRESS<<endl;
		address = DEFAULT_ADDRESS;
	}
	else{
		address = argv[2];
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
	
	/* ################################
	   ##Define other parameters here##
	   ################################*/
	
	const short frequency  = 40;
	
	//Instanciate commander objects
	AheadCommander *commander = new AheadCommander(sph, filename);
	
	//Start the streaming
	sph->addStreamObserver(commander);
	sph->startStream(frequency);
	return 0;
}
