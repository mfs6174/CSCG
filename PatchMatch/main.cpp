#include "inc.h"
#include "PM.h"

PMatch mc1=PMatch(),mc2=PMatch();
vector< vector< vector<double> > >pool;
vector< vector< vector<int> > >cnt;
int nh=0,nw=0;
double ta=0.5;
int bgit=2;

void vote(Mat &im,int Ns,int Nt)
{
  double ns=1.0/Ns,nt=1.0/Nt;
  pool=vector< vector< vector<double> > >(nh+1,vector< vector<double> >(nw+1,vector<double>(3,0) ));
  cnt=vector< vector< vector<int> > >(nh+1,vector< vector<int> >(nw+1,vector<int>(2,0) ));
  for (int x=1;x<=mc2.sh;x++)
    for (int y=1;y<=mc2.sw;y++)
    {
      int xx=x+mc2.fS[x][y].x,yy=y+mc2.fS[x][y].y;
      int i1=max(-(PATCH_SIZE-1)/2,max(-x+1,-xx+1)),i2=min(PATCH_SIZE/2,min(mc2.sh-x,mc2.th-xx));
      int j1=max(-(PATCH_SIZE-1)/2,max(-y+1,-yy+1)),j2=min(PATCH_SIZE/2,min(mc2.sw-y,mc2.tw-yy));
      for (int i=i1;i<=i2;i++)
        for (int j=j1;j<=j2;j++)
        {
          Vec3b &v2=mc2.imgT.at<Vec3b>(xx+i-1,yy+j-1);
          for (int k=0;k<3;k++)
            pool[x+i][y+j][k]+=v2.val[k]*(nt)*(1-ta);
          cnt[x+i][y+j][0]++;
        }
    }
  if (ta>0)
  {
    for (int x=1;x<=mc1.sh;x++)
      for (int y=1;y<=mc1.sw;y++)
      {
        int xx=x+mc1.fS[x][y].x,yy=y+mc1.fS[x][y].y;
        int i1=max(-(PATCH_SIZE-1)/2,max(-x+1,-xx+1)),i2=min(PATCH_SIZE/2,min(mc1.sh-x,mc1.th-xx));
        int j1=max(-(PATCH_SIZE-1)/2,max(-y+1,-yy+1)),j2=min(PATCH_SIZE/2,min(mc1.sw-y,mc1.tw-yy));
        for (int i=i1;i<=i2;i++)
          for (int j=j1;j<=j2;j++)
          {
            Vec3b &v2=mc1.imgS.at<Vec3b>(x+i-1,y+j-1);
            for (int k=0;k<3;k++)
              pool[xx+i][yy+j][k]+=v2.val[k]*(ns)*ta;
            cnt[xx+i][yy+j][1]++;
          }
      }
  }
  for (int i=1;i<=nh;i++)
    for (int j=1;j<=nw;j++)
    {
      Vec3b &v2=im.at<Vec3b>(i-1,j-1);
      for (int k=0;k<3;k++)
      {
        pool[i][j][k]/=(cnt[i][j][0]*nt*(1-ta)+cnt[i][j][1]*ns*ta);
        v2.val[k]=pool[i][j][k];
      }
    }
}

int main(int argc, char *argv[])
{
  if (argc==3)
  {
    int itnum;
    cout<<"input iteration times"<<endl;
    cin>>itnum;
    Mat im1,im2;
    im1=imread(string(argv[1]));
    im2=imread(string(argv[2]));
    mc1.load(im1,im2);
    cout<<"loaded"<<endl;
    mc1.init(false);
    for (int i=1;i<=itnum;i++)
    {
      cout<<"iteration "<<i<<endl;
      double err=mc1.doIter();
      cout<<err<<endl;
    }
  }
  if (argc==4)
  {
    ta=0.0;
    bgit=1000;
    int itnum;
    cout<<"input iteration times"<<endl;
    cin>>itnum;
    Mat im1,im2;
    im1=imread(string(argv[1]));
    im2=imread(string(argv[2]));
    nw=im2.cols;nh=im2.rows;
    int Nt=nw*nh,Ns=im1.cols*im1.rows;
    namedWindow("target",1);
    mc1.load(im1,im2);
    mc2.load(im2,im1);
    mc1.init(false);
    mc2.init(false);
    vote(im2,Ns,Nt);
    cout<<"done voting"<<endl;
    imshow("target",im2);
    imwrite("match"+string(argv[1])+"0"+".jpg",im2);
    waitKey(1000);
    double err;
    if (ta>0)
    {
      err=mc1.doIter();
      cout<<"done st iter with "<<err<<endl;
    }
    err=mc2.doIter();
    cout<<"done ts iter with "<<err<<endl;
    for (int it=1;it<=itnum;it++)
    {
      vote(im2,Ns,Nt);
      cout<<"done voting"<<endl;
      imshow("target",im2);
      stringstream id;
      id<<it;
      imwrite("match"+string(argv[1])+id.str()+".jpg",im2);
      waitKey(1000);
      if (it>bgit)
      {
        mc1.reload(im2,false);
        mc2.reload(im2,true);
      }
      if (ta>0)
      {
        err=mc1.doIter();
        cout<<"done st iter with "<<err<<endl;
      }
      err=mc2.doIter();
      cout<<"done ts iter with "<<err<<endl;
    }
  }
  if (argc==2)
  {
    bgit=1;
    int itnum,rnum;
    int fw,fh;
    cout<<"input iteration times,resize times"<<endl;
    cin>>itnum>>rnum;
    cout<<"intput new width height"<<endl;
    cin>>fw>>fh;
    Mat im1,im2;
    im1=imread(string(argv[1]));
    int Nt,Ns=im1.cols*im1.rows;
    //im2.create(nh,nw,CV_8UC3);
    //im2=imread(string(argv[2]));
    im1.copyTo(im2);
    nw=im2.cols;
    nh=im2.rows;
    Nt=nw*nh;
    namedWindow("target",1);
    for (int ttt=1;ttt<=rnum;ttt++)
    {
      //int titnum=itnum+itnum*(double)(ttt-1)/(itnum-1);
      int titnum=itnum;
      mc1.load(im1,im2);
      mc2.load(im2,im1);
      mc1.init(false);
      mc2.init(false);
      double err=mc1.doIter();
      cout<<"done st iter with "<<err<<endl;
      err=mc2.doIter();
      cout<<"done ts iter with "<<err<<endl;
      for (int it=1;it<=titnum;it++)
      {
        vote(im2,Ns,Nt);
        cout<<"done voting"<<endl;
        imshow("target",im2);
        waitKey(1000);
        if (it>bgit)
        {
          mc1.reload(im2,false);
          mc2.reload(im2,true);
        }
        double err=mc1.doIter();
        cout<<"done st iter with "<<err<<endl;
        err=mc2.doIter();
        cout<<"done ts iter with "<<err<<endl;
      }
      Mat imt;
      resize(im2,imt,Size( im1.cols*((double)fw/im1.cols+(1-(double)fw/im1.cols)*(rnum-ttt)/rnum),im1.rows*((double)fh/im1.rows+(1-(double)fh/im1.rows)*(rnum-ttt)/rnum) ),0,0,CV_INTER_AREA);
      im2=imt;
      nw=im2.cols;
      nh=im2.rows;
      Nt=nw*nh;
      stringstream id;
      id<<ttt;
      imwrite("img_"+string(argv[1])+id.str()+".jpg",im2);
    }
    imshow("target",im2);
    waitKey(0);
    
  }
  return 0;
}
