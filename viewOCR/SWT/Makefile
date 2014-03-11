include ../Makefile.common


All : main.o det.o
	make LIB
	$(GCC) swtDet.bin main.o det.o ../helpfunc.o ../config.o
main.o :FeaturesMain.cpp TextDetection.h
	$(GCC)  main.o -c FeaturesMain.cpp
det.o: TextDetection.cpp TextDetection.h
	$(GCC)  det.o -c TextDetection.cpp 
LIB : det.o
	$(ARR)  swt.a det.o ../helpfunc.o ../config.o
clean:
	rm -rf *.o *.a swtDet swtDet.bin

