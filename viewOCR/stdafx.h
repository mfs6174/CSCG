// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef STDAFX_H
#define STDAFX_H

//#define WINAPP
#ifdef WINAPP

#include "targetver.h"
#include <tchar.h>

#else

#include "auto_tchar.h"

#endif

#include <stdio.h>



// TODO: reference additional headers your program requires here

#include <cstdio>
#include <iostream>
#include <string>
#include <cmath>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <climits>
#include <locale.h>

#include<fstream>
#include<sstream>
#include<cstring>
#include<algorithm>
#include<map>
#include<vector>
#include<queue>
#include<deque>
#include<iomanip>
#include<set>
#define sf scanf
#define pf printf
#define llg long long 

const int maxlongint=2147483647;

#include "opencv2/opencv.hpp"

#include "tinyXML/XMLDoc.h"
#include "tinyXML/tinyxml.h"

using namespace std;
using namespace cv;

template<class T> class Image
{
  private:
  IplImage* imgp;
  public:
  Image(IplImage* img=0) {imgp=img;}
  ~Image(){imgp=0;}
  void operator=(IplImage* img) {imgp=img;}
  inline T* operator[](const int rowIndx) {
    return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
}; 
 
typedef struct{
  unsigned char b,g,r;
} RgbPixel; 
 
typedef struct{
  float b,g,r;
} RgbPixelFloat; 
 
typedef Image<RgbPixel>       RgbImage;
typedef Image<RgbPixelFloat>  RgbImageFloat;
typedef Image<unsigned char>  BwImage;
typedef Image<float>          BwImageFloat;


template<typename T> inline T sqr(T x) { return x * x;}
template<typename T> inline T clamp(T x, T lo, T hi) {return x < lo ? lo : (x > hi ? hi : x);}


#define doForPixel(img, i, j) for(int i = 0;i<img->height;i++) for(int j = 0;j<img->width;j++)
#define doForPixel2(img, i, j) for(int i = 0;i<img.rows;i++) for(int j = 0;j<img.cols;j++)
#define colorDistL2(c1, c2) sqrt(sqr(c1.val[0]-c2.val[0])+sqr(c1.val[1]-c2.val[1])+sqr(c1.val[2]-c2.val[2]))
#define colorDistL1(c1, c2) fabs(c1.val[0]-c2.val[0])+fabs(c1.val[1]-c2.val[1])+fabs(c1.val[2]-c2.val[2])

#define posDistL2(p1,p2) sqrt(sqr(p1.x-p2.x) + sqr(p1.y-p2.y))

#ifdef WINAPP

#ifdef _DEBUG
#pragma comment(lib,"User32.lib")
#pragma comment(lib, "opencv_core240d.lib")
#pragma comment(lib, "opencv_highgui240d.lib")
#pragma comment(lib, "opencv_imgproc240d.lib")
#else
#pragma comment(lib, "opencv_core240.lib")
#pragma comment(lib, "opencv_highgui240.lib")
#pragma comment(lib, "opencv_imgproc240.lib")
#pragma comment(lib,"User32.lib")
#endif

#endif

#endif
