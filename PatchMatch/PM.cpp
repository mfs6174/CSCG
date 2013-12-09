#include "inc.h"
#include "PM.h"

inline int _getn(int n)
{
  return rand()%n;
}

inline double _getpn1()
{
  double rt= (rand()%100000000/100000000.0);
  if (rt<0.5)
    rt=(-rt)*2;
  else
    rt=(rt-0.5)*2;
  return rt;
}

PMatch::PMatch()
{
  loaded=false;
  iTimes=0;
  both=false;
}

void PMatch::_vl()
{
}

int PMatch::load(Mat _s,Mat _t)
{
  _s.copyTo(imgS);
  _t.copyTo(imgT);
  sw=_s.cols;
  sh=_s.rows;
  tw=_t.cols;
  th=_t.rows;
  if (sw<1||sh<1||tw<1||th<1)
    return -1;
  loaded=true;
  return 0;
}

int PMatch::reload(Mat _s,bool _wh)
{
  int rt=0;
  if (_wh)
  {
    if ( (imgS.cols!=_s.cols) || (imgS.rows!=_s.rows) )
      rt=1;
    _s.copyTo(imgS);
  }
  else
  {
    if ( (imgT.cols!=_s.cols) || (imgT.rows!=_s.rows) )
      rt=1;
    _s.copyTo(imgT);
  }
  reset(true);
  return rt;
}

inline int sqr(int c)
{
  return c*c;
}

inline bool PMatch::_cplt(int x,int y,Point2i ofs)
{
  if (x-lhPatch>0&&y-lhPatch>0&&x+rhPatch<=sh&&y+rhPatch<=sw&&
      x+ofs.x-lhPatch>0&&y+ofs.y-lhPatch>0&&x+ofs.x+rhPatch<=th&&y+ofs.y+rhPatch<=tw)
    return true;
  else
    return false;
}

int PMatch::_moveDown(int x,int y)
{
  if (fGood[x][y]==maxlongint)
    return maxlongint;
  int rt=fGood[x][y]*sqrPsize;
  for (int j=-lhPatch;j<=rhPatch;j++)
  {
    Vec3b &v1=imgS.at<Vec3b>(x-lhPatch-1,y+j-1);
    Vec3b &v2=imgT.at<Vec3b>(x+fS[x][y].x-lhPatch-1,y+fS[x][y].y+j-1);
    rt-=sqr(v1.val[0]-v2.val[0])+sqr(v1.val[1]-v2.val[1])+sqr(v1.val[2]-v2.val[2]);
    Vec3b &v3=imgS.at<Vec3b>(x+rhPatch,y+j-1);
    Vec3b &v4=imgT.at<Vec3b>(x+fS[x][y].x+rhPatch,y+fS[x][y].y+j-1);
    rt+=sqr(v3.val[0]-v4.val[0])+sqr(v3.val[1]-v4.val[1])+sqr(v3.val[2]-v4.val[2]);
  }
  return rt/sqrPsize;
}

int PMatch::_moveRight(int x,int y)
{
  if (fGood[x][y]==maxlongint)
    return maxlongint;
  int rt=fGood[x][y]*sqrPsize;
  for (int i=-lhPatch;i<=rhPatch;i++)
  {
    Vec3b &v1=imgS.at<Vec3b>(x+i-1,y-lhPatch-1);
    Vec3b &v2=imgT.at<Vec3b>(x+fS[x][y].x+i-1,y+fS[x][y].y-lhPatch-1);
    rt-=sqr(v1.val[0]-v2.val[0])+sqr(v1.val[1]-v2.val[1])+sqr(v1.val[2]-v2.val[2]);
    Vec3b &v3=imgS.at<Vec3b>(x+i-1,y+rhPatch);
    Vec3b &v4=imgT.at<Vec3b>(x+fS[x][y].x+i-1,y+fS[x][y].y+rhPatch);
    rt+=sqr(v3.val[0]-v4.val[0])+sqr(v3.val[1]-v4.val[1])+sqr(v3.val[2]-v4.val[2]);
  }
  return rt/sqrPsize;
}

int PMatch::_cal(int x,int y,Point2i pnt,int pmax)
{
  int xx=x+pnt.x,yy=y+pnt.y;
  int vld=0,ssd=0;
  if (xx<1)
    xx=1;
  if (xx>th)
    xx=th;
  if (yy<1)
    yy=1;
  if (yy>tw)
    yy=tw;
  //int i1=max(-lhPatch,max(-x+1,-xx+1)),i2=min(rhPatch,min(sh-x,th-xx));
  //int j1=max(-lhPatch,max(-y+1,-yy+1)),j2=min(rhPatch,min(sw-y,tw-yy));

  int i1=max(-lhPatch,-x+1),i2=min(rhPatch,sh-x);
  int j1=max(-lhPatch,-y+1),j2=min(rhPatch,sw-y);

  if (i1<1-xx||i2>th-xx||j1<1-yy||j2>tw-yy)
    return maxlongint;
  if (pmax!=maxlongint)
    pmax=(pmax+1)*sqrPsize;
  for (int i=i1;i<=i2;i++)
    for (int j=j1;j<=j2;j++)
      {
          vld++;
          Vec3b &v1=imgS.at<Vec3b>(x+i-1,y+j-1);
          Vec3b &v2=imgT.at<Vec3b>(xx+i-1,yy+j-1);
          //cout<<(int)v1.val[0]<<endl;
          ssd+=sqr(v1.val[0]-v2.val[0])+sqr(v1.val[1]-v2.val[1])+sqr(v1.val[2]-v2.val[2]);
          if (ssd>pmax)
            return maxlongint;
      }
  return ssd/vld;
}

int PMatch::init(bool _both)
{
  if (!loaded)
    return -1;
  double ssd=0;
  both=_both;
  iTimes=0;
  lhPatch=(PATCH_SIZE-1)/2;
  rhPatch=PATCH_SIZE/2;
  sqrPsize=sqr(PATCH_SIZE);
  vlS.create(sh,sw,CV_8UC3);
  fS=initFs=vector< vector<Point2i> >(sh+1,vector<Point2i>(sw+1));
  fGood=vector< vector<int> >(sh+1,vector<int>(sw+1,maxlongint));
  for (int i=1;i<=sh;i++)
    for (int j=1;j<=sw;j++)
    {
      fS[i][j].x=_getn(th)+1-i;
      fS[i][j].y=_getn(tw)+1-j;
      int tt=_cal(i,j,fS[i][j],maxlongint);
      while (tt==maxlongint)
      {
        fS[i][j].x=_getn(th)+1-i;
        fS[i][j].y=_getn(tw)+1-j;
        tt=_cal(i,j,fS[i][j],maxlongint);
      }
      ssd+=tt;
      fGood[i][j]=tt;
    }
  // if (_both)
  // {
  //   vlT.create(th,tw,CV_8UC3);
  //   fT=vector< vector<Point2d> >(th+1,vector<Point2d>(tw+1));
  // }
  _vl();
  initFs=fS;
  cout<<ssd<<endl;
  return 0;
}

int PMatch::reset(bool onlyReCal)
{
  iTimes=0;
  if (!onlyReCal)
    fS=initFs;
  for (int i=1;i<=sh;i++)
    for (int j=1;j<=sw;j++)
      fGood[i][j]=_cal(i,j,fS[i][j],maxlongint);
  return 0;
}

double PMatch::doIter()
{
  double rt=0;
  iTimes++;
  int tt=0;
  for (int i=1;i<=sh;i++)
    for (int j=1;j<=sw;j++)
    {
      int e1,e2,e3;
      if (i>1)
      {
        if (_cplt(i-1,j,fS[i-1][j])&&_cplt(i,j,fS[i-1][j]))
          e1=_moveDown(i-1,j);
        else
          e1=_cal(i,j,fS[i-1][j],fGood[i][j]);
      }
      else
        e1=maxlongint;
      if (j>1)
      {
        if (_cplt(i,j-1,fS[i][j-1])&&_cplt(i,j,fS[i][j-1]))
          e2=_moveRight(i,j-1);
        else
          e2=_cal(i,j,fS[i][j-1],min(e1,fGood[i][j]));
      }
      else
        e2=maxlongint;
      //e3=_cal(i,j,fS[i][j],min(min(e1,e2),fGood[i][j]));
      e3=fGood[i][j];
      Point2i tp;
      if (e1<e2)
      {
        tp=fS[i-1][j];
        fGood[i][j]=e1;
      }
      else
      {
        e1=e2;
        tp=fS[i][j-1];
        fGood[i][j]=e2;
      }
      if (e3<e1)
      {
        tp=fS[i][j];
        fGood[i][j]=e3;
      }
      fS[i][j]=tp;
      //finish prop
      
      double alp=1;
      while (alp*th>=1&&alp*tw>=1)
      {
        int ofx=_getpn1()*alp*th,ofy=_getpn1()*alp*tw;
        alp*=ALPHA;
        //e2=_cal(i,j,fS[i][j],fGood[i][j]);
        e2=fGood[i][j];
        e1=_cal(i,j,fS[i][j]+Point2i(ofx,ofy),fGood[i][j]);
        if (e1<e2)
        {
          fS[i][j]=fS[i][j]+Point2i(ofx,ofy);
          fGood[i][j]=e1;
        }
        else
        {
          e1=e2;
        }
      }
      rt+=fGood[i][j];
      //tt++;
      //cout<<tt<<endl;
      //cout<<e1<<endl;
      //finish random search
    }
  _vl();
  return rt;
}
    



