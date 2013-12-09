#ifndef PM_H
#define PM_H

#define PATCH_SIZE 5
#define ALPHA 0.5

struct PMatch
{
  Mat imgS,imgT;
  Mat vlS,vlT;
  vector< vector<Point2i> > fS,initFs;
  vector< vector<int> >  fGood;
  bool loaded;
  int sw,sh,tw,th;
  int iTimes;
  bool both;
  int sqrPsize,lhPatch,rhPatch;
  PMatch();
  void _vl();
  bool _cplt(int x,int y,Point2i ofs);
  int _moveDown(int x,int y);
  int _moveRight(int x,int y);
  int _cal(int x,int y,Point2i pnt,int pmax);
  int load(Mat _s,Mat _t);
  int reload(Mat _s,bool _wh);
  int reset(bool only);
  int init(bool _both);
  double doIter();
};

#endif
