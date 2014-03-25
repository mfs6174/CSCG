/*
ID: mfs6174
email: mfs6174@gmail.com
PROG: roiHist
LANG: C++
*/

#include<iostream>
#include<string>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

struct histCmp
{
  Mat hsv;
  void load(const Mat & img){cvtColor(img,hsv,CV_BGR2HSV);};
  double compare(Rect r1,Rect r2);
  
};

double histCmp::compare(Rect r1,Rect r2)
{
  int hbins = 9, sbins = 4;
  int histSize[] = {hbins, sbins};
  // hue varies from 0 to 179, see cvtColor
  float hranges[] = { 0, 180 };
  // saturation varies from 0 (black-gray-white) to
  // 255 (pure spectrum color)
  float sranges[] = { 0, 256 };
  const float* ranges[] = { hranges, sranges };
  MatND hist1,hist2;
  // we compute the histogram from the 0-th and 1-st channels
  int channels[] = {0, 1};
  Mat tmp;
  tmp=Mat(hsv,r1);
  calcHist( &tmp, 1, channels, Mat(), // do not use mask
            hist1, 2, histSize, ranges,
            true, // the histogram is uniform
            false );
  normalize( hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat() );
  tmp=Mat(hsv,r2);
  calcHist( &tmp, 1, channels, Mat(), // do not use mask
            hist2, 2, histSize, ranges,
            true, // the histogram is uniform
            false );
  normalize( hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat() );
  return compareHist(hist1,hist2,  CV_COMP_BHATTACHARYYA );
}

struct selecter
{
  int status;
  Rect sel1,sel2;
  Mat oImage,showImage,tmpImage;
  void reset(){status=0;};
};

histCmp mcmp;
selecter sel;

void onMouse(int event,int x,int y,int flags,void*param)  
{  
    Point p1,p2;  
    if(event==CV_EVENT_LBUTTONDOWN && (sel.status==0 || sel.status==2))  
    {
      if (sel.status==0)
      {
        sel.sel1.x=x;
        sel.sel1.y=y;
      }
      else
      {
        sel.sel2.x=x;
        sel.sel2.y=y;
      }
      sel.status++;
      return;
    }  
    if( (sel.status==1 || sel.status==3)  &&event == CV_EVENT_MOUSEMOVE)  
    {
      if (sel.status==1)
      {
        sel.oImage.copyTo(sel.showImage);  
        p1 = Point(sel.sel1.x,sel.sel1.y);  
        p2 = Point(x,y);      
        rectangle(sel.showImage,p1,p2,Scalar(0,255,0),2);  
        imshow("img",sel.showImage);
      }
      else
      {
        sel.tmpImage.copyTo(sel.showImage);  
        p1 = Point(sel.sel2.x,sel.sel2.y);  
        p2 = Point(x,y);      
        rectangle(sel.showImage,p1,p2,Scalar(0,255,0),2);  
        imshow("img",sel.showImage);
      }
      return;
    }  
    if((sel.status==1 || sel.status==3) && event == CV_EVENT_LBUTTONUP)  
    {
      int xx,yy,zx,zy;
      if (sel.status==1)
      {
        xx=sel.sel1.x;
        yy=sel.sel1.y;
      }
      else
      {
        xx=sel.sel2.x;
        yy=sel.sel2.y;
      }
      zx=min(xx,x);
      zy=min(yy,y);
      xx=max(xx,x);
      yy=max(yy,y);
      if (sel.status==1)
      {
        if (yy-zy>0 && yy-zy<sel.oImage.rows && xx-zx>0 && xx-zx<sel.oImage.cols)
        {
          sel.sel1.x=zx;
          sel.sel1.y=zy;
          sel.sel1.width=xx-zx;
          sel.sel1.height=yy-zy;
          sel.status=2;
          sel.showImage.copyTo(sel.tmpImage);
        }
        else
        {
          cout<<"not a valid rect, please reselect"<<endl;
          sel.status=0;
          imshow("img",sel.oImage);
        }
      }
      else
      {
        if (yy-zy>0 && yy-zy<sel.oImage.rows && xx-zx>0 && xx-zx<sel.oImage.cols)
        {
          sel.sel2.x=zx;
          sel.sel2.y=zy;
          sel.sel2.width=xx-zx;
          sel.sel2.height=yy-zy;
          sel.status=0;
          cout<<"error is "<<mcmp.compare(sel.sel1,sel.sel2)<<endl;
          imshow("img",sel.oImage);
          
        }
        else
        {
          cout<<"not a valid rect, please reselect"<<endl;
          sel.status=2;
          imshow("img",sel.tmpImage);
        }

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

  myimage.copyTo(sel.oImage);
  mcmp.load(myimage);
  namedWindow("img");  
  imshow("img",sel.oImage);
  sel.reset();
  setMouseCallback("img",onMouse,0);  
  waitKey(0);
  return 0;
}
  
