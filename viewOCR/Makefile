include Makefile.common
All : main.o myocr.o config.o helpfunc.o test.o MSWT Tess Txml
	$(GPP)  viewOCR.bin main.o myocr.o config.o helpfunc.o test.o Tesseract/tess.a SWT/swt.a  tinyXML/txml.a
main.o :ViewOCR.cpp Test.h stdafx.h
	$(GPP)  main.o -c ViewOCR.cpp
myocr.o :MyOCRer.cpp MyOCReader.h Tesseract/TessWrapper.h SWT/TextDetection.h helpfunc.h config.h  stdafx.h
	$(GPP)  myocr.o -c MyOCRer.cpp
config.o: config.cpp config.h stdafx.h
	$(GPP)  config.o -c config.cpp
helpfunc.o: helpfunc.cpp helpfunc.h stdafx.h
	$(GPP)  helpfunc.o -c helpfunc.cpp
test.o: test.cpp Test.h MyOCReader.h stdafx.h
	$(GPP)  test.o -c test.cpp

clean:
	rm -rf *.o viewOCR viewOCR.bin
	make -C Tesseract clean
	make -C SWT clean
	make -C tinyXML clean
MSWT:
	make -C SWT
Tess:
	make -C Tesseract
Txml:
	make -C tinyXML
