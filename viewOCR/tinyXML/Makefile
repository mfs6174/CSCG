include ../Makefile.common

All : xml.o doc.o error.o parser.o
	$(ARR)  txml.a xml.o doc.o error.o parser.o
xml.o :tinyxml.cpp tinyxml.h
	$(GCC)  xml.o -c tinyxml.cpp
doc.o :XMLDoc.cpp XMLDoc.h
	$(GCC)  doc.o -c XMLDoc.cpp
error.o :tinyxmlerror.cpp tinyxml.h
	$(GCC)  error.o -c tinyxmlerror.cpp
parser.o :tinyxmlparser.cpp tinyxml.h
	$(GCC)  parser.o -c tinyxmlparser.cpp 

clean:
	rm -rf *.o *.a

