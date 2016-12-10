FILE=$1

#test if api is cloned
if [ -d sphero-linux-api/ ]
then
	echo "API sphero-linux-api/ already exist."
else
	echo "API sphero-linux-api/ doesn't exist. Git clone..."
	git clone https://github.com/slock83/sphero-linux-api
fi
#test if the .so library is compiled
if [ -f sphero-linux-api/libsphero.so ]
then
	echo "library sphero-linux-api/libsphero.so already exist."
else
	echo "library sphero-linux-api/libsphero.so doesn't exist. Using makefile in sphero-linux-api..."
	make -C sphero-linux-api/
fi
#test if the folder sphero exist
if [ -d sphero/ ]
then
	echo "folder sphero/ already exist."
else
	echo "folder sphero/ doesn't exist. Creating..."
	mkdir sphero
	#copy the folder structure of sphero-linux-api/src/
	cd sphero-linux-api/src/
	find -type d | xargs -i mkdir ../../sphero/{} 2> /dev/null
	#copy header of api in the sphero folder
	find -type f | grep -i ".h$\|.hpp$\|.tpp" | xargs -i cp {} ../../sphero/{}
	cd ../..
fi

#now compile my files
echo "   Compile ${FILE}.cpp"
g++ -std=c++11 -c ${FILE}.cpp && g++ -Lsphero-linux-api/ -o ${FILE} ${FILE}.o -lsphero
echo "define variable LD_LIBRARY_PATH=sphero-linux-api/"
export LD_LIBRARY_PATH=sphero-linux-api/
