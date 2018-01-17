#ifndef FLF_SPLAY_TREE_H
#define FLF_SPLAY_TREE_H

#include <iostream>
#include <vector>
#include <sstream>
#include <utility>
#include <string>
#include "util.h"

using namespace std;

//
// Node struct
//

template <typename K, typename V>
struct SplayNode
{
  SplayNode(const K &, const V &);
  ~SplayNode();

  void splay();

  SplayNode<K, V> * _parent;
  SplayNode<K, V> * _left;
  SplayNode<K, V> * _right;
  K _key;
  V _value;

  bool _isRoot();
  bool _isLeftSon();
  bool _isRightSon();
  void _rotateLeft();
  void _rotateRight();
};

// util functions
// parent != nullptr
template <typename K, typename V>
void mkLeftChild(SplayNode<K, V> *parent, SplayNode<K, V> *child)
{
  parent->_left = child;
  if (child != nullptr) {
    child->_parent = parent;
  }
}

// parent != nullptr
template <typename K, typename V>
void mkRightChild(SplayNode<K, V> *parent, SplayNode<K, V> *child)
{
  parent->_right = child;
  if (child != nullptr) {
    child->_parent = parent;
  }
}

template <typename K, typename V>
SplayNode<K, V>::SplayNode(const K &key, const V &val)
  : _key(key), _value(val)
  , _parent(nullptr), _left(nullptr), _right(nullptr)
{}

template <typename K, typename V>
SplayNode<K, V>::~SplayNode()
{
  if (_left != nullptr) {
    delete _left;
  }
  if (_right != nullptr) {
    delete _right;
  }
}

template <typename K, typename V>
bool SplayNode<K, V>::_isRoot()
{
  return _parent == nullptr;
}

template <typename K, typename V>
bool SplayNode<K, V>::_isLeftSon()
{
  return !_isRoot() && _parent->_left == this;
}

template <typename K, typename V>
bool SplayNode<K, V>::_isRightSon()
{
  return !_isRoot() && !_isLeftSon();
}

template <typename K, typename V>
void SplayNode<K, V>::_rotateLeft()
{
  auto x = this;
  auto b = x->_left;
  auto p = x->_parent;
  auto r = p->_parent;
  bool was_left_child = r == nullptr ? false : p->_isLeftSon() ? true : false;

  mkLeftChild(x, p);
  mkRightChild(p, b);
  x->_parent = r;

  if (r != nullptr) {
    if (was_left_child) {
      r->_left = x;
    } else {
      r->_right = x;
    }
  }
}

template <typename K, typename V>
void SplayNode<K, V>::_rotateRight()
{
  auto x = this;
  auto b = x->_right;
  auto p = x->_parent;
  auto r = p->_parent;
  bool was_left_child = r == nullptr ? false : p->_isLeftSon() ? true : false;

  mkRightChild(x, p);
  mkLeftChild(p, b);
  x->_parent = r;

  if (r != nullptr) {
    if (was_left_child) {
      r->_left = x;
    } else {
      r->_right = x;
    }
  }
}

template <typename K, typename V>
void SplayNode<K, V>::splay()
{
  if (_isRoot()) {
    return;
  }

  if (_parent->_isRoot()) { // zig
    if (_isLeftSon()) {
      _rotateRight();
    } else {
      _rotateLeft();
    }
  } else if (_isLeftSon() == _parent->_isLeftSon()) { // zig-zig
    if (_isLeftSon()) {
      _parent->_rotateRight();
      _rotateRight();
    } else {
      _parent->_rotateLeft();
      _rotateLeft();
    }
  } else { // zig-zag
    if (_isLeftSon()) {
      _rotateRight();
      _rotateLeft();
    } else {
      _rotateLeft();
      _rotateRight();
    }
  }

  splay();
}

//
// Tree struct (a wrapper around pointer to the node)
//

template <typename K, typename V>
struct SplayTree
{
  SplayTree();
  SplayTree(SplayNode<K, V> *);
  // ~SplayTree();

  void splay(SplayNode<K, V> *);
  maybe<V> find(const K &);
  void merge(const SplayTree<K, V> &);
  pair<SplayTree<K, V>, SplayTree<K, V>> split(const K &);
  void add(const K &, const V &);
  void remove(const K &);
  string show();

private:
  SplayNode<K, V> * _root;
};

template <typename K, typename V>
SplayTree<K, V>::SplayTree()
  : _root(nullptr)
{}

template <typename K, typename V>
SplayTree<K, V>::SplayTree(SplayNode<K, V> *node)
  : _root(node)
{}

// template <typename K, typename V>
// SplayTree<K, V>::~SplayTree()
// {
//   if (_root != nullptr) {
//     delete _root;
//   }
// }

template <typename K, typename V>
void SplayTree<K, V>::splay(SplayNode<K, V> *node)
{
  node->splay();
  _root = node;
}

template <typename K, typename V>
maybe<V> SplayTree<K, V>::find(const K &key)
{
  auto cur = _root;
  SplayNode<K, V> *prev = nullptr;
  while (cur != nullptr) {
    if (cur->_key == key) {
      splay(cur);
      return cur->_value;
    } else if (key < cur->_key) {
      prev = cur;
      cur = cur->_left;
    } else {
      prev = cur;
      cur = cur->_right;
    }
  }

  if (prev != nullptr) {
    splay(prev);
  }
  return {};
}

template <typename K, typename V>
void SplayTree<K, V>::merge(const SplayTree<K, V> &other)
{
  if (_root == nullptr) {
    _root = other._root;
    return;
  }

  auto mx = _root;
  while (_root->_left != nullptr) {
    mx = _root->_left;
  }

  splay(mx);
  _root->_right = other._root;
  _root->_right->_parent = _root;
}

template <typename K, typename V>
pair<SplayTree<K, V>, SplayTree<K, V>> SplayTree<K, V>::split(const K &key)
{
  if (_root == nullptr) {
    return make_pair(SplayTree(), SplayTree());
  }

  find(key);
  if (key < _root->_key) {
    auto l = SplayTree(_root->_left);
    _root->_left = nullptr;
    if (l._root != nullptr) {
      l._root->_parent = nullptr;
    }
    return make_pair(l, *this);
  }
  auto r = SplayTree(_root->_right);
  _root->_right = nullptr;
  if (r._root != nullptr) {
    r._root->_parent = nullptr;
  }
  return make_pair(*this, r);
}

template <typename K, typename V>
void SplayTree<K, V>::add(const K &key, const V &val)
{
  auto p = split(key);
  if (_root != nullptr && key == _root->_key) {
    if (p.first._root == _root) {
      mkRightChild(_root, p.second._root);
    } else {
      mkLeftChild(_root, p.first._root);
    }
    _root->_value = val;
  } else {
    auto n = new SplayNode<K, V>(key, val);
    mkLeftChild(n, p.first._root);
    mkRightChild(n, p.second._root);
    _root = n;
  }
}

template <typename K, typename V>
void SplayTree<K, V>::remove(const K &key)
{
  find(key);
  if (_root != nullptr && key == _root->_key) {
    auto old = _root;
    auto l = SplayTree(_root->_left);
    l.merge(SplayTree(_root->_right));
    _root = l._root;
    delete old;
  }
}

template <typename K, typename V>
void ostream_show(ostream &os, SplayNode<K, V> *n, bool left, vector<bool> &lvls)
{
  if (n == nullptr) {
    os << "()\n";
    return;
  }

  for (int i = 0; i < ((int) lvls.size()) - 1; i++) {
    if (lvls[i]) {
      os << '|';
    } else {
      os << ' ';
    }
    os << "         ";
  }

  if (lvls.size() > 0) {
    os << (left ? 'l' : 'r');
    os << "---------";
  }

  os << '(' << n->_key << ',' << n->_value << ")\n";

  if (n->_left != nullptr) {
    for (size_t i = 0; i < lvls.size(); i++) {
      if (lvls[i]) {
        os << '|';
      } else {
        os << ' ';
      }
      os << "         ";
    }
    os << "|\n";

    lvls.push_back(n->_right != nullptr);
    ostream_show(os, n->_left, true, lvls);
    lvls.pop_back();
  }

  if (n->_right != nullptr) {
    for (size_t i = 0; i < lvls.size(); i++) {
      if (lvls[i]) {
        os << '|';
      } else {
        os << ' ';
      }
      os << "         ";
    }
    os << "|\n";

    lvls.push_back(false);
    ostream_show(os, n->_right, false, lvls);
    lvls.pop_back();
  }
}

template <typename K, typename V>
string SplayTree<K, V>::show()
{
  stringstream ss;
  vector<bool> lvls;
  ostream_show(ss, _root, false, lvls);
  return ss.str();
}

#endif
