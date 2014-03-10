#ifndef MYORCER_H
#define MYORCER_H


namespace Core{

	class MyOCRer{

	public:
		MyOCRer(){ img = NULL;}
		~MyOCRer(){ if(img != NULL){ cvReleaseImage(&img);}}

	public:
		void loadConfig(char* config_file);
		bool loadImage(char* file);
		void textDetection();

		string getDetectText(){ return text_detect;}

	private:
		TessWrapper myTessWrapper;
		string text_detect;
		IplImage* img;
	};
}

#endif
