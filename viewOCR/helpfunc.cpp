#include "stdafx.h"
#include "helpfunc.h"

using std::map;

CvScalar operator * (CvScalar &s, float c){
	return cvScalar(s.val[0]*c, s.val[1]*c, s.val[2]*c);
}
CvScalar operator + (CvScalar &s1, CvScalar &s2){
	return cvScalar(s1.val[0]+s2.val[0], s1.val[1]+s2.val[1], s1.val[2]+s2.val[2]);
}
CvScalar operator - (CvScalar &s1, CvScalar &s2){
	return cvScalar(s1.val[0]-s2.val[0], s1.val[1]-s2.val[1], s1.val[2]-s2.val[2]);
}
CvScalar operator ^ (CvScalar &s, float c){
	return cvScalar(pow((float)s.val[0], c), pow((float)s.val[1], c), pow((float)s.val[2], c));
}

CvPoint2D32f operator + (CvPoint2D32f &p1, CvPoint2D32f &p2) {
	return cvPoint2D32f(p1.x + p2.x, p1.y + p2.y);
}

CvPoint2D32f operator - (CvPoint2D32f &p1, CvPoint2D32f &p2) {
	return cvPoint2D32f(p1.x - p2.x, p1.y - p2.y);
}

CvPoint2D32f operator * (CvPoint2D32f &p1, float f) {
	return cvPoint2D32f(p1.x * f, p1.y * f);
}

CvPoint2D32f operator * (CvPoint2D32f &p1, double d) {
	return cvPoint2D32f(p1.x * d, p1.y * d);
}

void adaptiveFindCannyThreshold(CvMat *dx, CvMat *dy, double precentage, double *low, double *high)
{
	CvSize size;
	IplImage *imge=0;
	int i,j;
	CvHistogram *hist;
	int hist_size = 255;
	float range_0[]={0,256};
	float* ranges[] = { range_0 };
	double PercentOfPixelsNotEdges = precentage;
	size = cvGetSize(dx);
	imge = cvCreateImage(size, IPL_DEPTH_32F, 1);
	// edge correspondence
	float maxv = 0;
	for(i = 0; i < size.height; i++ )
	{
		const short* _dx = (short*)(dx->data.ptr + dx->step*i);
		const short* _dy = (short*)(dy->data.ptr + dy->step*i);
		float* _image = (float *)(imge->imageData + imge->widthStep*i);
		for(j = 0; j < size.width; j++)
		{
			_image[j] = (float)(abs(_dx[j]) + abs(_dy[j]));
			maxv = maxv < _image[j] ? _image[j]: maxv;
		}
	}
	// the histogram
	range_0[1] = maxv;
	hist_size = (int)(hist_size > maxv ? maxv:hist_size);
	hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	cvCalcHist( &imge, hist, 0, NULL );
	int total = (int)(size.height * size.width * PercentOfPixelsNotEdges);
	float sum=0;
	int icount = hist->mat.dim[0].size;

	float *h = (float*)cvPtr1D( hist->bins, 0 );
	for(i = 0; i < icount; i++)
	{
		sum += h[i];
		if( sum > total )
			break; 
	}
	// get threshold
	*high = (i+1) * maxv / hist_size ;
	*low = *high * 0.4;
	cvReleaseImage( &imge );
	cvReleaseHist(&hist);
}

void adaptiveFindCannyThreshold(IplImage *img, double precentage, double *low, double *high) {
	CvMat *dx, *dy;
	dx = cvCreateMat(img->height, img->width, CV_16SC1);
	dy = cvCreateMat(img->height, img->width, CV_16SC1);
	cvSobel(img, dx, 1, 0, 3);
	cvSobel(img, dy, 0, 1, 3);
	adaptiveFindCannyThreshold(dx, dy, precentage, low, high);
	cvReleaseMat(&dx);
	cvReleaseMat(&dy);
}

int meanshiftOpenCV(Mat &src, Mat &pImgInd) {
	Mat tmp;
	pyrMeanShiftFiltering(src, tmp, 20, 2);
	imshow("seg",tmp);
	waitKey();
	map<int, int> marker;
	pImgInd.create(src.size(), CV_32S);

	int idxNum = 0;
	int height = src.rows, width = src.cols;
	for (int y = 0; y < height; y++) {
		int *imgIdx = pImgInd.ptr<int>(y);
		for (int x = 0; x < width; x++) {
			//int comp = u->find(y * width + x);
			Vec3b color = tmp.at<Vec3b>(y,x);
			int comp = (color[0] * 256 * 256) + (color[1] *256) + color[2];
			if (marker.find(comp) == marker.end())
				marker[comp] = idxNum++;

			int idx = marker[comp];
			imgIdx[x] = idx;
		}
	}  
	return idxNum;
}

double get_mean(vector<double> &vd) {
	double sum = std::accumulate(vd.begin(), vd.end(), 0.0);
	double mu = sum / double(vd.size());
	return mu;
}

double get_varriance(vector<double> &vd) {
	double mu = get_mean(vd);
	double sd = 0;
	//for (double d: vd) {
	for(int i=0;i<vd.size();i++){
		sd += sqr(vd[i] - mu);
	}
	return sd / double(vd.size());
}

#ifdef WINAPP
string UTF8ToGBK(const std::string& strUTF8)    
{    
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);    
	unsigned short * wszGBK = new unsigned short[len + 1];    
	memset(wszGBK, 0, len * 2 + 2);    
	MultiByteToWideChar(CP_UTF8, 0,LPCSTR(strUTF8.c_str()), -1, LPWSTR(wszGBK), len);    

	len = WideCharToMultiByte(CP_ACP, 0,LPCTSTR(wszGBK), -1, NULL, 0, NULL, NULL);    
	char *szGBK = new char[len + 1];    
	memset(szGBK, 0, len + 1);    
	WideCharToMultiByte(CP_ACP,0, LPCTSTR(wszGBK), -1, szGBK, len, NULL, NULL);    
	//strUTF8 = szGBK;    
	std::string strTemp(szGBK);    
	delete[]szGBK;    
	delete[]wszGBK;    
	return strTemp;    
}
#endif
