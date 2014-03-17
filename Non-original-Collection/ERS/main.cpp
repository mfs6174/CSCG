#include "MERCLazyGreedy.h"
#include "MERCInputImage.h"
#include "MERCOutputImage.h"

#include "Image.h"
#include "ImageIO.h"
#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
  if (argc<2)
  {
    cout<<"please give an image"<<endl;
    return 0;
  }
  double lambda,sigma;
  int nC,kernel = 0;
  size_t width,height;	
  Image<float> inputL,inputA,inputB;	
  MERCInputImage<float> input;
  MERCLazyGreedy merc;

  Mat myimage=imread(argv[1]),outimage;
  namedWindow("ers",1);
  cout<<"processing image input desired superpixels number lambda(0.5)  and sigma(5.0)"<<endl;
  cin>>nC>>lambda>>sigma;
  if (lambda==0||sigma==0)
  {
    lambda=0.5;
    sigma=5.0;
  }
  width=myimage.cols;
  height=myimage.rows;
  inputL.Resize(width,height,false);
  inputA.Resize(width,height,false);
  inputB.Resize(width,height,false);
  myimage.copyTo(outimage);
  Mat tmpimage(width,height,CV_8UC3);
  //myimage.copyTo(tmpimage);
  //tmpimage=tmpimage/255.0;
  cvtColor(myimage,tmpimage,CV_BGR2Lab);
  for (int col=0; col < width; col++)
    for (int row=0; row < height; row++)
    {
      Vec3b &vv=tmpimage.at<Vec3b>(row,col);
      //cout<<(int)vv[0]<<' '<<(int)vv[1]<<' '<<(int)vv[2]<<endl;
      inputL.Access(col,row) = vv[0];
      inputA.Access(col,row) = vv[1];
      inputB.Access(col,row) = vv[2];
    }
  input.ReadImage(&inputL,&inputA,&inputB);
  cout<<"done input"<<endl;
  merc.ClusteringTreeIF(input.nNodes_,input,kernel,sigma,lambda*1.0*nC,nC);
  cout<<"done clustering"<<endl;
  vector<int> label = MERCOutputImage::DisjointSetToLabel(merc.disjointSet_);
  vector< vector<int> > labs=vector< vector<int> >(height,vector<int>(width));
  for (int col=0; col < width; col++)
    for (int row=0; row < height; row++)
    {
      labs[row][col] = label[col+row*width];
      //cout<<labs[row][col]<<endl;
    }
  for (int col=0; col < width; col++)
    for (int row=0; row < height; row++)
    {
      Vec3b &vv=outimage.at<Vec3b>(row,col);
      for (int dx=-1;dx<=1;dx++)
        for (int dy=-1;dy<=1;dy++)
          if ( col+dx>=0&& col+dx < width&&row+dy>=0&&row+dy <height)
              if (labs[row][col]!=labs[row+dy][col+dx])
                vv=Vec3b(0,0,255);
    }         
  imwrite(string(argv[1])+"__ERS.jpg",outimage);
  imshow("ers",outimage);
  waitKey(0);
  return 0;
}
  


