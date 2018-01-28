#include <iostream>
#include <vector>
#include <sstream>
#include <utility>
#include <string>
#include "../headers/splay_tree.h"
#include "../headers/util.h"

using namespace std;

// util functions
// parent != nullptr
void mkLeftChild(SplayNode *parent, SplayNode *child)
{
  parent->_left = child;
  if (child != nullptr) {
    child->_parent = parent;
  }
}

// parent != nullptr
void mkRightChild(SplayNode *parent, SplayNode *child)
{
  parent->_right = child;
  if (child != nullptr) {
    child->_parent = parent;
  }
}

SplayNode::SplayNode(const int &key, const long long &val)
  : _key(key), _value(val), _dW(val), _dMin(0)
  , _parent(nullptr), _left(nullptr), _right(nullptr)
{}

SplayNode::~SplayNode()
{
  if (_left != nullptr) {
    delete _left;
  }
  if (_right != nullptr) {
    delete _right;
  }
}

bool SplayNode::_isRoot()
{
  return _parent == nullptr;
}

bool SplayNode::_isLeftSon()
{
  return !_isRoot() && _parent->_left == this;
}

bool SplayNode::_isRightSon()
{
  return !_isRoot() && !_isLeftSon();
}

void updMins(SplayNode *x)
{
  if (x == nullptr) return;

  long long dm = 0;
  if (x->_left != nullptr) {
    dm = min(dm, x->_left->_dMin + x->_left->_dW);
  }
  if (x->_right != nullptr) {
    dm = min(dm, x->_right->_dMin + x->_right->_dW);
  }
  x->_dMin = dm;
}

void pushDeltas(SplayNode *x, SplayNode *p, SplayNode *b)
{
  auto dWx = x->_dW;
  auto dWp = p->_dW;
  x->_dW = dWx + dWp;
  p->_dW = -dWx;

  if (b != nullptr) {
    b->_dW += dWx;
  }
}

void SplayNode::_rotateLeft()
{
  auto x = this;
  auto b = x->_left;
  auto p = x->_parent;
  auto r = p->_parent;
  bool was_left_child = r == nullptr ? false : p->_isLeftSon() ? true : false;
  pushDeltas(x, p, b);

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
  updMins(p);
  updMins(x);
}

void SplayNode::_rotateRight()
{
  auto x = this;
  auto b = x->_right;
  auto p = x->_parent;
  auto r = p->_parent;
  bool was_left_child = r == nullptr ? false : p->_isLeftSon() ? true : false;
  pushDeltas(x, p, b);

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
  updMins(p);
  updMins(x);
}

void SplayNode::splay()
{
  if (_isRoot()) {
    _value = _dW;
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

SplayTree::SplayTree()
  : _root(nullptr)
{}

SplayTree::SplayTree(SplayNode *node)
  : _root(node)
{}

void SplayTree::splay(SplayNode *node)
{
  node->splay();
  _root = node;
}

maybe<long long> SplayTree::find(const int &key)
{
  auto cur = _root;
  SplayNode *prev = nullptr;
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

void SplayTree::merge(const SplayTree &other)
{
  if (_root == nullptr) {
    _root = other._root;
    return;
  }

  auto mx = _root;
  while (mx->_right != nullptr) {
    mx = mx->_right;
  }

  splay(mx);
  _root->_right = other._root;
  _root->_right->_parent = _root;
  _root->_right->_dW -= _root->_value;
  updMins(_root);
}

pair<SplayTree, SplayTree> SplayTree::split(const int &key)
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
      auto lv = _root->_dW + l._root->_dW;
      l._root->_value = l._root->_dW = lv;
    }
    updMins(_root);
    return make_pair(l, *this);
  }
  auto r = SplayTree(_root->_right);
  _root->_right = nullptr;
  if (r._root != nullptr) {
    r._root->_parent = nullptr;
    auto rv = _root->_dW + r._root->_dW;
    r._root->_value = r._root->_dW = rv;
  }
  updMins(_root);
  return make_pair(*this, r);
}

void SplayTree::add(const int &key, const long long &val)
{
  auto p = split(key);
  if (_root != nullptr && key == _root->_key) {
    auto vdiff = _root->_value - val;
    if (p.first._root == _root) {
      mkRightChild(_root, p.second._root);
      if (p.second._root != nullptr) {
        p.second._root->_dW -= val;
      }
      if (_root->_left != nullptr) {
        _root->_left->_dW += vdiff;
      }
    } else {
      mkLeftChild(_root, p.first._root);
      if (p.first._root != nullptr) {
        p.first._root->_dW -= val;
      }
      if (_root->_right != nullptr) {
        _root->_right->_dW += vdiff;
      }
    }
    _root->_value = _root->_dW = val;
    updMins(_root);
  } else {
    auto n = new SplayNode(key, val);
    mkLeftChild(n, p.first._root);
    if (p.first._root != nullptr) {
      p.first._root->_dW -= val;
    }
    mkRightChild(n, p.second._root);
    if (p.second._root != nullptr) {
      p.second._root->_dW -= val;
    }
    _root = n;
    updMins(_root);
  }
}

void SplayTree::remove(const int &key)
{
  find(key);
  if (_root != nullptr && key == _root->_key) {
    auto old = _root;
    if (old->_left != nullptr) {
      old->_left->_value = old->_left->_dW = old->_value + old->_left->_dW;
      old->_left->_parent = nullptr;
    }
    if (old->_right != nullptr) {
      old->_right->_value = old->_right->_dW = old->_value + old->_right->_dW;
      old->_right->_parent = nullptr;
    }
    auto l = SplayTree(old->_left);
    l.merge(SplayTree(old->_right));
    _root = l._root;
    old->_left = nullptr;
    old->_right = nullptr;
    delete old;
  }
}

void SplayTree::increase(const int &key, const long long &val)
{
  find(key);
  if (_root != nullptr) {
    if (_root->_key <= key) {
      _root->_dW += val;
      if (_root->_right != nullptr) {
        _root->_right->_dW -= val;
      }
    } else {
      if (_root->_left != nullptr) {
        _root->_left->_dW += val;
      }
    }
  }
}

maybe<long long> SplayTree::min(const int &key)
{
  find(key);
  if (_root == nullptr) {
    return {};
  }
  if (key >= _root->_key) {
    long long dm = _root->_value;
    if (_root->_left != nullptr) {
      dm = std::min(dm, _root->_left->_dMin + _root->_left->_dW + _root->_dW);
    }
    return dm;
  }
  if (_root->_left != nullptr) {
    return _root->_dMin + _root->_left->_dW + _root->_dW;
  }
  return {};
}

void ostream_show(ostream &os, SplayNode *n, bool left, vector<bool> &lvls)
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
    os << "             ";
  }

  if (lvls.size() > 0) {
    os << (left ? 'l' : 'r');
    os << "-------------";
  }

  os << '(' << n->_key << ',' << n->_value << " (" << n->_dW << ")";
  if (n->_parent != nullptr) {
    os << " ^" << n->_parent->_key;
  } else {
    os << " ^X";
  }
  os << ")\n";

  if (n->_left != nullptr) {
    for (size_t i = 0; i < lvls.size(); i++) {
      if (lvls[i]) {
        os << '|';
      } else {
        os << ' ';
      }
      os << "             ";
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
      os << "             ";
    }
    os << "|\n";

    lvls.push_back(false);
    ostream_show(os, n->_right, false, lvls);
    lvls.pop_back();
  }
}

string SplayTree::show()
{
  stringstream ss;
  vector<bool> lvls;
  ostream_show(ss, _root, false, lvls);
  return ss.str();
}
