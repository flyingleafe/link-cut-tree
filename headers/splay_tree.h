#ifndef FLF_SPLAY_TREE_H
#define FLF_SPLAY_TREE_H

#include <utility>
#include <string>
#include <vector>
#include "util.h"

//
// Node struct
//

struct SplayNode
{
  SplayNode(const long long &);
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

SplayNode * find(SplayNode *x, const int &k);
SplayNode * merge(SplayNode *a, SplayNode *b);
void split(SplayNode *x, SplayNode **out_a, SplayNode **out_b);
void add(SplayNode *x, const long long &c);
long long min(SplayNode *x);

//
// Tree struct (a wrapper around pointer to the node)
//

struct SplayTree
{
  SplayTree();
  ~SplayTree();

  void append(const long long &);
  maybe<long long> lookup(const int &);
  void increase(const int &, const long long &);
  maybe<long long> minimum(const int &);
  std::string show();

  SplayNode * _root;
  // std::vector<SplayNode> _nodes;
};

#endif
