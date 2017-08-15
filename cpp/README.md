To build one of c++ script you can see in spheroneuralnetsrc,
Juste use compile.sh providin the script name without extension.
For exemple, to compile learn.cpp,
. compile.sh learn

Via apt-get, you should install these package:
build-essential
cmake
git
pkg-config
libprotobuf-dev
libleveldb-dev
libsnappy-dev
libhdf5-serial-dev
protobuf-compiler
libatlas-base-dev
sudo apt-get install -y --no-install-recommends libboost-all-dev
libgflags-dev
libgoogle-glog-dev
liblmdb-dev
bluez
libbluetooth
libbluetooth-dev
libreadline6-dev
libncurses5
libncurses5-dev

randomDrive :
The script to drive randomly the Sphero. Provide a filename as parameter to save data.

learn :
The script to perfom a learning by the LearningCommander.
Provide as parameter all training set data you want.
training data are available in the tests folder and test/low5Hz with the .txt extension
virtualtest3.txt is the data from the virtual sphero.
testXX.txt is data from the real Sphero.

sleep :
A script to stand by the Sphero.
No parameter is needed if you use the same Sphero I used.

dataConverter :
A script I used to convert data to arff files.
As parameter, provide an input file name and an output file name.

aheadDrive :
It was to test if the Sphero sample a yaw of -90 degrees when we ask it to go to 90 degrees.

perf :
Script to get data about the time between stream frames.
Used to study the streaming stability

unittest :
Unit tests to test data transformations

In commander/, caffe-master is a neural network API from Berkeley university:
http://caffe.berkeleyvision.org/

sphero-linux-api is the modified API to drive a Sphero. From slock83:
https://github.com/slock83/sphero-linux-api
