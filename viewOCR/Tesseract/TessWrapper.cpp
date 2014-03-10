#include "../stdafx.h"
#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"
#include "TessWrapper.h"

void TessWrapper::test() {
	char *outText;

	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	// Open input image with leptonica library
	Pix *image = pixRead("./swt.tif");
	api->SetImage(image);
	// Get OCR result
	api->SetVariable("save_blob_choices", "T");
	api->Recognize(NULL);
	tesseract::ResultIterator* ri = api->GetIterator();
	tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
	if(ri != 0) {
		do {
			const char* symbol = ri->GetUTF8Text(level);
			float conf = ri->Confidence(level);
			if(symbol != 0) {
				printf("symbol %s, conf: %f", symbol, conf);
				bool indent = false;
				tesseract::ChoiceIterator ci(*ri);
				do {
					if (indent) printf("\t\t ");
					printf("\t- ");
					const char* choice = ci.GetUTF8Text();
					printf("%s conf: %f\n", choice, ci.Confidence());
					indent = true;
				} while(ci.Next());
			}
			printf("---------------------------------------------\n");
			delete[] symbol;
		} while((ri->Next(level)));
	}

	// Destroy used object and release memory
	api->End();
	//delete []outText;
	pixDestroy(&image);
}

char* TessWrapper::recognize(std::string tiffile, std::string lang) {
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// the language, and Chinese is "chi_sim"
	if (api->Init(NULL, lang.c_str(), tesseract::OEM_DEFAULT)) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	Pix *image = pixRead(tiffile.c_str());
	printf("Finish\n");
	api->SetImage(image);
	char *res = api->GetUTF8Text();
	api->End();
	pixDestroy(&image);
	return res;
}
