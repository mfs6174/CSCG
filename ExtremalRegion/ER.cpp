#include<iostream>
#include "ER.h"

using namespace std;
using namespace cv;


void ExRegion::_floodfill(int x,int y)
{
  if (belong[x][y])
    return;
  if (x<1||y<1||x>sh||y>sw)
    return;
  if ( ( (!flip)?(chan.at<uchar>(x-1,y-1)):(255-chan.at<uchar>(x-1,y-1))) >threshold)
    return;
  if (flag)
  {
    flag=false;
    cnt++;
    blob.push_back(vector<Point2i>());
    blob[cnt-1].push_back(Point2i(x-1,y-1));
    belong[x][y]=cnt;
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
void ExRegion::init(Mat _input)
{
  _input.copyTo(input);
  sw=input.cols;
  sh=input.rows;
  belong=vector< vector<int> > (sh+1,vector<int>(sw+1,0));
  split(input,srgb);
  cvtColor(input,labImage,CV_BGR2Lab);
  split(labImage,slab);
  cvtColor(input,hsvImage,CV_BGR2HSV);
  split(hsvImage,shsv);
}


void ExRegion::calc(char _ch,int _threshold,bool _flip)
{
  cnt=0;
  belong=vector< vector<int> > (sh+1,vector<int>(sw+1,0));
  blob.clear();
  threshold=_threshold;
  flip=_flip;
  switch (_ch)
  {
  case 'r':
    chan=srgb[2];
    break;
  case 'g':
    chan=srgb[1];
    break;
  case 'b':
    chan=srgb[0];
    break;
  case 'h':
    chan=shsv[0];
    break;
  case 's':
    chan=shsv[1];
    break;
  case 'v':
    chan=shsv[2];
    break;
  case 'l':
    chan=slab[0];
    break;
  case 'A':
    chan=slab[1];
    break;
  case 'B':
    chan=slab[2];
    break;
  default:
    cout<<"not one of  rgbhsvlAB ,use default v channel"<<endl;
    chan=shsv[2];
  }
  for (int i=1;i<=sh;i++)
    for (int j=1;j<=sw;j++)
    {
      flag=true;
      _floodfill(i,j);
    }
  //  cout<<1<<endl;
  
  input.copyTo(output);
  for (int i=1;i<=sh;i++)
    for (int j=1;j<=sw;j++)
    {
      Vec3b &vv=output.at<Vec3b>(i-1,j-1);
      if (belong[i][j]==0)
      {
        vv[0]=vv[1]=vv[2]=0;
        continue;
      }
      if (belong[i][j]%3==0)
        vv=Vec3b(255,0,0);
      if (belong[i][j]%3==1)
        vv=Vec3b(0,255,0);
      if (belong[i][j]%3==2)
        vv=Vec3b(0,0,255);
    }
}
