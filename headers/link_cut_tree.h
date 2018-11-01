#ifndef FLF_LINK_CUT_TREE_H
#define FLF_LINK_CUT_TREE_H

#include <vector>
#include "util.h"
#include "splay_node.h"
#include "allocator.h"

struct LinkCutTree
{
  LinkCutTree();
  ~LinkCutTree();

  int newNode(const long long &v);
  void addLC(int node_id, const long long &c);
  maybe<long long> minLC(int node_id);
  void link(int parent, int child);
  void cut(int node_id);
  int lca(int u, int v);

private:
  std::vector<SplayNode *> _nodes;
  Allocator<SplayNode> _alloc;
};

void expose(SplayNode *u);
#endif
