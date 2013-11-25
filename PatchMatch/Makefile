GPP=g++ -g -pg -Wall `pkg-config --libs --cflags opencv` -msse -msse2 -msse3 -o
All :	main.o PM.o
	$(GPP) pmatch main.o PM.o
main.o:	main.cpp inc.h PM.h
	$(GPP) main.o -c main.cpp
PM.o:	PM.cpp inc.h PM.h
	$(GPP) PM.o -c PM.cpp