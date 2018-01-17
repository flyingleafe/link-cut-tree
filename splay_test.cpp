#include <iostream>
#include <string>
#include "headers/splay_tree.h"

using namespace std;

int main(int argc, char *argv[])
{
  int n;
  int k, v;
  string cmd;
  SplayTree<int, int> tree = {};
  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> cmd;
    if (cmd.compare("add") == 0) {
      cin >> k >> v;
      tree.add(k, v);
      cout << "added\n";
    } else if (cmd.compare("remove") == 0) {
      cin >> k;
      tree.remove(k);
    } else if (cmd.compare("find") == 0) {
      cin >> k;
      int val = tree.find(k).value_or(-1);
      if (val != -1) {
        cout << val << endl;
      } else {
        cout << "None\n";
      }
    }
  }
}
