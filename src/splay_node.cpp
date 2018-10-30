#include <cassert>
#include <algorithm>
#include "../headers/splay_node.h"

using namespace std;

int size(SplayNode *x)
{
  if (x != nullptr) {
    return x->_size;
  }
  return 0;
}

long long wMin(SplayNode *x)
{
  if (x != nullptr) {
    return x->_dMin + x->_dW;
  }
  return 0;
}

//
// Splay tree node
//

SplayNode::SplayNode(const long long &val)
  : _size(1), _dW(val), _dMin(0)
  , _parent(nullptr), _left(nullptr), _right(nullptr)
{}

SplayNode::SplayNode()
  : SplayNode(0)
{}

SplayNode::~SplayNode()
{
  // if (_left != nullptr) delete _left;
  // if (_right != nullptr) delete _right;
  // Using alloc instead
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
  return !_isRoot() && _parent->_right == this;
}

void updMins(SplayNode *x)
{
  assert(x != nullptr);
  x->_dMin = min(0ll, min(wMin(x->_left), wMin(x->_right)));
}

void updSize(SplayNode *x)
{
  assert(x != nullptr);
  x->_size = size(x->_left) + size(x->_right) + 1;
}

void postRotation(SplayNode *p, SplayNode *x, SplayNode *b)
{
  assert(p != nullptr);
  assert(x != nullptr);

  long long dWp = p->_dW;
  long long dWx = x->_dW;

  x->_dW += dWp;
  p->_dW = -dWx;

  if (b != nullptr) {
    b->_parent = p;
    b->_dW += dWx;
  }

  updSize(p);
  updSize(x);
  updMins(p);
  updMins(x);

  auto r = x->_parent;
  if (r != nullptr) {
    if (p == r->_left) {
      r->_left = x;
    } else {
      assert(p == r->_right);
      r->_right = x;
    }
  }
}

// Left rotation.
// Node and its right child should exist.
void rotateLeft(SplayNode *p)
{
  assert(p != nullptr);
  auto x = p->_right;
  assert(x != nullptr);
  auto b = x->_left;

  x->_left = p;
  x->_parent = p->_parent;
  p->_right = b;
  p->_parent = x;

  postRotation(p, x, b);
}

// Right rotation.
// Node and its left child should exist.
void rotateRight(SplayNode *p)
{
  assert(p != nullptr);
  auto x = p->_left;
  assert(x != nullptr);
  auto b = x->_right;

  x->_right = p;
  x->_parent = p->_parent;
  p->_left = b;
  p->_parent = x;

  postRotation(p, x, b);
}
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

void SplayNode::splay()
{
  while (!_isRoot()) {

    if (_parent->_isRoot()) { // zig
      if (_isLeftSon()) {
        rotateRight(_parent);
      } else {
        rotateLeft(_parent);
      }
    } else if (_isLeftSon() == _parent->_isLeftSon()) { // zig-zig
      if (_isLeftSon()) {
        rotateRight(_parent->_parent);
        rotateRight(_parent);
      } else {
        rotateLeft(_parent->_parent);
        rotateLeft(_parent);
      }
    } else { // zig-zag
      if (_isLeftSon()) {
        rotateRight(_parent);
        rotateLeft(_parent);
      } else {
        rotateLeft(_parent);
        rotateRight(_parent);
      }
    }

  }
}

// Just finds the element - without splay!
// Elements are indexed from 0.
SplayNode * find(SplayNode *x, int k)
{
  while (x != nullptr) {
    int l = size(x->_left);
    if (k < l) {
      x = x->_left;
    } else if (k == l) {
      break;
    } else {
      x = x->_right;
      k = k - l - 1;
    }
  }

  return x;
}

SplayNode * merge(SplayNode *a, SplayNode *b)
{
  if (a == nullptr) return b;
  if (b == nullptr) return a;

  assert(a->_isRoot());
  assert(b->_isRoot());

  b = find(b, 0);
  b->splay();

  b->_left = a;
  a->_parent = b;

  b->_size += a->_size;
  a->_dW -= b->_dW;
  b->_dMin = min(b->_dMin, a->_dMin + a->_dW);

  return b;
}

void split(SplayNode *x, SplayNode **out_a, SplayNode **out_b)
{
  if (x == nullptr) {
    *out_a = nullptr;
    *out_b = nullptr;
    return;
  }

  assert(x->_isRoot());

  x->splay();
  *out_b = x;
  auto a = x->_left;
  *out_a = a;
  if (a == nullptr) return;

  a->_parent = nullptr;
  a->_dW += x->_dW;
  x->_left = nullptr;
  x->_size -= a->_size;
  x->_dMin = min(0ll, wMin(x->_right));
}

void add(SplayNode *x, const long long &c)
{
  assert(x != nullptr);
  x->splay();
  x->_dW += c;

  if (x->_right != nullptr) {
    x->_right->_dW -= c;
  }
}

long long min(SplayNode *x)
{
  assert(x != nullptr);
  x->splay();
  return min(x->_dW, wMin(x->_left) + x->_dW);
}
