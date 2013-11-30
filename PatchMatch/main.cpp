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
            pool[x+i][y+j][k]+=v2.val[k]*(nt);
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
              pool[xx+i][yy+j][k]+=v2.val[k]*(ns);
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
        pool[i][j][k]/=(cnt[i][j][0]*nt+cnt[i][j][1]*ns);
        v2.val[k]=pool[i][j][k];
      }
    }
}

int main(int argc, char *argv[])
{
  if (argc<2)
  {
    cout<<"plese give which function to use"<<endl;
    return 0;
  }
  bool ran=false;
  if (string(argv[1])=="rec")
  {
    ran=true;
    ta=0.5;
    int itnum;
    cout<<"input iteration times"<<endl;
    cin>>itnum;
    Mat im1,im2;
    im1=imread(string(argv[2]));
    im2=imread(string(argv[3]));
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
    imwrite("rec_"+string(argv[3])+"0"+".jpg",im2);
    waitKey(1000);
    double err1,err2;
    err1=mc1.doIter();
    cout<<"done st iter with "<<err1<<endl;
    err2=mc2.doIter();
    cout<<"done ts iter with "<<err2<<endl;
    cout<<"done with D="<<err1/Ns+err2/Nt<<endl;
    for (int it=1;it<=itnum;it++)
    {
      vote(im2,Ns,Nt);
      cout<<"done voting"<<endl;
      imshow("target",im2);
      stringstream id;
      id<<it;
      imwrite("rec_"+string(argv[3])+id.str()+".jpg",im2);
      waitKey(1000);
      //mc1.reload(im2,false);
      //mc2.reload(im2,true);
      err1=mc1.doIter();
      cout<<"done st iter with "<<err1<<endl;
      err2=mc2.doIter();
      cout<<"done ts iter with "<<err2<<endl;
      cout<<"done with D="<<err1/Ns+err2/Nt<<endl;
    }
  }
  if (string(argv[1])=="ret")
  {
    ran=true;
    bgit=0;
    int itnum,rnum,pmnum;
    int fw,fh;
    double err1,err2;
    cout<<"input iteration times,resize times,patchMatch iteration times"<<endl;
    cin>>itnum>>rnum>>pmnum;
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
      for (int pit=1;pit<=pmnum;pit++)
      {
        err1=mc1.doIter();
        cout<<"done st iter with "<<err1<<endl;
        err2=mc2.doIter();
        cout<<"done ts iter with "<<err2<<endl;
      }
      cout<<"done with D="<<err1/Ns+err2/Nt<<endl;
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
        for (int pit=1;pit<=pmnum;pit++)
        {
          err1=mc1.doIter();
          cout<<"done st iter with "<<err1<<endl;
          err2=mc2.doIter();
          cout<<"done ts iter with "<<err2<<endl;
        }
        cout<<"done with D="<<err1/Ns+err2/Nt<<endl;
      }
      Mat imt;
      resize(im2,imt,Size( im1.cols*((double)fw/im1.cols+(1-(double)fw/im1.cols)*(rnum-ttt)/rnum),im1.rows*((double)fh/im1.rows+(1-(double)fh/im1.rows)*(rnum-ttt)/rnum) ),0,0,CV_INTER_AREA);
      im2=imt;
      nw=im2.cols;
      nh=im2.rows;
      Nt=nw*nh;
      stringstream id;
      id<<ttt;
      imwrite("img_"+string(argv[2])+id.str()+".jpg",im2);
    }
    imshow("target",im2);
    waitKey(0);
  }
  if (string(argv[1])=="atc")
  {
    ran=true;
  }
  
  if (!ran)
  {
    cout<<"please give a legal function name"<<endl;
  } 
  return 0;
}
