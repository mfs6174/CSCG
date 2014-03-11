include ../Makefile.common
All : main.o
	$(ARR)  tess.a main.o
main.o :TessWrapper.cpp TessWrapper.h
	$(GCC)  main.o -c TessWrapper.cpp

clean:
	rm -rf *.o *.a

