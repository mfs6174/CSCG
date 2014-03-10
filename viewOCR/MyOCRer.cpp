#include "stdafx.h"
#include "config.h"
#include "helpfunc.h"
#include "SWT/TextDetection.h"
#include "Tesseract/TessWrapper.h"
#include "MyOCReader.h"

namespace Core{

	void MyOCRer::loadConfig(char* config_file){
		tixml::XMLDoc doc;
		doc.Load(config_file);
		input_filename = doc.get<string>("input.filename", 0);
		ocr_lang = doc.get<string>("input.language", 0);

		meanshift_sp = doc.get<double>("parameter.mean-shift.@sp", 0);
		meanshift_sr = doc.get<double>("parameter.mean-shift.@sr", 0);

		bilateral_size1 = doc.get<int>("parameter.bilateral.@size1", 0);
		bilateral_size2 = doc.get<int>("parameter.bilateral.@size2", 0);
		bilateral_sigma1 = doc.get<double>("parameter.bilateral.@sigma1", 0);
		bilateral_sigma2 = doc.get<double>("parameter.bilateral.@sigma2", 0);

		swt_canny_ratio = doc.get<double>("parameter.swt.@canny-non-edge-ratio", 0);
		swt_dark_on_light = doc.get<int>("parameter.swt.@dark-on-light", 0);
		swt_filter_variance_mean_ratio = doc.get<double>("parameter.swt.@fliter-variance-mean-ratio", 0);

		segment_sigma = doc.get<double>("parameter.segment.@sigma", 0);
		segment_c = doc.get<double>("parameter.segment.@c", 0);
		segment_minsize = doc.get<int>("parameter.segment.@min-size", 0);

		merge_color_thres = doc.get<double>("parameter.merge.@color-thres", 0);
		merge_swt_thres = doc.get<double>("parameter.merge.@swt-thres", 0);

	}

	bool MyOCRer::loadImage(char* file){
		if(img != NULL){
			cvReleaseImage(&img);
		}
		if((img = cvLoadImage(file , 1)) == NULL){
			printf("1\n");
			return false;
		}
		printf("0\n");
		return true;
	}

	void MyOCRer::textDetection(){

		//IplImage *newImage = cvLoadImage(input_filename.c_str());
		if(img == NULL) return;
		IplImage *new_swt_text = swt::textDetection(img, swt_dark_on_light);
		cvThreshold(new_swt_text, new_swt_text, 254, 255, CV_THRESH_BINARY);
		cvSaveImage("swt.tif", new_swt_text);
		char * text = myTessWrapper.recognize("swt.tif", ocr_lang);
		text_detect = UTF8ToGBK(text);
		std::cout << UTF8ToGBK( text ) << std::endl;
	}
}
