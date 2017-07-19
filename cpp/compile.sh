FILE=$1
BUILDFOLDER='build/'
COMMANDERFOLDER='commander/'
THIS='compile.sh'

#test if api is cloned
if [ -d sphero-linux-api/ ]
then
	echo "[${THIS}] API sphero-linux-api/ already exist."
else
	echo "[${THIS}] API sphero-linux-api/ doesn't exist. Git clone..."
	git clone https://github.com/slock83/sphero-linux-api
fi
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
	echo "[${THIS}] The commander folder already exist."
else
	echo "[${THIS}] The commander folder doesn't exist. Creating..."
	mkdir ${BUILDFOLDER}${COMMANDERFOLDER}
fi
#compile the commander files
echo "[${THIS}] Compile files in commander folder"
cd ${COMMANDERFOLDER}
g++ -std=c++11 -c *.cpp
cd ..
mv ${COMMANDERFOLDER}*.o ${BUILDFOLDER}${COMMANDERFOLDER}
#now compile my files
echo "[${THIS}]    Compile ${FILE}.cpp ..."
g++ -DMAP -std=c++11 -c ${FILE}.cpp -o ${BUILDFOLDER}${FILE}.o
g++ -Lsphero-linux-api/ -o ${FILE} ${BUILDFOLDER}${FILE}.o ${BUILDFOLDER}${COMMANDERFOLDER}*.o -lsphero
echo "[${THIS}] define variable LD_LIBRARY_PATH=sphero-linux-api/"
export LD_LIBRARY_PATH=sphero-linux-api/
