#include <string>
#include <iostream>
#include "SLIC.h"


SLIC slic;

int main(int argc, char *argv[])
{
  if (argc<2)
  {
    cout<<"please give at least one image"<<endl;
    return 0;
  }
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
  return 0;
}
  


