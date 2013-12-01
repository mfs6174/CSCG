#ifndef PM_H
#define PM_H

#define PATCH_SIZE 5
#define ALPHA 0.5

struct PMatch
{
  Mat imgS,imgT;
  Mat vlS,vlT;
  vector< vector<Point2d> > fS;
  vector< vector<int> >  fGood;
  bool loaded;
  int sw,sh,tw,th;
  int iTimes;
  bool both;
  PMatch();
  void _vl();
  int _cal(int x,int y,Point2d pnt,int pmax);
  int load(Mat _s,Mat _t);
  int reload(Mat _s,bool _wh);
  int init(bool _both);
  double doIter();
};

#endif
