#ifndef HELPFUNC_H
#define HELPFUNC_H


CvScalar operator * (CvScalar &s, float c);
CvScalar operator + (CvScalar &s, CvScalar &c);
CvScalar operator - (CvScalar &s, CvScalar &c);
CvScalar operator ^ (CvScalar &s, float c);

CvPoint2D32f operator + (CvPoint2D32f &p1, CvPoint2D32f &p2);
CvPoint2D32f operator - (CvPoint2D32f &p1, CvPoint2D32f &p2);
CvPoint2D32f operator * (CvPoint2D32f &p1, float f);
CvPoint2D32f operator * (CvPoint2D32f &p1, double d);

void adaptiveFindCannyThreshold(IplImage *img, double precentage, double *low, double *high);

int meanshiftOpenCV(Mat &src, Mat &dst);

double get_mean(vector<double> &vd);

double get_varriance(vector<double> &vd);

string UTF8ToGBK(const std::string& strUTF8);

#endif
