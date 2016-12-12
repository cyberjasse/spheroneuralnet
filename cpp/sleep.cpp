/**Stand by the Sphero*/
#include <iostream>
#include <csignal>
#include <string>
#include <vector>
#include <sys/time.h>
#include "sphero/bluetooth/bluez_adaptor.h"
#include "sphero/Sphero.hpp"

string DEFAULT_ADDRESS = "68:86:E7:00:58:83";
//in a global variable to use it in the signal handler:
Sphero* sph = NULL;

void sigHandler(int num){
	cout << "Handle interrupt "<<num<<"\n";
	if(sph != NULL){
		cout << "delete sph"<<endl;
		delete sph;
	}
	exit(num);
}

using namespace std;
/** Enter the device adress in parameter (format : "XX:XX:XX:XX:XX:XX") */
int main(int argc, char* argv[]){
	//load the adress
	string address;
	if(argc<2){
		cout << "Default address is "<<DEFAULT_ADDRESS<<endl;
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
		cout << " >Connection established< " << endl;
	});
	if(sph->connect()){
		cout << " >connect() == true< " << endl;
		cout<<"sleep 1 second"<<endl;
		sleep(1);
		cout<<"Stand by the Sphero"<<endl;
		sph->sleep(0);
	}
	else{
		cout << " >connection failed< " << endl;
		delete sph;
		return 1;
	}
	delete sph;//calling destructor
	return 0;
}
