GPP=g++ -g -pg -Wall `pkg-config --libs --cflags opencv` -msse -msse2 -msse3 -o
All :	main.o  MERCInput.o MERCOutput.o MERCDisjointSet.o MERCFunctions.o MERCLazyGreedy.o
	$(GPP) ERS.bin main.o  MERCInput.o MERCOutput.o MERCDisjointSet.o MERCFunctions.o MERCLazyGreedy.o
main.o:	main.cpp  MERCLazyGreedy.h MERCInputImage.h MERCOutputImage.h Image.h ImageIO.h
	$(GPP) main.o -c main.cpp
MERCInput.o:	MERCInput.cpp	MERCInput.h
	$(GPP) MERCInput.o -c MERCInput.cpp	
MERCOutput.o:	MERCOutput.cpp	MERCOutput.h
	$(GPP) MERCOutput.o -c MERCOutput.cpp	
MERCDisjointSet.o:	MERCDisjointSet.cpp	MERCDisjointSet.h MList.h
	$(GPP) MERCDisjointSet.o -c MERCDisjointSet.cpp	
MERCFunctions.o:	MERCFunctions.cpp	MERCFunctions.h
	$(GPP) MERCFunctions.o -c MERCFunctions.cpp	
MERCLazyGreedy.o:	MERCLazyGreedy.cpp	MERCLazyGreedy.h MERCClustering.h MERCDisjointSet.h MERCInput.h MERCEdge.h MSubmodularHeap.h MERCFunctions.h
	$(GPP) MERCLazyGreedy.o -c MERCLazyGreedy.cpp	
