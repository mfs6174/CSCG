#include <string>
#include <cstring>
#include <iostream>
#include "SLIC.h"


SLIC slic;
const int minsize=20;
const double ratio2=0.95,ratio1=0.85;

int check2Color(const Mat & image,const vector<Point2i> &bin)
{
  int tong[8][8][8]={0};
  for (int k=0;k<bin.size();k++)
  {
    int i=bin[k].x,j=bin[k].y;
    const Vec3b &vv=image.at<Vec3b>(i,j);
    tong[vv[0]/32][vv[1]/32][vv[2]/32]++;
  }
  int mm=0,mx=0;
  for (int i=0;i<8;i++)
    for (int j=0;j<8;j++)
      for (int k=0;k<8;k++)
      {
        if (tong[i][j][k]>mm)
        {
          mm=tong[i][j][k];
          mx=mm;
        }
        else
          if (tong[i][j][k]>mx)
            mx=tong[i][j][k];
      }
  //cout<<mm<<' '<<bin.size()<<endl;
  if ((double)mm/bin.size()>=ratio1)
    return 1;
  if ( (double)(mm+mx)/bin.size()>=ratio2)
    
    return 2;
  return 0;
}

int main(int argc, char *argv[])
{
  if (argc<2)
  {
    cout<<"please give at least one image"<<endl;
    return 0;
  }
  if (argc==3)
  {
    Mat myimage=imread(argv[1]),outimage,smask,wimage;
    myimage.copyTo(wimage);
    namedWindow("slic",1);
    int width=myimage.cols,height=myimage.rows;
    int maxp=(width/minsize)*(height/minsize);
    smask=Mat::zeros(width,height,CV_8U);
    vector< vector<int> > labs;
    vector<int> vld;
    vector< vector<Point2i> > bins;
    int rnum,des=1;
    cout<<"maxp= "<<maxp<<endl;
    while (des<=maxp)
    {
      des*=2;
      slic.SLICO_Provider(myimage,labs,outimage,des,rnum);
      cout<<"done desire "<<des<<" with "<<rnum<<endl;
      bins=vector< vector<Point2i> >(rnum+1,vector<Point2i>());
      vld=vector<int>(rnum+1,0);
      for (int i=0;i<height;i++)
        for (int j=0;j<width;j++)
        {
          bins[labs[i][j]].push_back(Point2i(i,j));
          if (smask.at<uchar>(i,j)==0)
            vld[labs[i][j]]++;
        }
      for (int i=0;i<rnum;i++)
      {
        if ((double)vld[i]/(bins[i].size())<0.5)
          continue;
        int res=check2Color(myimage,bins[i]);
        for (int j=0;j<bins[i].size();j++)
          if (smask.at<uchar>(bins[i][j].x,bins[i][j].y)==0)
            smask.at<uchar>(bins[i][j].x,bins[i][j].y)=res;
      }
      myimage.copyTo(outimage);
      for (int i=0;i<height;i++)
        for (int j=0;j<width;j++)
        {
          Vec3b &vv=outimage.at<Vec3b>(i,j);
          if (smask.at<uchar>(i,j)==0)
            vv=Vec3b(0,0,0);
          if (smask.at<uchar>(i,j)==1)
            vv=Vec3b(255,255,255);
        }
      imshow("slic",outimage);
      waitKey(500);
    }
    myimage.copyTo(outimage);
    for (int i=0;i<height;i++)
      for (int j=0;j<width;j++)
      {
        Vec3b &vv=outimage.at<Vec3b>(i,j);
        if (smask.at<uchar>(i,j)==0)
          vv=Vec3b(0,0,0);
        if (smask.at<uchar>(i,j)==1)
          vv=Vec3b(255,255,255);
      }
    cout<<"done with "<<rnum<<" superpixels"<<endl;
    imwrite(string(argv[1])+"__SLIC0__2C.jpg",outimage);
    imshow("slic",outimage);
    waitKey(0);
  }
  else
  {
    for (int ic=1;ic<=argc-1;ic++)
    {
      Mat myimage=imread(argv[ic]),outimage;
      namedWindow("slic",1);
      cout<<"processing image "<<ic<<" input desired superpixels number"<<endl;
      int th;
      cin>>th;
      vector< vector<int> > labs;
      int rnum;
      slic.SLICO_Provider(myimage,labs,outimage,th,rnum);
      cout<<"done with "<<rnum<<" superpixels"<<endl;
      imwrite(string(argv[ic])+"__SLIC0.jpg",outimage);
      imshow("slic",outimage);
      waitKey(1000);
    }
    waitKey(0);
  }
  return 0;
}
  


