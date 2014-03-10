#include "stdafx.h"
#include "MyOCReader.h"
#include "Test.h"

namespace Core{

	void Test::test1(){
		tixml::XMLDoc doc;
		doc.Load("input.xml");
		string imgBase = doc.get<string>("input.imgBase" , 0);
		string imgSaveBase = doc.get<string>("input.imgSaveBase" , 0);
		int imgNum = doc.get<int>("input.imgNum" , 0);


		char buf[255];
		for(int i=1;i<=imgNum;i++){
			MyOCRer ocr;
			sprintf(buf, "%s%d%s" , imgBase.c_str() , i , ".jpg");
			IplImage* output6 = cvLoadImage(buf , 1);
			//system("pause");
			printf("%s\n", buf);
			if(ocr.loadImage(buf) == false) continue;
			ocr.loadConfig("config.xml");
			ocr.textDetection();
			string text = ocr.getDetectText();

			// save 
			sprintf(buf , "%s%d%s" , imgSaveBase.c_str() , i , "_ray.png");
			IplImage* output1 = cvLoadImage("rays.png" , 1);
			cvSaveImage(buf, output1);

			sprintf(buf , "%s%d%s" , imgSaveBase.c_str() , i , "_swt.png");
			IplImage* output2 = cvLoadImage("SWT.png" , 1);
			cvSaveImage(buf, output2);		

			sprintf(buf , "%s%d%s" , imgSaveBase.c_str() , i , "_cmp.png");
			IplImage* output3 = cvLoadImage("components.png" , 1);
			cvSaveImage(buf, output3);

			sprintf(buf , "%s%d%s" , imgSaveBase.c_str() , i , "_text.png");
			IplImage* output4 = cvLoadImage("text.png" , 1);
			cvSaveImage(buf, output4);

			sprintf(buf , "%s%d%s" , imgSaveBase.c_str() , i , "_canny.png");
			IplImage* output5 = cvLoadImage("canny.png" , 1);
			cvSaveImage(buf, output5);

			sprintf(buf , "%s%d%s" , imgSaveBase.c_str() , i , ".png");
			//IplImage* output4 = cvLoadImage("text.png" , 1);
			cvSaveImage(buf, output6);

			sprintf(buf, "%s%d%s" , imgSaveBase.c_str() , i , ".txt"); 
			FILE *fp = fopen(buf, "w+");
			fprintf(fp, "%s\n" , text.c_str());
			fclose(fp);
		}

	}
}
