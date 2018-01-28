#ifndef FLF_SPLAY_TREE_H
#define FLF_SPLAY_TREE_H

#include <utility>
#include <string>
#include "util.h"

//
// Node struct
//

struct SplayNode
{
  SplayNode(const int &, const long long &);
  ~SplayNode();

  SplayNode * _parent;
  SplayNode * _pathparent;
  SplayNode * _left;
  SplayNode * _right;

  int _key;
  long long _value;
  long long _dW;
  long long _dMin;

  void splay();
  bool _isRoot();
  bool _isLeftSon();
  bool _isRightSon();
  void _rotateLeft();
  void _rotateRight();
};

void mkLeftChild(SplayNode *parent, SplayNode *child);
void mkRightChild(SplayNode *parent, SplayNode *child);
void updMins(SplayNode *x);
void pushDeltas(SplayNode *x, SplayNode *p, SplayNode *b);

//
// Tree struct (a wrapper around pointer to the node)
//

struct SplayTree
{
  SplayTree();
  SplayTree(SplayNode *);
  // ~SplayTree();

  void splay(SplayNode *);
  maybe<long long> find(const int &);
  void merge(const SplayTree &);
  std::pair<SplayTree, SplayTree> split(const int &);
  void add(const int &, const long long &);
  void remove(const int &);
  void increase(const int &, const long long &);
  maybe<long long> min(const int &);
  std::string show();

private:
  SplayNode * _root;
};

#endif
