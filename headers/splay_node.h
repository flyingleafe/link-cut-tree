#ifndef FLF_SPLAY_NODE_H
#define FLF_SPLAY_NODE_H

#include <utility>
#include <cstdlib>

//
// Node struct
//

struct SplayNode
{
  SplayNode(int, const long long &);
  ~SplayNode();

  SplayNode * _parent;
  SplayNode * _pathparent;
  SplayNode * _left;
  SplayNode * _right;

  const int id;
  size_t _size;
  long long _dW;
  long long _dMin;

  void splay();
  bool _isRoot();
  bool _isLeftSon();
  bool _isRightSon();
};

size_t size(SplayNode *x);
void updMins(SplayNode *x);
void updSize(SplayNode *x);

SplayNode * find(SplayNode *x, size_t k);
SplayNode * merge(SplayNode *a, SplayNode *b);
std::pair<SplayNode *, SplayNode *> split(SplayNode *x, bool root_to_left = false);
void add(SplayNode *x, const long long &c);
long long min(SplayNode *x);

#endif
