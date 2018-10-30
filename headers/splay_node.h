#ifndef FLF_SPLAY_NODE_H
#define FLF_SPLAY_NODE_H

//
// Node struct
//

struct SplayNode
{
  SplayNode(const long long &);
  SplayNode();
  ~SplayNode();

  SplayNode * _parent;
  SplayNode * _pathparent;
  SplayNode * _left;
  SplayNode * _right;

  int _size;
  long long _dW;
  long long _dMin;

  void splay();
  bool _isRoot();
  bool _isLeftSon();
  bool _isRightSon();
};

SplayNode * find(SplayNode *x, int k);
SplayNode * merge(SplayNode *a, SplayNode *b);
void split(SplayNode *x, SplayNode **out_a, SplayNode **out_b);
void add(SplayNode *x, const long long &c);
long long min(SplayNode *x);

#endif
