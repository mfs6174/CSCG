/*
  Copyright 2011, Ming-Yu Liu

  All Rights Reserved 

  Permission to use, copy, modify, and distribute this software and 
  its documentation for any non-commercial purpose is hereby granted 
  without fee, provided that the above copyright notice appear in 
  all copies and that both that copyright notice and this permission 
  notice appear in supporting documentation, and that the name of 
  the author not be used in advertising or publicity pertaining to 
  distribution of the software without specific, written prior 
  permission. 

  THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  ANY PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR 
  ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES 
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING 
  OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
*/
#include "MERCDisjointSet.h"


MERCDisjointSet::MERCDisjointSet(int nElements) 
{	
  nElements_ = nElements;
  nVertices_ = nElements_;
  cnt_=0;
  p_ = new int [nElements_*2+1];
  size_ = new int [nElements_*2+1];
  tp_ = new int [nElements_*2+1];
  tr_ = new int [nElements_*2+1];
  tl_ = new int [nElements_*2+1];
  lists_ = new MList<int> [nElements_*2+1];
  Np1 = new MListNode<int>* [nElements_*2+1];
  Np2 = new MListNode<int>* [nElements_*2+1];

  int reservedSize = (int)std::sqrt( 1.0*nElements );
  // Initialization with the cluster size and id
  for (int i = 0; i < nElements; i++) 
  {
    p_[i] = i;
    size_[i] = 1;
    lists_[i].PushBack(i);
    tr_[i]=tl_[i]=-1;
    tp_[i]=i;
  }
  for (int i = nElements; i < nElements*2+1; i++) 
  {
    p_[i] = i;
    size_[i] = 0;
    tr_[i]=tl_[i]=-1;
    tp_[i]=i;
  }

}
  
MERCDisjointSet::~MERCDisjointSet() 
{
  Release();
}

void MERCDisjointSet::Release()
{
  delete [] p_;
  delete [] size_;
  delete [] lists_;
  delete [] tp_;
  delete [] tl_;
  delete [] tr_;
  delete [] Np1;
  delete [] Np2;
  p_ = NULL;
  size_ = NULL;
  lists_ = NULL;
  tp_ = NULL;
  tr_ = NULL;
  tl_ = NULL;
  Np1 = NULL;
  Np2 = NULL;

}

void MERCDisjointSet::Set(int x,int l)
{
  p_[x] = l;
}

int MERCDisjointSet::Find(int x) 
{
  // return the cluster ID
  if (p_[x]==x)
    return x;
  p_[x]=Find(p_[x]);
  return p_[x];
}

int MERCDisjointSet::Join(int x, int y) 
{
  int aID = Find(x);
  int bID = Find(y);
  
  // The size is only maintained for cluster ID.
  int aSize = size_[aID];
  int bSize = size_[bID];
  cnt_++;
  int newID=nVertices_-1+cnt_;
  //std::cout<<"newid"<<newID<<std::endl;
  size_[newID] = aSize+bSize;
  p_[aID]=p_[bID]=newID;
  tp_[aID]=tp_[bID]=newID;
  tl_[newID]=aID;
  tr_[newID]=bID;
  lists_[newID].Append(lists_[aID]);
  lists_[newID].Append(lists_[bID]);
  Np1[newID]= lists_[newID].first_;
  Np2[newID]= lists_[newID].last_;
  nElements_--;
  //std::cout<<x<<std::endl;
  //std::cout<<y<<std::endl;
  return newID;
}
