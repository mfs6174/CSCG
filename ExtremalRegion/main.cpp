#include <string>
#include <iostream>
#include "ER.h"


ExRegion er;

int main(int argc, char *argv[])
{
  if (argc<2)
  {
    cout<<"please give an image"<<endl;
    return 0;
  }
  Mat myimage=imread(argv[1]);
  er.init(myimage);
  namedWindow("low2high",1);
  namedWindow("high2low",1);
  Mat tmpimage;
  while (1)
  {
    cout<<"input channel and threshold(1-255)"<<endl;
    char ch;
    int th;
    cin>>ch>>th;
    er.calc(ch,th,0);
    er.output.copyTo(tmpimage);
    er.calc(ch,th,1);
    imshow("low2high",tmpimage);
    imshow("high2low",er.output);
    imwrite(string(argv[1])+"__l2h.jpg",tmpimage);
    imwrite(string(argv[1])+"__h2l.jpg",er.output);
    waitKey(0);
  }
  return 0;
}
  


