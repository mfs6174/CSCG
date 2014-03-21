#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Saliency.h"

int cnt,sw,sh;
vector< vector<Point2i> > blob;
vector< vector<int> > belong;
vector<bool> cls;
bool flag;
Mat mask;

void _floodfill(int x,int y)
{
   if (x<1||y<1||x>sh||y>sw)
   {
     cls[cnt-1]=true;
     return;
   }
   if (belong[x][y])
     return;
   if (mask.at<uchar>(x-1,y-1)!=0)
     return;
   if (flag)
   {
     flag=false;
     cnt++;
     blob.push_back(vector<Point2i>());
     blob[cnt-1].push_back(Point2i(x-1,y-1));
     belong[x][y]=cnt;
     cls.push_back(false);
   }
   else
   {
     belong[x][y]=cnt;
     blob[cnt-1].push_back(Point2i(x-1,y-1));
   }
  _floodfill(x+1,y);
  _floodfill(x,y+1);
  _floodfill(x-1,y);
  _floodfill(x,y-1);
}

int main(int argc, char *argv[])
{
  if (argc<2)
  {
    cout<<"please give an image"<<endl;
    return 0;
  }
  if (argc>2)
  {
    ifstream inf(argv[2]);
    string imname;
    Mat img3f,outimage,wimage;
    int th,pt;
    cout<<"input th pt"<<endl;
    cin>>th>>pt;
    while (inf>>imname)
    {
      cout<<"processing "<<imname<<endl;
      img3f=imread(imname);
      img3f.copyTo(wimage);
      img3f.convertTo(img3f, CV_32FC3, 1.0/255);
      outimage = Saliency::gFuns[pt](img3f);
      cout<<"done sal"<<endl;
      outimage.convertTo(outimage,CV_8U,255);
      imwrite(imname+"__SAL.jpg",outimage);
      threshold(outimage,mask,th,255,THRESH_BINARY);
      cnt=0;
      sw=img3f.cols;
      sh=img3f.rows;
      belong=vector< vector<int> > (sh+1,vector<int>(sw+1,0));
      blob.clear();
      for (int i=1;i<=sh;i++)
        for (int j=1;j<=sw;j++)
        {
          flag=true;
          _floodfill(i,j);
        }
      for (int i=1;i<=cnt;i++)
        if (!cls[i-1])
        {
          // cout<<"fill "<<blob[i-1].size()<<endl;
          for (int j=0;j<blob[i-1].size();j++)
            mask.at<uchar>(blob[i-1][j].x,blob[i-1][j].y)=255;
        }
      for (int i=0;i<wimage.rows;i++)
        for (int j=0;j<wimage.cols;j++)
        {
          Vec3b &vv=wimage.at<Vec3b>(i,j);
          if (mask.at<uchar>(i,j)==0)
            vv=Vec3b(0,0,0);
        }
      imwrite(imname+"__mask.jpg",wimage);
    }
  }
  else
  {
    Mat img3f=imread(argv[1]),outimage;
    namedWindow("SAL",1);
    img3f.convertTo(img3f, CV_32FC3, 1.0/255);
    outimage = Saliency::gFuns[0](img3f);
    imshow("SAL",outimage);
    outimage.convertTo(outimage,CV_8UC3,255);
    imwrite(string(argv[1])+"__SAL.jpg",outimage);
    waitKey(0);
  }
  return 0;
}
  


