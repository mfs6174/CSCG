GPP=g++ -g -pg -Wall `pkg-config --libs --cflags opencv` -msse -msse2 -msse3 -o
All :	main.o ER.o
	$(GPP) ER.bin main.o ER.o
main.o:	main.cpp  ER.h
	$(GPP) main.o -c main.cpp
ER.o:	ER.cpp ER.h
	$(GPP) ER.o -c ER.cpp
