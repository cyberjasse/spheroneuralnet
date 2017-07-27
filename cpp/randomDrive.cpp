/**
 * Drive randomly the sphero and save data about the stream and commands
 * 
 * The first parameter is the name of the file to store data
 * The second parameter is the adress of the sphero. But optional. By default, its the adress of my sphero.
 * (format : "XX:XX:XX:XX:XX:XX")
 */
#include <iostream>
#include <csignal>
#include <string>
#include <vector>
#include "sphero/bluetooth/bluez_adaptor.h"
#include "sphero/Sphero.hpp"
#include "commander/RandomCommander.hpp"

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
		cout << "Default address is "<<DEFAULT_ADDRESS<<endl;
		address = DEFAULT_ADDRESS;
	}
	else{
		address = argv[2];
	}
	
	//create the Sphero object
	Sphero *sph = new Sphero(address.c_str(), new bluez_adaptor());
	//connection
	if(sph->connect()){
		cout << " >connect() == true< " << endl;
	}
	else{
		cout << " >connection failed< " << endl;
		delete sph;
		return 1;
	}
	
	/* ################################
	   ##Define other parameters here##
	   ################################*/
	
	const short frequency  = 5;
	const int   squareSize = 150;//cm
	const int   maxSampling= 5000;
	
	//Create the map
	int halfsize = squareSize/2;
	struct Bounds bounds = {-halfsize,halfsize,halfsize,-halfsize};
	
	//Instanciate commander objects
	RandomCommander *commander = new RandomCommander(sph, filename, &bounds);
	
	//Start the streaming
	sph->addStreamObserver(commander);
	sph->startStream(frequency);
	return 0;
}
