GPP=g++ -g -pg -Wall `pkg-config --libs --cflags opencv` -msse -msse2 -msse3 -o
All :	main.o SLIC.o
	$(GPP) SLIC.bin main.o SLIC.o
main.o:	main.cpp  SLIC.h
	$(GPP) main.o -c main.cpp
SLIC.o:	SLIC.cpp SLIC.h
	$(GPP) SLIC.o -c SLIC.cpp
