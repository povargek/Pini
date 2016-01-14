GPP=g++
OUTFILE="Bin/Pini.so"
COMPILE_FLAGS=-m32 -std=c++11 -c -O3 -w -DLINUX -ISDK/amx/

all:
	$(GPP) $(COMPILE_FLAGS) SDK/*.cpp	
	$(GPP) $(COMPILE_FLAGS) *.cpp	
	$(GPP) -m32 -O -pthread -unistd -fshort-wchar -shared -o $(OUTFILE) *.o
	rm *.o