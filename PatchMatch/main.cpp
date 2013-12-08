#include "inc.h"
#include "PM.h"

PMatch mc1=PMatch(),mc2=PMatch();
vector< vector< vector<int> > > pool[2];
vector< vector< vector<int> > >cnt;
int nh=0,nw=0;
double clAlpha=0.5;
int bgit=2;
int lhPatch=-(PATCH_SIZE-1)/2,rhPatch=PATCH_SIZE/2;
void vote(Mat &im,int Ns,int Nt)
{
  double ns=1.0/Ns,nt=1.0/Nt;
  pool[0]=pool[1]=( vector< vector< vector<int> > >(nh+1,vector< vector<int> >(nw+1,vector<int>(3,0) )) );
  cnt=vector< vector< vector<int> > >(nh+1,vector< vector<int> >(nw+1,vector<int>(2,0) ));
  if (clAlpha<1)
  {
    for (int x=1;x<=mc2.sh;x++)
      for (int y=1;y<=mc2.sw;y++)
      {
        int xx=x+mc2.fS[x][y].x,yy=y+mc2.fS[x][y].y;
        int i1=max(lhPatch,max(-x+1,-xx+1)),i2=min(rhPatch,min(mc2.sh-x,mc2.th-xx));
        int j1=max(lhPatch,max(-y+1,-yy+1)),j2=min(rhPatch,min(mc2.sw-y,mc2.tw-yy));
        for (int i=i1;i<=i2;i++)
          for (int j=j1;j<=j2;j++)
          {
            Vec3b &v2=mc2.imgT.at<Vec3b>(xx+i-1,yy+j-1);
            for (int k=0;k<3;k++)
              pool[0][x+i][y+j][k]+=v2.val[k];
            cnt[x+i][y+j][0]++;
          }
      }
  }
  if (clAlpha>0)
  {
    for (int x=1;x<=mc1.sh;x++)
      for (int y=1;y<=mc1.sw;y++)
      {
        int xx=x+mc1.fS[x][y].x,yy=y+mc1.fS[x][y].y;
        int i1=max(lhPatch,max(-x+1,-xx+1)),i2=min(rhPatch,min(mc1.sh-x,mc1.th-xx));
        int j1=max(lhPatch,max(-y+1,-yy+1)),j2=min(rhPatch,min(mc1.sw-y,mc1.tw-yy));
        for (int i=i1;i<=i2;i++)
          for (int j=j1;j<=j2;j++)
          {
            Vec3b &v2=mc1.imgS.at<Vec3b>(x+i-1,y+j-1);
            for (int k=0;k<3;k++)
              pool[1][xx+i][yy+j][k]+=v2.val[k];
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
        v2.val[k]=(pool[0][i][j][k]*nt*(1-clAlpha)+pool[1][i][j][k]*ns*clAlpha)/(cnt[i][j][0]*nt*(1-clAlpha)+cnt[i][j][1]*ns*clAlpha);
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
    clAlpha=0.5;
    int itnum;
    cout<<"input patchMatch iteration times"<<endl;
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
    waitKey(100);
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
      waitKey(100);
      //mc1.reload(im2,false);
      //mc2.reload(im2,true);
      err1=mc1.doIter();
      cout<<"done st iter with "<<err1<<endl;
      err2=mc2.doIter();
      cout<<"done ts iter with "<<err2<<endl;
      cout<<"done with D="<<err1/Ns+err2/Nt<<endl;
    }
  }
  if (string(argv[1])=="c2fret")
  {
    ran=true;
    bgit=0;
    int itnum,rnum,pmnum;
    int fw,fh,ffw,ffh;
    double err1,err2;
    cout<<"input iteration times,resize times,patchMatch iteration times"<<endl;
    cin>>itnum>>rnum>>pmnum;
    cout<<"intput new width height"<<endl;
    cin>>fw>>fh;
    Mat im1,im2;
    im1=imread(string(argv[2]));
    vector<Mat> im1Pymd;
    int mLev=ceil(log(min(fw,fh)))-1;
    resize(im1,im2,Size(fw,fh));
    buildPyramid(im2,im1Pymd,mLev);
    ffw=im1Pymd[mLev].cols;
    ffh=im1Pymd[mLev].rows;
    buildPyramid(im1,im1Pymd,mLev);
    int Nt,Ns=im1.cols*im1.rows;
    im1Pymd[mLev].copyTo(im2);
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
      for (int it=1;it<=titnum;it++)
      {
        for (int pit=1;pit<=pmnum;pit++)
        {
          err1=mc1.doIter();
          cout<<"done st iter with "<<err1<<endl;
          err2=mc2.doIter();
          cout<<"done ts iter with "<<err2<<endl;
        }
        cout<<"done with D="<<err1/Ns+err2/Nt<<endl;
        vote(im2,Ns,Nt);
        cout<<"done voting"<<endl;
        imshow("target",im2);
        waitKey(300);
        if (it>bgit)
        {
          mc1.reload(im2,false);
          mc2.reload(im2,true);
        }
      }
      Mat imt;
      resize(im2,imt,Size( im1Pymd[mLev].cols*((double)ffw/im1Pymd[mLev].cols+(1-(double)ffw/im1Pymd[mLev].cols)*(rnum-ttt)/rnum),im1Pymd[mLev].rows*((double)ffh/im1Pymd[mLev].rows+(1-(double)ffh/im1Pymd[mLev].rows)*(rnum-ttt)/rnum) ),0,0,CV_INTER_AREA);
      im2=imt;
      nw=im2.cols;
      nh=im2.rows;
      Nt=nw*nh;
      stringstream id;
      id<<ttt;
      imwrite("img_"+string(argv[2])+id.str()+".jpg",im2);
    }
    for (int lev=mLev-1;lev>=0;lev--)
    {
       int Nt,Ns=im1.cols*im1.rows;
       im1Pymd[lev].copyTo(im2);
       nw=im2.cols;
       nh=im2.rows;
       Nt=nw*nh;
      int titnum=itnum;
      mc1.load(im1,im2);
      mc2.load(im2,im1);
      mc1.init(false);
      mc2.init(false);
      for (int it=1;it<=titnum;it++)
      {
        for (int pit=1;pit<=pmnum;pit++)
        {
          err1=mc1.doIter();
          cout<<"done st iter with "<<err1<<endl;
          err2=mc2.doIter();
          cout<<"done ts iter with "<<err2<<endl;
        }
        cout<<"done with D="<<err1/Ns+err2/Nt<<endl;
        vote(im2,Ns,Nt);
        cout<<"done voting"<<endl;
        imshow("target",im2);
        waitKey(300);
        if (it>bgit)
        {
          mc1.reload(im2,false);
          mc2.reload(im2,true);
        }
      }
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
    im1=imread(string(argv[2]));
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
    int itnum;
    int fw,fh;
    double err1,err2;
    int peakX,peakY;
    double maxD=0,minD=9999999;
    cout<<"input patchMatch iteration times"<<endl;
    cin>>itnum;
    cout<<"intput cropped width height"<<endl;
    cin>>fw>>fh;
    Mat im1,im2,dissMap,vmap;
    im1=imread(string(argv[2]));
    int Nt,Ns=im1.cols*im1.rows;
    dissMap.create(im1.rows,im1.cols,CV_64F);
    vmap=Mat::zeros(im1.rows,im1.cols,CV_8UC1);
    namedWindow("cropped",1);
    int lux,luy,rdx,rdy;
    //cout<<im1.rows<<' '<<im1.cols<<endl;
    lux=max(-(fw-1)/2,0);
    luy=max(-(fh-1)/2,0);
    rdx=min(fw/2,im1.cols-1);
    rdy=min(fh/2,im1.rows-1);
    im2=im1(Rect(lux,luy,rdx-lux+1,rdy-luy+1));
    nh=im2.rows;
    nw=im2.cols;
    Nt=nh*nw;
    mc1.load(im1,im2);
    mc1.init(false);
    int lip=(fh-1)/2,ljp=(fw-1)/2;
    int rip=fh/2,rjp=fw/2;
    for (int mi=lip;mi+rip<im1.rows;mi+=10)
      for (int mj=ljp;mj+rjp<im1.cols;mj+=10)
      {
        vmap.at<uchar>(mi,mj)=1;
        cout<<"calculating "<<mi<<','<<mj<<endl;
        lux=max(mj-(fw-1)/2,0);
        luy=max(mi-(fh-1)/2,0);
        rdx=min(mj+fw/2,im1.cols-1);
        rdy=min(mi+fh/2,im1.rows-1);
        im2=im1(Rect(lux,luy,rdx-lux+1,rdy-luy+1));
        nh=im2.rows;
        nw=im2.cols;
        Nt=nh*nw;
        //cout<<lux<<' '<<luy<<' '<<rdx<<' '<<rdy<<endl;
        if (mc1.reload(im2,false))
          mc1.init(false);
        else
          mc1.reset();
        imshow("cropped",im2);
         waitKey(100);
        for (int it=1;it<=itnum;it++)
        {
          err1=mc1.doIter();
          cout<<"done st iter with "<<err1<<endl;
          // err2=mc2.doIter();
          // cout<<"done ts iter with "<<err2<<endl;
        }
        double dissm=err1/Ns;//+err2/Nt;
        maxD=max(maxD,dissm);
        if (dissm<minD)
        {
          peakX=mj;
          peakY=mi;
        }
        minD=min(minD,dissm);
        cout<<"done with D="<<dissm<<endl;
        dissMap.at<double>(mi,mj)=dissm;
      }
    namedWindow("dissValue",1);
    for (int mi=0;mi<im1.rows;mi++)
      for (int mj=0;mj<im1.cols;mj++)
      {
        if (vmap.at<uchar>(mi,mj)>0)
          vmap.at<uchar>(mi,mj)=(uchar)( (dissMap.at<double>(mi,mj)-minD)/(maxD-minD)*255 );
        else
          vmap.at<uchar>(mi,mj)=255;
      }
    lux=max(peakX-(fw-1)/2,0);
    luy=max(peakY-(fh-1)/2,0);
    rdx=min(peakX+fw/2,im1.cols-1);
    rdy=min(peakY+fh/2,im1.rows-1);
    im2=im1(Rect(lux,luy,rdx-lux+1,rdy-luy+1));
    cout<<"Peak is "<<peakY<<','<<peakX<<endl;
    imshow("dissValue",vmap);
    imshow("cropped",im2);
    waitKey(0);
    imwrite("crop_"+string(argv[2])+".jpg",im2);
    imwrite("dismap_"+string(argv[2])+".jpg",vmap);
  }
  
  if (!ran)
  {
    cout<<"please give a legal function name"<<endl;
  } 
  return 0;
}
