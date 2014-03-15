#ifndef ER_H
#define ER_H

#include "opencv2/opencv.hpp"
#include <vector>
typedef unsigned char uchar;
using namespace std;
using namespace cv;

struct ExRegion
{
public:
  int cnt;
  Mat input,output,labImage,hsvImage;
  vector<Mat> srgb,slab,shsv;
  vector< vector<Point2i> > blob;
  vector< vector<int> > belong;

  void init(Mat _input);
  void calc(char _ch,int threshold,bool flip);

private:
  int sw,sh,threshold;
  Mat chan;
  bool flip;
  void _floodfill(int x,int y);
  bool flag;
};
  
#endif
