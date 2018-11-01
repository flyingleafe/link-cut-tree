#include <cassert>
#include "../headers/link_cut_tree.h"

using namespace std;

LinkCutTree::LinkCutTree()
  : _nodes()
{}

LinkCutTree::~LinkCutTree()
{}

// Is node a root of LC-tree?
bool isLCRoot(SplayNode *u)
{
  assert(u != nullptr);
  assert(u->_isRoot());

  return u->_left == nullptr && u->_pathparent == nullptr;
}

// Expose the path to which `u` belongs and find the way
void expose(SplayNode *u)
{
  assert(u != nullptr);
  u->splay();

  auto v = u;
  while (v->_pathparent != nullptr) {
    auto p = v->_pathparent;
    assert(p != nullptr);

    // separate the path we reconnect to
    auto pair = split(p, true);
    auto p_tail = pair.second;
    if (p_tail != nullptr) {
      p_tail->_pathparent = p;
    }

    p->_right = v;
    v->_parent = p;
    v->_pathparent = nullptr;
    v->_dW -= p->_dW;
    updMins(p);
    updSize(p);

    v = p;
  }

  // Make it a root of newly formed backbone path
  u->splay();
}

// Creates a new node
int LinkCutTree::newNode(const long long &v)
{
  int id = _nodes.size();
  _nodes.push_back(_alloc.alloc(id, v));
  return id;
}

maybe<long long> LinkCutTree::get(int node_id)
{
  if (node_id < 0 || node_id >= _nodes.size()) {
    return {};
  }

  auto v = _nodes[node_id];
  v->splay();
  return v->_dW;
}

void LinkCutTree::addLC(int node_id, const long long &c)
{
  auto u = _nodes[node_id];
  expose(u);
  add(u, c);
}

maybe<long long> LinkCutTree::minLC(int node_id)
{
  auto u = _nodes[node_id];
  expose(u);
  return min(u);
}

void LinkCutTree::link(int parent, int child)
{
  if (parent == child) return;
  auto u = _nodes[parent];
  auto v = _nodes[child];

  expose(v);
  assert(isLCRoot(v));

  v->_pathparent = u;
  expose(v);
}

void LinkCutTree::cut(int node_id)
{
  auto u = _nodes[node_id];
  expose(u);
  split(u);
}

maybe<int> LinkCutTree::lca(int fst, int snd)
{
  auto u = _nodes[fst];
  auto v = _nodes[snd];
  expose(u);
  expose(v);
  u->splay();

  auto p = u->_pathparent;
  if (p != nullptr) {
    return p->id;
  }
  return {};
}
