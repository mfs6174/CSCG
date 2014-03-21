#include <string>
#include <cstring>
#include <iostream>
#include "SLIC.h"


SLIC slic;
const int minsize=30;
const double ratio2=0.99,ratio1=0.98;

int check2Color(const Mat & image,const vector<Point2i> &bin)
{
  Mat imat(bin.size(),3,CV_64FC1),cov,mean,eig,vec;
  for (int k=0;k<bin.size();k++)
  {
    int i=bin[k].x,j=bin[k].y;
    const Vec3b &vv=image.at<Vec3b>(i,j);
    for (int p=0;p<3;p++)
      imat.at<double>(k,p)=vv[p];
  }
  calcCovarMatrix(imat,cov,mean,CV_COVAR_NORMAL|CV_COVAR_ROWS|CV_COVAR_SCALE);
  eigen(cov,eig,vec);
  double egsum=( sum(eig) )[0];
  //cout<<egsum<<endl;
  cout<<"eigenvalue is "<<eig.at<double>(0)<<' '<<eig.at<double>(1)<<' '<<eig.at<double>(3)<<endl;
  for (int i=0;i<3;i++)
    cout<<"vec "<<i<<' '<<vec.at<double>(i,0)<<' '<<vec.at<double>(i,1)<<' '<<vec.at<double>(i,2)<<endl;
  if (eig.at<double>(0)/egsum>ratio1)
    return 1;
  if ( (eig.at<double>(0)+eig.at<double>(1))/egsum>ratio2)
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
    Mat labimage;
    cvtColor(myimage,labimage,CV_BGR2Lab);
    myimage.copyTo(wimage);
    namedWindow("slic",1);
    int width=myimage.cols,height=myimage.rows;
    int maxp=(width/minsize)*(height/minsize);
    smask=Mat::zeros(height,width,CV_8U);
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
  


