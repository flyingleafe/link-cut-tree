#ifndef FLF_SPLAY_TREE_H
#define FLF_SPLAY_TREE_H

#include <string>
#include "util.h"
#include "splay_node.h"
#include "allocator.h"

//
// Tree struct (a wrapper around pointer to the node)
//

struct SplayTree
{
  SplayTree();
  ~SplayTree();

  void append(const long long &);
  maybe<long long> lookup(const int &);
  void increase(int, int, const long long &);
  maybe<long long> minimum(const int &);
  std::string show();

  SplayNode * _root;
  Allocator<SplayNode> _alloc;
};

#endif
