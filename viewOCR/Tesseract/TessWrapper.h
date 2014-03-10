#ifndef TESSWRAPPER_H
#define TESSWRAPPER_H

class TessWrapper {
public:
	TessWrapper() {}
	void test();
	char* recognize(std::string tiffile, std::string lang = std::string("eng"));
protected:
private:
};
#endif
