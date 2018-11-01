#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "../headers/splay_tree.h"
#include "../headers/util.h"

using namespace std;

//
// Splay tree
//

SplayTree::SplayTree()
  : _root(nullptr)
{}

SplayTree::~SplayTree()
{
  // if (_root != nullptr) delete _root;
  // Using alloc instead
}

void SplayTree::append(const long long &v)
{
  int id = (int) size(_root);
  auto n = _alloc.alloc(id, v);
  _root = merge(_root, n);

  // avoiding bamboos
  size_t l = _root->_size / 2;
  _root = find(_root, l);
  _root->splay();
}

maybe<long long> SplayTree::lookup(const int &k)
{
  auto res = find(_root, k);
  if (res == nullptr) {
    return {};
  }
  res->splay();
  _root = res;
  return _root->_dW;
}

void SplayTree::increase(int from, int to, const long long &v)
{
  assert(from <= to);
  if (_root == nullptr) return;

  from = max(from, 0);
  to = min(to, (int) _root->_size - 1);

  if (from == 0) {
    auto res = find(_root, to);
    assert(res != nullptr);

    add(res, v);
    _root = res;
  } else {
    auto pivot = find(_root, from);
    assert(pivot != nullptr);

    auto pair = split(pivot);
    auto res = find(pair.second, to - from);
    add(res, v);
    _root = merge(pair.first, res);
  }
}

maybe<long long> SplayTree::minimum(const int &k)
{
  auto res = find(_root, k);
  if (res == nullptr) {
    return {};
  }
  _root = res;
  return min(_root);
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

  os << '(' << n->_size << " (" << n->_dW << ", " << n->_dMin << "))\n";

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
