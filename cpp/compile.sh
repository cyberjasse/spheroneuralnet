FILE=$1
BUILDFOLDER='build/'
COMMANDERFOLDER='commander/'
NEURONNETFOLDER='neuronnet/'
THIS='compile.sh'
CAFFEINCLUDES='-Icommander/caffe-master/include -Icommander/caffe-master/build/src'

#test if the .so library is compiled
if [ -f sphero-linux-api/libsphero.so ]
then
	echo "[${THIS}] library sphero-linux-api/libsphero.so already exist."
else
	echo "[${THIS}] library sphero-linux-api/libsphero.so doesn't exist. Using makefile in sphero-linux-api..."
	make -C sphero-linux-api/
fi
#test if the folder sphero exist
if [ -d sphero/ ]
then
	echo "[${THIS}] folder sphero/ already exist."
else
	echo "[${THIS}] folder sphero/ doesn't exist. Creating..."
	mkdir sphero
	#copy the folder structure of sphero-linux-api/src/
	cd sphero-linux-api/src/
	find -type d | xargs -i mkdir ../../sphero/{} 2> /dev/null
	#copy header of api in the sphero folder
	find -type f | grep -i ".h$\|.hpp$\|.tpp" | xargs -i cp {} ../../sphero/{}
	cd ../..
fi
#compile caffe
if [ -d ${COMMANDERFOLDER}/caffe-master/build/ ]
then
	echo "[${THIS}] caffe already built."
else
	echo "[${THIS}] Compile caffe."
	make -C ${COMMANDERFOLDER}/caffe-master/
fi
#test if the build folder exist
if [ -d ${BUILDFOLDER} ]
then
	echo "[${THIS}] The build folder already exist."
else
	echo "[${THIS}] The build folder doesn't exist. Creating..."
	mkdir ${BUILDFOLDER}
fi
#test if the commander folder exist
if [ -d ${BUILDFOLDER}${COMMANDERFOLDER} ]
then
	echo "[${THIS}] The commander build folder already exist."
else
	echo "[${THIS}] The commander build folder doesn't exist. Creating..."
	mkdir ${BUILDFOLDER}${COMMANDERFOLDER}
fi
#compile the commander files
echo "[${THIS}] Compile files in commander folder"
cd ${COMMANDERFOLDER}
g++ -std=c++11 -c -Icaffe-master/include -Icaffe-master/build/src *.cpp
cd ..
mv ${COMMANDERFOLDER}*.o ${BUILDFOLDER}${COMMANDERFOLDER}
#test if the neuronnet folder exist
if [ -d ${BUILDFOLDER}${NEURONNETFOLDER} ]
then
	echo "[${THIS}] The neuronnet build folder already exist."
else
	echo "[${THIS}] The neuronnet build folder doesn't exist. Creating..."
	mkdir ${BUILDFOLDER}${NEURONNETFOLDER}
fi
#compile the neuronnet files
echo "[${THIS}] Compile files in neuronnet folder"
NEURONNETPATH='neuronnet/src/'
cd ${NEURONNETPATH}
g++ -c *.cpp
cd ../..
mv ${NEURONNETPATH}*.o ${BUILDFOLDER}${NEURONNETFOLDER}
#now compile my files
if [ -n "$1" ]
then
	echo "[${THIS}]    Compile ${FILE}.cpp .   If there is a problem, perhaps you have to change the path /usr/lib/x86_64-gnu/ in this compile.sh, line 80"
	g++ -std=c++11 -c ${CAFFEINCLUDES} ${FILE}.cpp -o ${BUILDFOLDER}${FILE}.o && \
	g++ -Lsphero-linux-api/ -Lcommander/caffe-master/build/lib -L/usr/lib/x86_64-gnu/ -o ${FILE} ${BUILDFOLDER}${FILE}.o ${BUILDFOLDER}${COMMANDERFOLDER}*.o ${BUILDFOLDER}${NEURONNETFOLDER}*.o -lsphero -lcaffe -lglog -lboost_system
else
	echo "[${THIS}] No file entered in parameter. Only package are compiled."	
fi
#Defile a variable. Else the executable will not find the sphero api
echo "[${THIS}] LD_LIBRARY_PATH with paths"
export LD_LIBRARY_PATH=sphero-linux-api/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:commander/caffe-master/build/lib
