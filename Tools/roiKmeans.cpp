/*
ID: mfs6174
email: mfs6174@gmail.com
PROG: roiKmeans
LANG: C++
*/

#include<iostream>
#include<string>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

inline float sqr(float x)
{
  return x*x;
}
  
inline float diff(const Mat &img3f, int x1, int y1, int x2, int y2)
{
	const Vec3f &p1 = img3f.at<Vec3f>(y1, x1);
	const Vec3f &p2 = img3f.at<Vec3f>(y2, x2);
	return sqrt(sqr(p1[0] - p2[0]) + sqr(p1[1] - p2[1]) + sqr(p1[2] - p2[2]));
}

struct KmeansC
{
  Mat lab,gray,grad,origin;
  int width,height;
  void load(const Mat & img)
  {
    img.copyTo(origin);
    cvtColor( img, gray, CV_BGR2GRAY );
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    Sobel( gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    Sobel( gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
    img.convertTo(lab,CV_32FC3,1.0/255);
    cvtColor(lab,lab,CV_BGR2Lab);
    width=img.cols;
    height=img.rows;
    const int grDist=2;
    grad=Mat::zeros(height,width,CV_32F);
    for (int i=0;i<height;i++)
      for (int j=0;j<width;j++)
        for (int o=-grDist;o<=grDist;o++)
          for (int p=-grDist;p<=grDist;p++)
          {
            if (i+o < 0 || i+o>=height || j+p<0 ||j+p>=width )
              continue;
            grad.at<float>(i,j)=max(grad.at<float>(i,j),diff(lab,j,i,j+p,i+o));
          }
    normalize( grad, grad, 0, 1, NORM_MINMAX, -1, Mat() );
    imshow("grad",grad);
  }
  double compute(Rect r1);
  double computeG(Rect r1);
};

double KmeansC::compute(Rect r1)
{
  Mat mydata(r1.height*r1.width,1,CV_32FC3),labels;
  Mat centers(2, 1, mydata.type());
  for (int i=r1.y;i<r1.y+r1.height;i++)
    for (int j=r1.x;j<r1.x+r1.width;j++)
      mydata.at<Vec3f>((i-r1.y)*r1.width+j-r1.x,0)=lab.at<Vec3f>(i,j);
  double rst=kmeans(mydata, 3, labels,
                    TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 50, 0.1),
                    5, KMEANS_PP_CENTERS,centers);
  for (int i=r1.y;i<r1.y+r1.height;i++)
    for (int j=r1.x;j<r1.x+r1.width;j++)
    {
      if (labels.at<int>((i-r1.y)*r1.width+j-r1.x,0)==2)
        origin.at<Vec3b>(i,j)=Vec3b(128,128,128);
      else
        origin.at<Vec3b>(i,j)=Vec3b(labels.at<int>((i-r1.y)*r1.width+j-r1.x,0)*255,labels.at<int>((i-r1.y)*r1.width+j-r1.x,0)*255,labels.at<int>((i-r1.y)*r1.width+j-r1.x,0)*255);
    }
  return rst/(r1.height*r1.width);
}
double KmeansC::computeG(Rect r1)
{
  Mat tmp(grad,r1);
  Scalar mn=mean(tmp);
  return mn[0];
}

struct selecter
{
  int status;
  Rect box;
  Mat oImage,showImage,tmpImage;
  void reset(){status=0;};
};

KmeansC mcmp;
selecter sel;

void onMouse(int event,int x,int y,int flags,void*param)  
{  
    Point p1,p2;  
    if(event==CV_EVENT_LBUTTONDOWN && (sel.status==0))  
    {
      sel.box.x=x;
      sel.box.y=y;
      sel.status++;
      return;
    }  
    if( (sel.status==1)  &&event == CV_EVENT_MOUSEMOVE)  
    {
    
      sel.oImage.copyTo(sel.showImage);  
      p1 = Point(sel.box.x,sel.box.y);  
      p2 = Point(x,y);      
      rectangle(sel.showImage,p1,p2,Scalar(0,255,0),2);  
      imshow("img",sel.showImage);
      return;
    }  
    if((sel.status==1) && event == CV_EVENT_LBUTTONUP)  
    {
      int xx,yy,zx,zy;
      xx=sel.box.x;
      yy=sel.box.y;
      zx=min(xx,x);
      zy=min(yy,y);
      xx=max(xx,x);
      yy=max(yy,y);
      if (yy-zy>0 && yy-zy<sel.oImage.rows && xx-zx>0 && xx-zx<sel.oImage.cols)
      {
        sel.box.x=zx;
        sel.box.y=zy;
        sel.box.width=xx-zx;
        sel.box.height=yy-zy;
        cout<<"square error is "<<mcmp.compute(sel.box)<<endl;
        cout<<"mean grad  is "<<mcmp.computeG(sel.box)<<endl;
        sel.status=0;
        sel.oImage.copyTo(sel.showImage);
        Rect &r1=sel.box;
        for (int i=r1.y;i<r1.y+r1.height;i++)
          for (int j=r1.x;j<r1.x+r1.width;j++)
            sel.showImage.at<Vec3b>(i,j)=mcmp.origin.at<Vec3b>(i,j);
        imshow("img",sel.showImage);
        imwrite("tmp.jpg",sel.showImage);
      }
      else
      {
        cout<<"not a valid rect, please reselect"<<endl;
        sel.status=0;
        imshow("img",sel.oImage);
      }
    }     
}  
int main(int argc, char *argv[])
{
  if (argc<2)
  {
    cout<<"please give an image"<<endl;
    return-1;
  }
  Mat myimage=imread(argv[1]);
  namedWindow("grad");
  myimage.copyTo(sel.oImage);
  mcmp.load(myimage);
  namedWindow("img");
  imshow("img",sel.oImage);
  sel.reset();
  setMouseCallback("img",onMouse,0);  
  waitKey(0);
  return 0;
}
  
